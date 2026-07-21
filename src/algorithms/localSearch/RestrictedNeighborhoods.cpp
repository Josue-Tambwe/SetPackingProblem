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
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);


        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t k = 0; k < sorted_deactivated_vars_bound; k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                // linear pruning on activated variables
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





    #if HAS_X86 && HAS_AVX2

    bool findRestrictedTwoOneExchangeSIMDX86(int &first_index_to_deactivate,
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
        const size_t nb_words = solution.getNbConsumedResourceWords();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);


        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t k = 0; k < sorted_deactivated_vars_bound; k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                // linear pruning on activated variables
                size_t bound_j = computeLinearPruningIndexBound(computeIndexRatio(i, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t j = (i+1); j < bound_j; j++){

                    // case of a promissing exchange
                    if(profit[sorted_deactivated_vars[k]] > (profit[sorted_activated_vars[i]] + profit[sorted_activated_vars[j]])){

                        // case of a feasible move
                        if(!checkConflictTwoOneMoveAVX2(nb_words,
                                                        resource_requirements[sorted_activated_vars[i]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[k]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){
                                                    
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

    #endif




    #if HAS_ARM && HAS_NEON

    bool findRestrictedTwoOneExchangeSIMDARM(int &first_index_to_deactivate,
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
        const size_t nb_words = solution.getNbConsumedResourceWords();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);


        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t k = 0; k < sorted_deactivated_vars_bound; k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                // linear pruning on activated variables
                size_t bound_j = computeLinearPruningIndexBound(computeIndexRatio(i, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t j = (i+1); j < bound_j; j++){

                    // case of a promissing exchange
                    if(profit[sorted_deactivated_vars[k]] > (profit[sorted_activated_vars[i]] + profit[sorted_activated_vars[j]])){

                        // case of a feasible move
                        if(!checkConflictTwoOneMoveNEON(nb_words,
                                                        resource_requirements[sorted_activated_vars[i]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[k]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){
                                                    
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

    #endif











    // ---------------------- 1-2 Exchange ------------------------------------

    bool findRestrictedOneTwoExchange(int &first_index_to_activate,
                                    int &second_index_to_activate,
                                    int &to_deactivate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Params &params,
                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);

        double inverse_sorted_deactivated_vars_bound = 1.0 / (static_cast<double>(sorted_deactivated_vars_bound) + 1e-6);
        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t i = 0; i < sorted_deactivated_vars_bound; i++){

            // quadratic pruning on the inner loop within deactivated variables
            size_t bound_j = computeQuadraticPruningIndexBound(computeIndexRatio(i, inverse_sorted_deactivated_vars_bound), 
                                                               sorted_deactivated_vars.size());

            for(size_t j = (i+1); j < bound_j; j++){

                // linear pruning on activated variables
                size_t bound_k = computeLinearPruningIndexBound(computeIndexRatio(j, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t k = 0; k < bound_k; k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[i]] + profit[sorted_deactivated_vars[j]]) > profit[sorted_activated_vars[k]]){

                        if(!checkConflictOneTwoMove(resource_requirements[sorted_deactivated_vars[i]].getData(),
                                                    resource_requirements[sorted_deactivated_vars[j]].getData(),
                                                    resource_requirements[sorted_activated_vars[k]].getData(),
                                                    solution.getConsumedResourcesData())){

                            first_index_to_activate = sorted_deactivated_vars[i];
                            second_index_to_activate = sorted_deactivated_vars[j];
                            to_deactivate = sorted_activated_vars[k];
                            return true;

                        }

                    }

                }

            }

        }

        return false;
    }





    #if HAS_X86 && HAS_AVX2

    bool findRestrictedOneTwoExchangeSIMDX86(int &first_index_to_activate,
                                            int &second_index_to_activate,
                                            int &to_deactivate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);

        double inverse_sorted_deactivated_vars_bound = 1.0 / (static_cast<double>(sorted_deactivated_vars_bound) + 1e-6);
        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t i = 0; i < sorted_deactivated_vars_bound; i++){

            // quadratic pruning on the inner loop within deactivated variables
            size_t bound_j = computeQuadraticPruningIndexBound(computeIndexRatio(i, inverse_sorted_deactivated_vars_bound), 
                                                               sorted_deactivated_vars.size());

            for(size_t j = (i+1); j < bound_j; j++){

                // linear pruning on activated variables
                size_t bound_k = computeLinearPruningIndexBound(computeIndexRatio(j, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t k = 0; k < bound_k; k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[i]] + profit[sorted_deactivated_vars[j]]) > profit[sorted_activated_vars[k]]){

                        if(!checkConflictOneTwoMoveAVX2(nb_words,
                                                        resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[k]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){

                            first_index_to_activate = sorted_deactivated_vars[i];
                            second_index_to_activate = sorted_deactivated_vars[j];
                            to_deactivate = sorted_activated_vars[k];
                            return true;

                        }

                    }

                }

            }

        }

        return false;
    }

    #endif




    #if HAS_ARM && HAS_NEON

    bool findRestrictedOneTwoExchangeSIMDARM(int &first_index_to_activate,
                                            int &second_index_to_activate,
                                            int &to_deactivate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        // pruning on deactivated variables
        size_t sorted_deactivated_vars_bound = computeDeactiatedVariablesBound(sorted_deactivated_vars.size(), 
                                                                               params);

        double inverse_sorted_deactivated_vars_bound = 1.0 / (static_cast<double>(sorted_deactivated_vars_bound) + 1e-6);
        double inverse_sorted_activated_vars_bound = 1.0 / (static_cast<double>(sorted_activated_vars.size()) + 1e-6);


        for(size_t i = 0; i < sorted_deactivated_vars_bound; i++){

            // quadratic pruning on the inner loop within deactivated variables
            size_t bound_j = computeQuadraticPruningIndexBound(computeIndexRatio(i, inverse_sorted_deactivated_vars_bound), 
                                                               sorted_deactivated_vars.size());

            for(size_t j = (i+1); j < bound_j; j++){

                // linear pruning on activated variables
                size_t bound_k = computeLinearPruningIndexBound(computeIndexRatio(j, inverse_sorted_activated_vars_bound), 
                                                                sorted_activated_vars.size());

                for(size_t k = 0; k < bound_k; k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[i]] + profit[sorted_deactivated_vars[j]]) > profit[sorted_activated_vars[k]]){

                        if(!checkConflictOneTwoMoveNEON(nb_words,
                                                        resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[k]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){

                            first_index_to_activate = sorted_deactivated_vars[i];
                            second_index_to_activate = sorted_deactivated_vars[j];
                            to_deactivate = sorted_activated_vars[k];
                            return true;

                        }

                    }

                }

            }

        }

        return false;
    }

    #endif


}
