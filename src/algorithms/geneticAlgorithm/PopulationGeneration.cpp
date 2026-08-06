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
                                std::vector<float> &individuals_construction_alpha_value, 
                                std::vector<char> &individuals_local_search,
                                const std::array<float, 10> &alpha_values,
                                const std::vector<std::array<double, 3>> all_proportions){

        // initialization

        std::array<size_t, 10> nb_individuals_per_alpha_value = allocateIndividualsToAlphaValues(nb_individuals_to_generate);

        std::vector<std::array<size_t, 3>> nb_individuals_per_local_search(alpha_values.size());

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


}
