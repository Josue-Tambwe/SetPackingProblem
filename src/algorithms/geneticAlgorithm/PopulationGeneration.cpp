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
 * @file PopulationGeneration.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/PopulationGeneration.hpp"

namespace spp{


    std::array<size_t, 3> allocateIndividualsToMetaheuristics(std::array<double, 3> proportions,
                                                              size_t nb_individuals){

        // initialization
        std::array<size_t, 3> individuals_allocations{};

        size_t cumulative_sum = 0;

        // Variable Neighborhood Descent
        individuals_allocations[0] = static_cast<size_t>(std::floor(nb_individuals * proportions[0]));
        cumulative_sum += individuals_allocations[0];

        // Tabu Search
        individuals_allocations[1] = static_cast<size_t>(std::floor(nb_individuals * proportions[1]));
        cumulative_sum += individuals_allocations[1];

        // Simulated Annealing
        individuals_allocations[2] = static_cast<size_t>(std::floor(nb_individuals * proportions[2]));
        cumulative_sum += individuals_allocations[2];

        // Distribute missing individuals based on the largest fractional part
        if(nb_individuals > cumulative_sum){

            size_t max_index = 0;
            double max_proportion = proportions[0];

            // finding the maximum proportion index
            for(size_t index = 1; index < proportions.size(); index ++){

                if(max_proportion < proportions[index]){max_index = index; max_proportion = proportions[index];}

            }

            // updating the number of individuals
            individuals_allocations[max_index] += (nb_individuals - cumulative_sum);
        }

        return individuals_allocations;
    }





    std::array<size_t, 10> allocateIndividualsToAlphaValues(size_t nb_individuals_to_generate){

        std::array<size_t, 10> individuals_allocations{};

        size_t base = static_cast<size_t>(nb_individuals_to_generate / 10);

        for(size_t i = 0; i < individuals_allocations.size(); i++){individuals_allocations[i] = base;}

        size_t rest = nb_individuals_to_generate % 10;

        for(size_t i = 0; i < rest; i++){individuals_allocations[i] += 1;}

        return individuals_allocations;
    }





    void setAlphaAndLocalSearch(size_t nb_individuals_to_generate,
                                std::array<size_t, 10> &nb_individuals_per_alpha_value,
                                std::vector<std::array<size_t, 3>> &nb_individuals_per_local_search, 
                                std::vector<float> &individuals_construction_alpha_value, 
                                std::vector<char> &individuals_local_search,
                                const std::array<float, 10> &alpha_values,
                                const std::vector<std::array<double, 3>> &all_proportions){

        // initialization

        nb_individuals_per_alpha_value = allocateIndividualsToAlphaValues(nb_individuals_to_generate);

        nb_individuals_per_local_search.resize(alpha_values.size());

        for(size_t i = 0; i < all_proportions.size(); i++){

            nb_individuals_per_local_search[i] = allocateIndividualsToMetaheuristics(all_proportions[i], 
                                                                                     nb_individuals_per_alpha_value[i]);
        }

        individuals_construction_alpha_value.resize(nb_individuals_to_generate);
        individuals_local_search.resize(nb_individuals_to_generate);

        size_t inner_counter = 0;

        for(size_t i = 0; i < nb_individuals_per_alpha_value.size(); i++){

            // Variable Neighborhood Descent
            for(size_t j = 0; j < nb_individuals_per_local_search[i][0]; j++){

                individuals_construction_alpha_value[inner_counter] = alpha_values[i];
                individuals_local_search[inner_counter] = 'v';
                inner_counter += 1;

            }

            // Tabu Search
            for(size_t j = 0; j < nb_individuals_per_local_search[i][1]; j++){

                individuals_construction_alpha_value[inner_counter] = alpha_values[i];
                individuals_local_search[inner_counter] = 't';
                inner_counter += 1;

            }

            // Simulated Annealing
            for(size_t j = 0; j < nb_individuals_per_local_search[i][2]; j++){

                individuals_construction_alpha_value[inner_counter] = alpha_values[i];
                individuals_local_search[inner_counter] = 's';
                inner_counter += 1;

            }
        }

    }




    Solution generateIndividual(float alpha,
                                char local_search,
                                const Params &params,
                                const Instance &instance){

        // construction
        Solution individual = randomizedConstruction(alpha, instance);

        // local search

        if(local_search == 'v'){

            variableNeighborhoodDescentImprovement(individual,
                                                   params,
                                                   instance);
        }

        else if(local_search == 't'){

            tabuSearchImprovement(alpha,
                                  individual,
                                  params,
                                  instance);
        }

        else{

            simulatedAnnealingImprovement(alpha,
                                          individual,
                                          params,
                                          instance);
        }

        return individual;
    }




    void generateIndividualsSingleThread(int start,
                                         int end,
                                         std::vector<Solution> &individuals,
                                         const std::vector<float> &individuals_construction_alpha_value,
                                         const std::vector<char> &individuals_local_search,
                                         const Params &params,
                                         const Instance &instance){


        for(int index = start; index <= end; index++){

            individuals[index] = generateIndividual(individuals_construction_alpha_value[index],
                                                individuals_local_search[index],
                                                params,
                                                instance);

        }


    }





    std::vector<Solution> generateIndividuals(const std::vector<float> &individuals_construction_alpha_value,
                                              const std::vector<char> &individuals_local_search,
                                              const Params &params,
                                              const Instance &instance){

        // initialization
        std::vector<Solution> individuals(individuals_local_search.size(), 
                                          Solution(instance));

        int work_size = static_cast<int>(individuals.size());
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            workers[id] = std::thread(generateIndividualsSingleThread,
                                      start,
                                      end,
                                      std::ref(individuals),
                                      std::ref(individuals_construction_alpha_value),
                                      std::ref(individuals_local_search),
                                      std::ref(params),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

        return individuals;
    }





    void updateLowLevelLocalSearchProportions(std::vector<std::array<double, 3>> &all_proportions,
                                              const std::array<size_t, 10> &nb_individuals_per_alpha_value,
                                              const std::vector<std::array<size_t, 3>> &nb_individuals_per_local_search,
                                              std::vector<Solution> &individuals,
                                              const Instance &instance){

        size_t inner_counter = 0;

        for(size_t i = 0; i < nb_individuals_per_alpha_value.size(); i++){

            // Variable Neighborhood Descent
            std::int64_t cumulative_VND_fitness = 0;

            for(size_t j = 0; j < nb_individuals_per_local_search[i][0]; j++){

                cumulative_VND_fitness += individuals[inner_counter].getObjectiveValue(instance);
                inner_counter += 1;
            }

            // Tabu Search
            std::int64_t cumulative_TS_fitness = 0;

            for(size_t j = 0; j < nb_individuals_per_local_search[i][1]; j++){

                cumulative_TS_fitness += individuals[inner_counter].getObjectiveValue(instance);
                inner_counter += 1;
            }

            // Simulated Annealing
            std::int64_t cumulative_SA_fitness = 0;

            for(size_t j = 0; j < nb_individuals_per_local_search[i][2]; j++){

                cumulative_SA_fitness += individuals[inner_counter].getObjectiveValue(instance);
                inner_counter += 1;
            }

            double average_VND_fitness = static_cast<double>(cumulative_VND_fitness) / std::max(static_cast<size_t>(1), nb_individuals_per_local_search[i][0]);

            double average_TS_fitness = static_cast<double>(cumulative_TS_fitness) / std::max(static_cast<size_t>(1), nb_individuals_per_local_search[i][1]);

            double average_SA_fitness = static_cast<double>(cumulative_SA_fitness) / std::max(static_cast<size_t>(1), nb_individuals_per_local_search[i][2]);

            double cumulative_inverse = 1.0 / (average_VND_fitness + average_TS_fitness + average_SA_fitness + 1e-9);

            // updating proportions

            // Variable Neighborhood Descent
            all_proportions[i][0] = average_VND_fitness * cumulative_inverse;

            // Tabu Search 
            all_proportions[i][1] = average_TS_fitness * cumulative_inverse;

            // Tabu Search 
            all_proportions[i][2] = average_SA_fitness * cumulative_inverse;

        }

    }


}