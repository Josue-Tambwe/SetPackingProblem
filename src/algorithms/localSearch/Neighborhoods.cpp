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

    // ---------------------- 2-1 Exchange ------------------------------------


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

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

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



    // ---------------------- 1-1 Exchange ------------------------------------

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



    // ---------------------- 0-1 Exchange ------------------------------------

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





    // ---------------------- 1-2 Exchange ------------------------------------


    size_t findIterationBoundFirstToActivate(int to_deactivate_index,
                                            std::vector<int> &sorted_deactivated_vars,
                                            const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();

        for(size_t index = 1; (index + 1) < sorted_deactivated_vars.size(); index++){

            if((profit[sorted_deactivated_vars[0]] + profit[sorted_deactivated_vars[index]]) < profit[to_deactivate_index]){

                return index;
            }
        }

        return 0;

    }



    bool findOneTwoExchange(int &to_deactivate,
                           int &first_index_to_activate,
                           int &second_index_to_activate,
                           std::vector<int> &sorted_activated_vars,
                           std::vector<int> &sorted_deactivated_vars,
                           Solution &solution,
                           const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        for(size_t i = 0; i < sorted_activated_vars.size(); i++){

            size_t bound_j = findIterationBoundFirstToActivate(sorted_activated_vars[i], 
                                                               sorted_deactivated_vars, 
                                                               instance);
            size_t j = 0;

            while(j < bound_j){

                for(size_t k = (j+1); k < sorted_deactivated_vars.size(); k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[j]] + profit[sorted_deactivated_vars[k]]) > profit[sorted_activated_vars[i]]){

                        if(!checkConflictOneTwoMove(resource_requirements[sorted_deactivated_vars[j]].getData(),
                                                   resource_requirements[sorted_deactivated_vars[k]].getData(),
                                                   resource_requirements[sorted_activated_vars[i]].getData(),
                                                   solution.getConsumedResourcesData())){

                            to_deactivate = sorted_activated_vars[i];
                            first_index_to_activate = sorted_deactivated_vars[j];
                            second_index_to_activate = sorted_deactivated_vars[k];
                            return true;

                        }

                    }

                }

                j += 1;
            }

        }

        return false;

    }





    #if HAS_AVX2

    bool findTwoOneExchangeSIMDX86(int &first_index_to_deactivate,
                                   int &second_index_to_deactivate,
                                   int &index_to_activate,
                                   std::vector<int> &sorted_activated_vars,
                                   std::vector<int> &sorted_deactivated_vars,
                                   Solution &solution,
                                   const Instance &instance){


        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t k = 0; k < sorted_deactivated_vars.size(); k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                for(size_t j = (i+1); j < sorted_activated_vars.size(); j++){

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




    bool findOneOneExchangeSIMDX86(int &index_to_deactivate,
                                   int &index_to_activate,
                                   std::vector<int> &sorted_activated_vars,
                                   std::vector<int> &sorted_deactivated_vars,
                                   Solution &solution,
                                   const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a promissing exchange
                if(profit[sorted_deactivated_vars[i]] > profit[sorted_activated_vars[j]]){

                    // case of a feasible move
                    if(!checkConflictOneOneMoveAVX2(nb_words, 
                                                    resource_requirements[sorted_activated_vars[j]].getPointerToData(),
                                                    resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                    solution.getConsumedResourcesPointerToData())){
                                                    
                        index_to_deactivate = sorted_activated_vars[j];
                        index_to_activate = sorted_deactivated_vars[i];
                        return true;

                    }
                }
            }
        }

        return false;
        
    }





    bool findZeroOneExchangeSIMDX86(int &index_to_activate,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Instance &instance){


        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a feasible move
            if(!checkConflictZeroOneMoveAVX2(nb_words,
                                             resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                             solution.getConsumedResourcesPointerToData())){
                                                    
                index_to_activate = sorted_deactivated_vars[i];
                return true;

            }
        }

        return false;
        
    }



    bool findOneTwoExchangeSIMDX86(int &to_deactivate,
                                int &first_index_to_activate,
                                int &second_index_to_activate,
                                std::vector<int> &sorted_activated_vars,
                                std::vector<int> &sorted_deactivated_vars,
                                Solution &solution,
                                const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_activated_vars.size(); i++){

            size_t bound_j = findIterationBoundFirstToActivate(sorted_activated_vars[i], 
                                                               sorted_deactivated_vars, 
                                                               instance);
            size_t j = 0;

            while(j < bound_j){

                for(size_t k = (j+1); k < sorted_deactivated_vars.size(); k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[j]] + profit[sorted_deactivated_vars[k]]) > profit[sorted_activated_vars[i]]){

                        if(!checkConflictOneTwoMoveAVX2(nb_words,
                                                        resource_requirements[sorted_deactivated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[k]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[i]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){

                            to_deactivate = sorted_activated_vars[i];
                            first_index_to_activate = sorted_deactivated_vars[j];
                            second_index_to_activate = sorted_deactivated_vars[k];
                            return true;

                        }

                    }

                }

                j += 1;
            }

        }

        return false;

    }

    #endif




    #if HAS_NEON

    bool findTwoOneExchangeSIMDARM(int &first_index_to_deactivate,
                                   int &second_index_to_deactivate,
                                   int &index_to_activate,
                                   std::vector<int> &sorted_activated_vars,
                                   std::vector<int> &sorted_deactivated_vars,
                                   Solution &solution,
                                   const Instance &instance){


        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t k = 0; k < sorted_deactivated_vars.size(); k++){

            for(size_t i = 0; (i+1) < sorted_activated_vars.size(); i++){

                for(size_t j = (i+1); j < sorted_activated_vars.size(); j++){

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




    bool findOneOneExchangeSIMDARM(int &index_to_deactivate,
                                   int &index_to_activate,
                                   std::vector<int> &sorted_activated_vars,
                                   std::vector<int> &sorted_deactivated_vars,
                                   Solution &solution,
                                   const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a promissing exchange
                if(profit[sorted_deactivated_vars[i]] > profit[sorted_activated_vars[j]]){

                    // case of a feasible move
                    if(!checkConflictOneOneMoveNEON(nb_words, 
                                                    resource_requirements[sorted_activated_vars[j]].getPointerToData(),
                                                    resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                    solution.getConsumedResourcesPointerToData())){
                                                    
                        index_to_deactivate = sorted_activated_vars[j];
                        index_to_activate = sorted_deactivated_vars[i];
                        return true;

                    }
                }
            }
        }

        return false;
        
    }




    bool findZeroOneExchangeSIMDARM(int &index_to_activate,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Instance &instance){


        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a feasible move
            if(!checkConflictZeroOneMoveNEON(nb_words,
                                             resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                             solution.getConsumedResourcesPointerToData())){
                                                    
                index_to_activate = sorted_deactivated_vars[i];
                return true;

            }
        }

        return false;
        
    }




    bool findOneTwoExchangeSIMDARM(int &to_deactivate,
                                    int &first_index_to_activate,
                                    int &second_index_to_activate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Instance &instance){

        const std::vector<int>& profit = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_activated_vars.size(); i++){

            size_t bound_j = findIterationBoundFirstToActivate(sorted_activated_vars[i], 
                                                               sorted_deactivated_vars, 
                                                               instance);
            size_t j = 0;

            while(j < bound_j){

                for(size_t k = (j+1); k < sorted_deactivated_vars.size(); k++){

                    // case of a promissing exchange
                    if((profit[sorted_deactivated_vars[j]] + profit[sorted_deactivated_vars[k]]) > profit[sorted_activated_vars[i]]){

                        if(!checkConflictOneTwoMoveNEON(nb_words,
                                                        resource_requirements[sorted_deactivated_vars[j]].getPointerToData(),
                                                        resource_requirements[sorted_deactivated_vars[k]].getPointerToData(),
                                                        resource_requirements[sorted_activated_vars[i]].getPointerToData(),
                                                        solution.getConsumedResourcesPointerToData())){

                            to_deactivate = sorted_activated_vars[i];
                            first_index_to_activate = sorted_deactivated_vars[j];
                            second_index_to_activate = sorted_deactivated_vars[k];
                            return true;

                        }

                    }

                }

                j += 1;
            }

        }

        return false;

    }

    #endif



 }
 