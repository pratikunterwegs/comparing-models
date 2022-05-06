#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include "data_types.h"
#include "landscape.h"
#include "agents.h"

class simulation {
public:
    simulation(const int popsize, const int scenario,
               const int nItems, const float landsize,
               const int nClusters,
               const float clusterSpread,
               const int tmax,
               const int genmax,
               const float range_perception,
               const int handling_time,
               const int regen_time,
               const int nThreads,
               const float dispersal,
               const float mProb,
               const float mSize):
        // population, food, and data structures
        pop (scenario, popsize, range_perception, handling_time),
        food(nItems, landsize, nClusters, clusterSpread, regen_time),
        gen_data (genmax, popsize, std::max(static_cast<int>(static_cast<float>(genmax) * 0.001f), 2)), // increment hardcoded

        // eco-evolutionary parameters
        scenario(scenario),
        tmax(tmax),
        genmax(genmax),
        g_patho_init(g_patho_init),

        // agent perception and behaviour, food growth
        range_food(range_food),
        range_agents(range_agents),
        range_move(range_move),
        handling_time(handling_time),
        regen_time(regen_time),

        // disease parameters
        initialInfections(initialInfections),
        costInfect(costInfect),
        pTransmit(pTransmit),

        // parallelisation
        nThreads (nThreads),

        // natal dispersal
        dispersal(dispersal),
        infect_percent(infect_percent),

        // mutation probability and step size
        mProb(mProb),
        mSize(mSize),

        // movement data
        mdPre(tmax, popsize),
        mdPost(tmax, popsize)
    {}
    ~simulation() {}

    Population pop;
    Resources food;
    genData gen_data;
    const int scenario, tmax, genmax, g_patho_init;
    const float range_food, range_agents, range_move;
    const int handling_time;

    const int regen_time, initialInfections;
    const float costInfect;
    float pTransmit;
    int nThreads;
    const float dispersal;
    const bool infect_percent;

    const float mProb, mSize;

    moveData mdPre, mdPost;

    // funs
    Rcpp::List do_simulation_mechanistic();
    Rcpp::List do_simulation_random();

};

#endif // SIMULATIONS_H
