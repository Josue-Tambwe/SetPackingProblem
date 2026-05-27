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
 * @file NeighborhoodUtils.hpp.cpp
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #include "algorithms/localSearch/NeighborhoodUtils.hpp"


 namespace spp{


    std::vector<float> computeVariableScores(const Instance &instance){

        std::vector<float> scores(instance.getNbVars());
        const std::vector<int>& profit_vector = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        for(size_t var = 0; var < scores.size(); var++){

            scores[var] = profit_vector[var] / (resource_requirements[var].getNbNonZeroBits() + epsilon);
        }

        return scores;
    }


    std::vector<int> sortNonZeroVars(Solution &solution,
                                     std::vector<float> &scores){
        
                                               
        std::vector<int> sorted_indexes = solution.getNonZeroVarsIndexes();
        std::vector<MoveNode> tmp(sorted_indexes.size());

        for(size_t i = 0; i < sorted_indexes.size(); i++){

            tmp[i] = MoveNode(sorted_indexes[i], 
                              scores[sorted_indexes[i]]);
        }

        std::sort(tmp.begin(), tmp.end(), std::less<>());

        for(size_t i = 0; i < sorted_indexes.size(); i++){

            sorted_indexes[i] = tmp[i].getId();
        }

        return sorted_indexes;

    }



    std::vector<int> sortZeroVars(Solution &solution,
                                  std::vector<float> &scores){


        std::vector<int> sorted_indexes = solution.getZeroVarsIndexes();
        std::vector<MoveNode> tmp(sorted_indexes.size());

        for(size_t i = 0; i < sorted_indexes.size(); i++){

            tmp[i] = MoveNode(sorted_indexes[i], 
                              scores[sorted_indexes[i]]);
        }

        std::sort(tmp.begin(), tmp.end(), std::greater<>());

        for(size_t i = 0; i < sorted_indexes.size(); i++){

            sorted_indexes[i] = tmp[i].getId();
        }

        return sorted_indexes;
    }

 }