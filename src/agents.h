#ifndef AGENTS_H
#define AGENTS_H

#define _USE_MATH_DEFINES
/// code to make agents
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <boost/foreach.hpp>
#include "landscape.h"
#include "network.h"


// Agent class
struct Population {
public:
    Population(const int popsize, const float range_perception, 
        const int handling_time, const float max_dispersal) :
        // agents, positions, energy and traits
        nAgents (popsize),
        coordX (popsize, 0.0f),
        coordY (popsize, 0.0f),
        initX (popsize, 0.0f),
        initY (popsize, 0.0f),
        intake (popsize, 0.001f),
        energy (popsize, 0.001f),
        sF (popsize, 0.f),
        sH (popsize, 0.f),
        sN (popsize, 0.f),
        
        wF (popsize, 0.f),
        wH (popsize, 0.f),
        wN (popsize, 0.f),
        w0 (popsize, 0.f),

        // counters for handling and social metrics
        counter (popsize, 0),
        associations(popsize, 0),

        // agent sensory parameters
        n_samples (5.f),
        range_perception(range_perception),
        max_dispersal(max_dispersal),
        handling_time(handling_time),

        // vectors for agent order
        order(popsize, 1),
        forageItem(popsize, -1),
        
        // distance moved
        moved(popsize, 0.f),
        dispersal(popsize, 0.f),

        // a network object
        pbsn(popsize)
    {}
    ~Population() {}

    // agent count, coords, and energy
    const int nAgents;
    std::vector<float> coordX, coordY, initX, initY, intake, energy;

    // weights for angles
    std::vector<float> sF, sH, sN;

    // weights for distances
    std::vector<float> wF, wH, wN, w0;

    // counter and metrics
    std::vector<int> counter, associations; // number of total interactions

    // sensory range and foraging
    const float n_samples, range_perception, max_dispersal;
    const int handling_time;

    // shuffle vector and transmission
    std::vector<int> order;
    std::vector<int> forageItem;

    // movement distances
    std::vector<float> moved, dispersal;

    // position rtree
    bgi::rtree< value, bgi::quadratic<16> > agentRtree;

    // network object
    Network pbsn;

    /// functions for the population ///
    // population order, trait and position randomiser
    void shufflePop();
    void setTrait (const float mSize);
    void initPos(Resources food);

    // make rtree and get nearest agents and food
    void updateRtree();

    int countFood (const Resources &food, const float x, const float y);
    
    std::vector<int> getFoodId (
        const Resources &food,
        const float x, const float y
    );
    
    std::pair<int, int > countAgents (
        const float x, const float y);
    
    std::vector<int> getNeighbourId (
        const float x, const float y
    );

    // functions to move and forage on a landscape
    void move(const Resources &food);
    void do_natal_dispersal(const Resources &food, const float max_dispersal);

    void pickForageItem(const Resources &food, const int nThreads);
    void doForage(Resources &food);
    
    // funs to handle fitness and reproduce
    std::vector<float> handleFitness();
    void Reproduce(const Resources food, 
        const float mProb,
        const float mSize
    );
    
    // counting proximity based interactions
    void countAssoc(const int nThreads);

    // return population data
    Rcpp::DataFrame returnPopData();
};

// a dinky function for distance and passed to catch test
float get_distance(float x1, float x2, float y1, float y2);

#endif // AGENTS_H
