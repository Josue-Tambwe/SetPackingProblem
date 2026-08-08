/*------------------------------------------------------------------------------
 * SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
 *              and efficient BitVector techniques.
 *------------------------------------------------------------------------------
 * Copyright (c) 2026 Josué Tambwe
 *               Nantes Université, France
 *------------------------------------------------------------------------------
 * This software is released under the MIT License.
 * See the LICENSE file for more details.
 *------------------------------------------------------------------------------
 */


/** 
 * @file GeneticAlgorithmUtils.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"


namespace spp{


    void computePopulationFitnessOneThread(int start,
                                           int end,
                                           std::vector<double> &population_fitness,
                                           std::vector<Solution> &population,
                                           const Instance &instance){

        for(int index = start; index <= end; index++){

            population_fitness[index] = static_cast<double>(population[index].getObjectiveValue(instance));
        }
    }




    std::vector<double> computePopulationFitness(std::vector<Solution> &population,
                                                 const Params &params,
                                                 const Instance &instance){
        
        // initialization
        std::vector<double> population_fitness(population.size());

        int work_size = static_cast<int>(population.size());
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            workers[id] = std::thread(computePopulationFitnessOneThread,
                                      start,
                                      end,
                                      std::ref(population_fitness),
                                      std::ref(population),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

        return population_fitness;
    }
    
}