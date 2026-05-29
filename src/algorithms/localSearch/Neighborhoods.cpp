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
 * @file Neighborhoods.cpp
 * @author Josué Tambwe
 * @date 29 May 2026
 */

 #include "algorithms/localSearch/Neighborhoods.hpp"

 namespace spp{


    bool findTwoOneExchange(int &first_index_to_deactivate,
                            int &second_index_to_deactivate,
                            int &index_to_activate,
                            std::vector<int> &sorted_activated_vars,
                            std::vector<int> &sorted_deactivated_vars,
                            Solution &solution,
                            const Instance &instance){


        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();


        for(size_t k = 0; k < sorted_deactivated_vars.size(); k++){

            for(size_t i = 0; i < (sorted_activated_vars.size() - 1); i++){

                for(size_t j = (i+1); j < sorted_activated_vars.size(); j++){

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




    bool findOneOneExchange(int &index_to_deactivate,
                           int &index_to_activate,
                           std::vector<int> &sorted_activated_vars,
                           std::vector<int> &sorted_deactivated_vars,
                           Solution &solution,
                           const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a promissing exchange
                if(profit[sorted_deactivated_vars[i]] > profit[sorted_activated_vars[j]]){

                    // case of a feasible move
                    if(!checkConflictOneOneMove(resource_requirements[sorted_activated_vars[j]].getData(),
                                                resource_requirements[sorted_deactivated_vars[i]].getData(),
                                                solution.getConsumedResourcesData())){
                                                    
                        index_to_deactivate = sorted_activated_vars[j];
                        index_to_activate = sorted_deactivated_vars[i];
                        return true;

                    }
                }
            }
        }

        return false;
        
    }




    bool findZeroOneExchange(int &index_to_activate,
                             std::vector<int> &sorted_deactivated_vars,
                             Solution &solution,
                             const Instance &instance){

        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a feasible move
            if(!checkConflictZeroOneMove(resource_requirements[sorted_deactivated_vars[i]].getData(),
                                        solution.getConsumedResourcesData())){
                                                    
                index_to_activate = sorted_deactivated_vars[i];
                return true;

            }
        }

        return false;
        
    }



 }
 