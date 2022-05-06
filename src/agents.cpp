#define _USE_MATH_DEFINES
/// code to make agents
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <random>

#include <boost/foreach.hpp>

#include <Rcpp.h>
#include <RcppParallel.h>

#include "network.h"
#include "landscape.h"
#include "agents.h"

// to shuffle pop id
void Population::shufflePop() {
    if (order[0] == order[nAgents - 1])
    {
        for (size_t i = 0; i < static_cast<size_t>(nAgents); i++)
        {
            order[i] = i;
        }
        std::random_shuffle ( order.begin(), order.end() );
    }
    else {
        std::random_shuffle ( order.begin(), order.end() );
    }
    
}

// to update agent Rtree
void Population::updateRtree () {
    // initialise rtree
    bgi::rtree< value, bgi::quadratic<16> > tmpRtree;
    for (int i = 0; i < nAgents; ++i)
    {
        point p = point(coordX[i], coordY[i]);
        tmpRtree.insert(std::make_pair(p, i));
    }
    std::swap(agentRtree, tmpRtree);
    tmpRtree.clear();
}

// uniform distribution for agent position
std::uniform_real_distribution<float> agent_ran_pos(0.0f, 1.f);

// function for initial positions
void Population::initPos(Resources food) {
    for (size_t i = 0; i < static_cast<size_t>(nAgents); i++) {
        coordX[i] = agent_ran_pos(rng) * food.dSize;
        initX[i] = coordX[i];
        coordY[i] = agent_ran_pos(rng) * food.dSize;
        initY[i] = coordY[i];
    }
    updateRtree();
}

// set agent trait
void Population::setTrait(const float mSize) {

    // create a cauchy distribution, mSize is the scale
    std::cauchy_distribution<float> agent_ran_trait(0.f, mSize);

    for(int i = 0; i < nAgents; i++) {
        sF[i] = agent_ran_trait(rng);
        sH[i] = agent_ran_trait(rng);
        sN[i] = agent_ran_trait(rng);
    }
}

float get_distance(float x1, float x2, float y1, float y2) {
    return std::sqrt(std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2));
}

// general function for agents within distance
std::pair<int, int> Population::countAgents (
    const float xloc, const float yloc) {
    
    int handlers = 0;
    int nonhandlers = 0;
    std::vector<value> near_agents;
    // query for a simple box
    agentRtree.query(bgi::satisfies([&](value const& v) {
        return bg::distance(v.first, point(xloc, yloc)) < range_perception;}),
        std::back_inserter(near_agents));

    BOOST_FOREACH(value const& v, near_agents) {
        
        if(counter[v.second] > 0) handlers ++; else nonhandlers ++;
    }
    near_agents.clear();
    // first element is number of near entities
    // second is the identity of entities
    return std::pair<int, int> {handlers, nonhandlers};
}

// function for near agent ids
std::vector<int> Population::getNeighbourId (
    const float xloc, const float yloc) {
    
    std::vector<int> agent_id;
    std::vector<value> near_agents;
    // query for a simple box
    // neighbours for associations are counted over the MOVEMENT RANGE
    agentRtree.query(bgi::satisfies([&](value const& v) {
        return bg::distance(v.first, point(xloc, yloc)) < range_perception;}),
        std::back_inserter(near_agents));

    BOOST_FOREACH(value const& v, near_agents) {
        agent_id.push_back(v.second);
    }
    near_agents.clear();
    // first element is number of near entities
    // second is the identity of entities
    return agent_id;
}

// general function for items within distance
int Population::countFood (
    const Resources &food,
    const float xloc, const float yloc) {

    int nFood = 0;
    std::vector<value> near_food;

    // check any available
    if (food.nAvailable > 0) {
        // query for a simple box
        food.rtree.query(bgi::satisfies([&](value const& v) {
            return bg::distance(v.first, point(xloc, yloc)) < range_perception;}),
            std::back_inserter(near_food));

        BOOST_FOREACH(value const& v, near_food) {
            // count only which are available!
            if (food.available[v.second]) {
                nFood++;
            }
        }
        near_food.clear();
    }

    return nFood;
}

// function for the nearest available food item
std::vector<int> Population::getFoodId (
    const Resources &food,
    const float xloc, const float yloc) {
        
    std::vector<int> food_id;
    std::vector<value> near_food;
    // check any available
    if (food.nAvailable > 0) {
        // query for a simple box
        // food is accessed over the MOVEMENT RANGE
        food.rtree.query(bgi::satisfies([&](value const& v) {
            return bg::distance(v.first, point(xloc, yloc)) < range_perception;}), 
            std::back_inserter(near_food));

        BOOST_FOREACH(value const& v, near_food) {
            // count only which are available!
            if (food.available[v.second]) {
                food_id.push_back(v.second);
            }
        }
        near_food.clear();
    }

    // first element is number of near entities
    // second is the identity of entities
    return food_id;
}

/// rng for suitability
std::normal_distribution<float> noise(0.f, 0.01f);
std::cauchy_distribution<float> noise_cauchy(0.f, 0.001f);


// function to paralellise choice of forage item
void Population::pickForageItem(const Resources &food, const int nThreads){
    shufflePop();
    // nearest food
    std::vector<int> idTargetFood (nAgents, -1);

    if (nThreads > 1)
    {
        // loop over agents --- no shuffling required here
        tbb::task_scheduler_init _tbb(tbb::task_scheduler_init::automatic); // automatic for now
        // try parallel foraging --- agents pick a target item
        tbb::parallel_for(
            tbb::blocked_range<unsigned>(1, order.size()),
                [&](const tbb::blocked_range<unsigned>& r) {
                for (unsigned i = r.begin(); i < r.end(); ++i) {
                    if ((counter[i] > 0) | (food.nAvailable == 0)) { 
                        // nothing -- agent cannot forage or there is no food
                    }
                    else {
                        // find nearest item ids
                        std::vector<int> theseItems = getFoodId(food, coordX[i], coordY[i]);
                        int thisItem = -1;

                        // check near items count
                        if(theseItems.size() > 0) {
                            // take first item by default
                            thisItem = theseItems[0];
                            idTargetFood[i] = thisItem;
                        }
                    }
                }
            }
        );
    } else if (nThreads == 1)
    {
        for (int i = 0; i < nAgents; ++i) {
            if ((counter[i] > 0) | (food.nAvailable == 0)) { 
                // nothing -- agent cannot forage or there is no food
            }
            else {
                // find nearest item ids
                std::vector<int> theseItems = getFoodId(food, coordX[i], coordY[i]);
                int thisItem = -1;

                // check near items count
                if(theseItems.size() > 0) {
                    // take first item by default
                    thisItem = theseItems[0];
                    idTargetFood[i] = thisItem;
                }
            }
        }
    }

    forageItem = idTargetFood;
}

// function to exploitatively forage on picked forage items
void Population::doForage(Resources &food) {
    // all agents have picked a food item if they can forage
    // now forage in a serial loop --- this cannot be parallelised
    // this order is randomised
    for (size_t i = 0; i < static_cast<size_t>(nAgents); i++)
    {
        int id = order[i];
        if ((counter[id] > 0) | (food.nAvailable == 0)) {
            // nothing
        } else {
            int thisItem = forageItem[id]; //the item picked by this agent
            // check selected item is available
            if (thisItem != -1)
            {
                counter[id] = handling_time;
                intake[id] += 1.0; // increased here --- not as described.

                // reset food availability
                food.available[thisItem] = false;
                food.counter[thisItem] = food.regen_time;
                food.nAvailable --;
            }
        }
    }
}

void Population::countAssoc(const int nThreads) {
    for (int i = 0; i < nAgents; ++i) {
        // count nearby agents and update raw associations
        std::vector<int> nearby_agents = getNeighbourId(coordX[i], coordY[i]);
        associations[i] += nearby_agents.size();

        // loop over nearby agents and update association matrix
        for (size_t j = 0; j < nearby_agents.size(); j++)
        {
            int target_agent = nearby_agents[j];
            pbsn.adjMat (i, target_agent) += 1;
        }
    }
}

/// minor function to normalise vector
std::vector<float> Population::handleFitness() {
    // sort vec fitness
    std::vector<float> vecFitness = energy;
    std::sort(vecFitness.begin(), vecFitness.end()); // sort to to get min-max
    // scale to max fitness
    float maxFitness = vecFitness[vecFitness.size()-1];
    float minFitness = vecFitness[0];

    // reset to energy
    vecFitness = energy;
    // rescale copied energy vector by min anx max fitness
    for(size_t i = 0; i < static_cast<size_t>(nAgents); i++) {
        vecFitness[i] = ((vecFitness[i]  - minFitness) / (maxFitness - minFitness)) +
         noise(rng);
    }
    
    return vecFitness;
}

// fun for replication
void Population::Reproduce(const Resources food,
    const float dispersal, const float mProb, const float mSize) 
{
    // mutation probability and size distribution --- inefficient but oh well
    std::bernoulli_distribution mutation_happens(mProb);
    std::cauchy_distribution<float> mutation_size(0.0, mSize);

    // choose the range over which individuals are dispersed
    std::normal_distribution<float> sprout(0.f, dispersal);
    std::vector<float> vecFitness;
    vecFitness = handleFitness();

    // set up weighted lottery
    std::discrete_distribution<> weightedLottery(vecFitness.begin(), vecFitness.end());

    // get parent trait based on weighted lottery
    std::vector<float> tmp_sF (nAgents, 0.f);
    std::vector<float> tmp_sH (nAgents, 0.f);
    std::vector<float> tmp_sN (nAgents, 0.f);
    
    // reset associations
    associations = std::vector<int> (nAgents, 0);

    // reset distance moved
    moved = std::vector<float> (nAgents, 0.f);

    // reset adjacency matrix
    pbsn.adjMat = Rcpp::NumericMatrix(nAgents, nAgents);

    // positions
    std::vector<float> coord_x_2 (nAgents, 0.f);
    std::vector<float> coord_y_2 (nAgents, 0.f);
    
    for (int a = 0; a < nAgents; a++) {
        size_t parent_id = static_cast<size_t>(weightedLottery(rng));

        tmp_sF[a] = sF[parent_id];
        tmp_sH[a] = sH[parent_id];
        tmp_sN[a] = sN[parent_id];

        // inherit positions from parent
        coord_x_2[a] = coordX[parent_id] + sprout(rng);
        coord_y_2[a] = coordY[parent_id] + sprout(rng);

        // robustly wrap positions
        if(coord_x_2[a] < 0.f) coord_x_2[a] = food.dSize + coord_x_2[a];
        if(coord_x_2[a] > food.dSize) coord_x_2[a] = coord_x_2[a] - food.dSize;

        if(coord_y_2[a] < 0.f) coord_y_2[a] = food.dSize + coord_y_2[a];
        if(coord_y_2[a] > food.dSize) coord_y_2[a] = coord_y_2[a] - food.dSize;

    }

    // swap coords --- this initialises individuals near their parent's position
    std::swap(coordX, coord_x_2);
    std::swap(coordY, coord_y_2);
    coord_x_2.clear(); coord_y_2.clear();

    // update initial positions!
    initX = coordX;
    initY = coordY;

    // reset counter
    counter = std::vector<int> (nAgents, 0);
    assert(static_cast<int>(counter.size()) == nAgents && "counter size wrong");

    // mutate trait: trait shifts up or down with an equal prob
    // trait mutation prob is mProb, in a two step process
    for (int a = 0; a < nAgents; a++) {
        if(mutation_happens(rng)) {
            tmp_sF[a] = tmp_sF[a] + mutation_size(rng);
        }
        if(mutation_happens(rng)) {
            tmp_sH[a] = tmp_sH[a] + mutation_size(rng);
        }
        if(mutation_happens(rng)) {
            tmp_sN[a] = tmp_sN[a] + mutation_size(rng);
        }
    }
    
    // swap trait matrices
    std::swap(sF, tmp_sF);
    std::swap(sH, tmp_sH);
    std::swap(sN, tmp_sN);

    tmp_sF.clear(); tmp_sH.clear(); tmp_sN.clear();
    
    // swap energy
    std::vector<float> tmpEnergy (nAgents, 0.001);
    std::swap(energy, tmpEnergy);
    tmpEnergy.clear();

    // swap intake
    std::vector<float> tmpIntake (nAgents, 0.001);
    std::swap(intake, tmpIntake);
    tmpIntake.clear();
}