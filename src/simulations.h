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
        pop (popsize, range_perception, handling_time, scenario),
        food(nItems, landsize, nClusters, clusterSpread, regen_time),
        gen_data (genmax, popsize, std::max(static_cast<int>(static_cast<float>(genmax) * 0.001f), 
            (scenario == 2 ? 2 : 1))), // increment hardcoded

        // eco-evolutionary parameters
        scenario(scenario),
        tmax(tmax),
        genmax(genmax),

        // agent perception and behaviour, food growth
        range_perception(range_perception),
        handling_time(handling_time),
        regen_time(regen_time),

        // parallelisation
        nThreads (nThreads),

        // natal dispersal
        dispersal(dispersal),
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
    const int scenario, tmax, genmax;
    const float range_perception;
    const int handling_time;

    const int regen_time;
    int nThreads;
    const float dispersal;
    
    const float mProb, mSize;

    moveData mdPre, mdPost;

    // funs
    Rcpp::List do_simulation_mechanistic();
    Rcpp::List do_simulation_random();

};

#endif // SIMULATIONS_H
