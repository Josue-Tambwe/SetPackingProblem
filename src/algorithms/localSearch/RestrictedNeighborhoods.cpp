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
 * @file RestrictedNeighborhoods.cpp
 * @author Josué Tambwe
 * @date 18 July 2026
 */

#include "algorithms/localSearch/RestrictedNeighborhoods.hpp"


namespace spp{

    // ---------------------- 2-1 Exchange ------------------------------------


    bool findRestrictedTwoOneExchange(int &first_index_to_deactivate,
                                    int &second_index_to_deactivate,
                                    int &index_to_activate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Params &params,
                                    const Instance &instance){


        // preprocessing
        if(sorted_activated_vars.size() < 2 || sorted_deactivated_vars.size() < 1){return false;}


        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), params);


        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t k = 0; k < sorted_deactivated_vars_bound; k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                // pruning on activated variables
                size_t bound_j = computeLinearPruningIndexBound(computeIndexRatio(i, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t j = (i+1); j < bound_j; j++){

                    // case of a promissing exchange
                    if(profit[sorted_deactivated_vars[k]] > (profit[sorted_activated_vars[i]] + profit[sorted_activated_vars[j]])){

                        // case of a feasible move
                        if(!checkConflictTwoOneMove(resource_requirements[sorted_activated_vars[i]].getData(),
                                                    resource_requirements[sorted_activated_vars[j]].getData(),
                                                    resource_requirements[sorted_deactivated_vars[k]].getData(),
                                                    solution.getConsumedResourcesData())){
                                                    
                            first_index_to_deactivate = sorted_activated_vars[i];
                            second_index_to_deactivate = sorted_activated_vars[j];
                            index_to_activate = sorted_deactivated_vars[k];
                            return true;

                        }
                    }
                }
            }
        }

        return false;
    }
}
