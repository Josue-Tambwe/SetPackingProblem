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
 * @file Selection.cpp
 * @author Josué Tambwe
 * @date 12 August 2026
 */

#include "algorithms/geneticAlgorithm/Selection.hpp"


namespace spp{


    std::vector<Solution> unifyPopulation(std::vector<Solution> &first_group,
                                          std::vector<Solution> &second_group,
                                          const Instance &instance){

        // initialization
        size_t total_population_size = first_group.size() + second_group.size();
        std::vector<Solution> unified_population(total_population_size, Solution(instance));

        size_t inner_index = 0;

        for(Solution individual : first_group){unified_population[inner_index] = individual; inner_index += 1;}

        for(Solution individual : second_group){unified_population[inner_index] = individual; inner_index += 1;}

        return unified_population;
    }




    std::vector<size_t> computeSurvivorsIndexes(size_t nb_survivors, 
                                                std::vector<Solution> &unified_population,
                                                const Params &params,
                                                const Instance &instance){

        std::vector<double> fitness = computePopulationFitness(unified_population,
                                                               params,
                                                               instance);

        std::vector<MoveNode> tmp(unified_population.size());

        for(int i = 0; i < static_cast<int>(unified_population.size()); i++){

            tmp[i] = MoveNode(i, 
                              static_cast<float>(fitness[i]));
        }

        // sorting by a decreasing order
        std::sort(tmp.begin(), tmp.end(), std::greater<>());

        std::vector<size_t> survivor_indexes(nb_survivors);

        for(size_t i = 0; i < nb_survivors; i++){survivor_indexes[i] = tmp[i].getId();}

        return survivor_indexes;

    }




    std::vector<Solution> computeEliteSurvivors(size_t nb_survivors, 
                                                std::vector<Solution> &unified_population,
                                                const Params &params,
                                                const Instance &instance){


        std::vector<size_t> survivor_indexes = computeSurvivorsIndexes(nb_survivors, 
                                                                       unified_population,
                                                                       params,
                                                                       instance);


        std::vector<Solution> elite_survivors(nb_survivors, Solution(instance));

        for(size_t i = 0; i < nb_survivors; i++){
            
            elite_survivors[i] = unified_population[survivor_indexes[i]];
        
        }

        return elite_survivors;
    }

}
