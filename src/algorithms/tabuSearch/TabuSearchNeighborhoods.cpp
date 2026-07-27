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
 * @file TabuSearchNeighborhoods.cpp
 * @author Josué Tambwe
 * @date 25 July 2026
 */

#include "algorithms/tabuSearch/TabuSearchNeighborhoods.hpp"

namespace spp{



    // ------------------- 2-1 Exchange ----------------------

    bool findRestrictedTwoOneExchangeScalarTabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 2 || sorted_deactivated_vars.size() < 1){return false;}


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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::TwoOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[i]),
                                                          static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[k])
                                                         }
                                                        )
                                               )
                          ){

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

    bool findRestrictedTwoOneExchangeSIMDX86TabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 2 || sorted_deactivated_vars.size() < 1){return false;}


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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::TwoOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[i]),
                                                          static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[k])
                                                         }
                                                        )
                                               )
                          ){

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

    bool findRestrictedTwoOneExchangeSIMDARMTabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 2 || sorted_deactivated_vars.size() < 1){return false;}


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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::TwoOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[i]),
                                                          static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[k])
                                                         }
                                                        )
                                               )
                          ){

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





    void findRestrictedTwoOneExchangeTabuSearch(std::int64_t &best_improvement,
                                                TabuMove &best_move,
                                                Solution &solution,
                                                std::vector<float> &scores,
                                                const TabuList &tabu_list,
                                                const Params &params,
                                                const Instance &instance){

        // initialization

        int first_index_to_deactivate = -1;
        int second_index_to_deactivate = -1;
        int index_to_activate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        bool move_found = false;

        if(params.use_simd){

            #if HAS_AVX2
                
                move_found = findRestrictedTwoOneExchangeSIMDX86TabuSearch(first_index_to_deactivate,
                                                                           second_index_to_deactivate,
                                                                           index_to_activate,
                                                                           sorted_activated_vars,
                                                                           sorted_deactivated_vars,
                                                                           solution,
                                                                           tabu_list,
                                                                           params,
                                                                           instance);

            #elif HAS_NEON

                move_found = findRestrictedTwoOneExchangeSIMDARMTabuSearch(first_index_to_deactivate,
                                                                           second_index_to_deactivate,
                                                                           index_to_activate,
                                                                           sorted_activated_vars,
                                                                           sorted_deactivated_vars,
                                                                           solution,
                                                                           tabu_list,
                                                                           params,
                                                                           instance);
                

            #endif

        }

        else{

            move_found = findRestrictedTwoOneExchangeScalarTabuSearch(first_index_to_deactivate,
                                                                      second_index_to_deactivate,
                                                                      index_to_activate,
                                                                      sorted_activated_vars,
                                                                      sorted_deactivated_vars,
                                                                      solution,
                                                                      tabu_list,
                                                                      params,
                                                                      instance);
        }

        if(move_found){

            const std::vector<int>& profit = instance.getProfitVector();
            std::int64_t local_improvement =  profit[index_to_activate] - (profit[first_index_to_deactivate] + profit[second_index_to_deactivate]);

            // update of the best move
            if(local_improvement > best_improvement){

                best_improvement = local_improvement;

                best_move = TabuMove(TabuMoveType::TwoOne,
                                    {static_cast<size_t>(first_index_to_deactivate),
                                    static_cast<size_t>(second_index_to_deactivate),
                                    static_cast<size_t>(index_to_activate)
                                    }
                                    );

            }
        }

    }






    // ------------------ 1-1 Exchange --------------------

    bool findOneOneExchangeScalarTabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}

        
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a non-tabu move
                if(!tabu_list.contains(TabuMove(TabuMoveType::OneOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i])
                                                         }
                                                        )
                                               )
                        ){

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




    #if HAS_X86 && HAS_AVX2

    bool findOneOneExchangeSIMDX86TabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}

        
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a non-tabu move
                if(!tabu_list.contains(TabuMove(TabuMoveType::OneOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i])
                                                         }
                                                        )
                                               )
                        ){

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

    #endif




    #if HAS_ARM && HAS_NEON

    bool findOneOneExchangeSIMDARMTabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}


        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

                // case of a non-tabu move
                if(!tabu_list.contains(TabuMove(TabuMoveType::OneOne, 
                                                         {static_cast<size_t>(sorted_activated_vars[j]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i])
                                                         }
                                                        )
                                               )
                        ){

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

    #endif





    void findOneOneExchangeTabuSearch(std::int64_t &best_improvement,
                                      TabuMove &best_move,
                                      Solution &solution,
                                      std::vector<float> &scores,
                                      const TabuList &tabu_list,
                                      const Params &params,
                                      const Instance &instance){

        // initialization

        int index_to_deactivate = -1;
        int index_to_activate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        bool move_found = false;

        if(params.use_simd){

            #if HAS_AVX2
                
                move_found = findOneOneExchangeSIMDX86TabuSearch(index_to_deactivate,
                                                                 index_to_activate,
                                                                 sorted_activated_vars,
                                                                 sorted_deactivated_vars,
                                                                 solution,
                                                                 tabu_list,
                                                                 instance);

            #elif HAS_NEON

                move_found = findOneOneExchangeSIMDARMTabuSearch(index_to_deactivate,
                                                                 index_to_activate,
                                                                 sorted_activated_vars,
                                                                 sorted_deactivated_vars,
                                                                 solution,
                                                                 tabu_list,
                                                                 instance);
                

            #endif

        }

        else{

            move_found = findOneOneExchangeScalarTabuSearch(index_to_deactivate,
                                                            index_to_activate,
                                                            sorted_activated_vars,
                                                            sorted_deactivated_vars,
                                                            solution,
                                                            tabu_list,
                                                            instance);
        }

        if(move_found){

            const std::vector<int>& profit = instance.getProfitVector();
            std::int64_t local_improvement =  profit[index_to_activate] - profit[index_to_deactivate];

            // update of the best move
            if(local_improvement > best_improvement){

                best_improvement = local_improvement;

                best_move = TabuMove(TabuMoveType::OneOne,
                                    {static_cast<size_t>(index_to_deactivate),
                                    static_cast<size_t>(index_to_activate)
                                    }
                                    );

            }

        }

    }







    // ------------------ 0-1 Exchange --------------------

    bool findZeroOneExchangeScalarTabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a non-tabu move
            if(!tabu_list.contains(TabuMove(TabuMoveType::ZeroOne, 
                                            {static_cast<size_t>(sorted_deactivated_vars[i])}
                                            )
                                    )
                ){

                // case of a feasible move
                if(!checkConflictZeroOneMove(resource_requirements[sorted_deactivated_vars[i]].getData(),
                                             solution.getConsumedResourcesData())){
                                                    
                    index_to_activate = sorted_deactivated_vars[i];
                    return true;
                }
            }
        }

        return false;
    }




    #if HAS_X86 && HAS_AVX2

    bool findZeroOneExchangeSIMDX86TabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a non-tabu move
            if(!tabu_list.contains(TabuMove(TabuMoveType::ZeroOne, 
                                            {static_cast<size_t>(sorted_deactivated_vars[i])}
                                            )
                                    )
                ){

                // case of a feasible move
                if(!checkConflictZeroOneMoveAVX2(nb_words,
                                                resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                solution.getConsumedResourcesPointerToData())){
                                                        
                    index_to_activate = sorted_deactivated_vars[i];
                    return true;

                }

            }

        }

        return false;
    }

    #endif





    #if HAS_ARM && HAS_NEON

    bool findZeroOneExchangeSIMDARMTabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();

        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            // case of a non-tabu move
            if(!tabu_list.contains(TabuMove(TabuMoveType::ZeroOne, 
                                            {static_cast<size_t>(sorted_deactivated_vars[i])}
                                            )
                                    )
                ){

                // case of a feasible move
                if(!checkConflictZeroOneMoveNEON(nb_words,
                                                resource_requirements[sorted_deactivated_vars[i]].getPointerToData(),
                                                solution.getConsumedResourcesPointerToData())){
                                                        
                    index_to_activate = sorted_deactivated_vars[i];
                    return true;

                }

            }

        }

        return false;
    }

    #endif





    void findZeroOneExchangeTabuSearch(std::int64_t &best_improvement,
                                       TabuMove &best_move,
                                       Solution &solution,
                                       std::vector<float> &scores,
                                       const TabuList &tabu_list,
                                       const Params &params,
                                       const Instance &instance){

        // initialization

        int index_to_activate = -1;
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);
        bool move_found = false;

        if(params.use_simd){

            #if HAS_AVX2
                
                move_found = findZeroOneExchangeSIMDX86TabuSearch(index_to_activate,
                                                                  sorted_deactivated_vars,
                                                                  solution,
                                                                  tabu_list,
                                                                  instance);

            #elif HAS_NEON

                move_found = findZeroOneExchangeSIMDARMTabuSearch(index_to_activate,
                                                                  sorted_deactivated_vars,
                                                                  solution,
                                                                  tabu_list,
                                                                  instance);
                

            #endif

        }

        else{

            move_found = findZeroOneExchangeScalarTabuSearch(index_to_activate,
                                                             sorted_deactivated_vars,
                                                             solution,
                                                             tabu_list,
                                                             instance);
        }

        if(move_found){

            const std::vector<int>& profit = instance.getProfitVector();
            std::int64_t local_improvement =  profit[index_to_activate];

            // update of the best move
            if(local_improvement > best_improvement){

                best_improvement = local_improvement;

                best_move = TabuMove(TabuMoveType::ZeroOne,
                                    {static_cast<size_t>(index_to_activate)}
                                    );

            }
        }

    }









    // ------------------ 1-2 Exchange --------------------

    bool findRestrictedOneTwoExchangeScalarTabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}

        
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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::OneTwo, 
                                                         {static_cast<size_t>(sorted_activated_vars[k]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i]),
                                                          static_cast<size_t>(sorted_deactivated_vars[j])
                                                         }
                                                        )
                                               )
                          ){

                            // case of a feasible move
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

    bool findRestrictedOneTwoExchangeSIMDX86TabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}


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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::OneTwo, 
                                                         {static_cast<size_t>(sorted_activated_vars[k]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i]),
                                                          static_cast<size_t>(sorted_deactivated_vars[j])
                                                         }
                                                        )
                                               )
                          ){

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

    bool findRestrictedOneTwoExchangeSIMDARMTabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 2){return false;}


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

                    // case of a non-tabu move
                    if(!tabu_list.contains(TabuMove(TabuMoveType::OneTwo, 
                                                         {static_cast<size_t>(sorted_activated_vars[k]),
                                                          static_cast<size_t>(sorted_deactivated_vars[i]),
                                                          static_cast<size_t>(sorted_deactivated_vars[j])
                                                         }
                                                        )
                                               )
                          ){

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





    void findRestrictedOneTwoExchangeTabuSearch(std::int64_t &best_improvement,
                                                TabuMove &best_move,
                                                Solution &solution,
                                                std::vector<float> &scores,
                                                const TabuList &tabu_list,
                                                const Params &params,
                                                const Instance &instance){


        // initialization

        int first_index_to_activate = -1;
        int second_index_to_activate = -1;
        int to_deactivate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        bool move_found = false;

        if(params.use_simd){

            #if HAS_AVX2
                
                move_found = findRestrictedOneTwoExchangeSIMDX86TabuSearch(first_index_to_activate,
                                                                           second_index_to_activate,
                                                                           to_deactivate,
                                                                           sorted_activated_vars,
                                                                           sorted_deactivated_vars,
                                                                           solution,
                                                                           tabu_list,
                                                                           params,
                                                                           instance);

            #elif HAS_NEON

                move_found = findRestrictedOneTwoExchangeSIMDARMTabuSearch(first_index_to_activate,
                                                                           second_index_to_activate,
                                                                           to_deactivate,
                                                                           sorted_activated_vars,
                                                                           sorted_deactivated_vars,
                                                                           solution,
                                                                           tabu_list,
                                                                           params,
                                                                           instance);
                

            #endif

        }

        else{

            move_found = findRestrictedOneTwoExchangeScalarTabuSearch(first_index_to_activate,
                                                                      second_index_to_activate,
                                                                      to_deactivate,
                                                                      sorted_activated_vars,
                                                                      sorted_deactivated_vars,
                                                                      solution,
                                                                      tabu_list,
                                                                      params,
                                                                      instance);
        }

        if(move_found){

            const std::vector<int>& profit = instance.getProfitVector();
            std::int64_t local_improvement = (profit[first_index_to_activate] + profit[second_index_to_activate]) - profit[to_deactivate];

            // update of the best move
            if(local_improvement > best_improvement){

                best_improvement = local_improvement;

                best_move = TabuMove(TabuMoveType::OneTwo,
                                    {static_cast<size_t>(to_deactivate),
                                    static_cast<size_t>(first_index_to_activate),
                                    static_cast<size_t>(second_index_to_activate)
                                    }
                                    );

            }
        }

    }









    TabuMove findBestMove(TabuList &tabu_list,
                          Solution &solution,
                          const Params &params,
                          const Instance &instance){

        // initialization
        std::vector<float> scores = computeVariableScores(instance);
        TabuMove best_move;
        std::int64_t best_improvement = std::numeric_limits<std::int64_t>::min();


        // seaching in the 2-1 neighborhood
        findRestrictedTwoOneExchangeTabuSearch(best_improvement,
                                               best_move,
                                               solution,
                                               scores,
                                               tabu_list,
                                               params,
                                               instance);


        // seaching in the 1-1 neighborhood
        findOneOneExchangeTabuSearch(best_improvement,
                                     best_move,
                                     solution,
                                     scores,
                                     tabu_list,
                                     params,
                                     instance);


        // seaching in the 0-1 neighborhood
        findZeroOneExchangeTabuSearch(best_improvement,
                                      best_move,
                                      solution,
                                      scores,
                                      tabu_list,
                                      params,
                                      instance);


        // seaching in the 1-2 neighborhood
        findRestrictedOneTwoExchangeTabuSearch(best_improvement,
                                               best_move,
                                               solution,
                                               scores,
                                               tabu_list,
                                               params,
                                               instance);


        return best_move;

    }





    void updateSolutionAfterLocalSearch(Solution &solution, 
                                        const TabuMove &best_move, 
                                        const Instance &instance){


        // 0-1 exchange
        if(best_move.getMoveType() == TabuMoveType::ZeroOne){

            size_t index_to_activate = best_move.getIndexes()[0];

            // activation
            solution.activateVar(index_to_activate, instance);
        }


        // 1-1 exchange
        if(best_move.getMoveType() == TabuMoveType::OneOne){

            size_t index_to_deactivate = best_move.getIndexes()[0];
            size_t index_to_activate = best_move.getIndexes()[1];

            // deactivation
            solution.deactivateVar(index_to_deactivate, instance);

            // activation
            solution.activateVar(index_to_activate, instance);
        }


        // 1-2 exchange
        if(best_move.getMoveType() == TabuMoveType::OneTwo){

            size_t to_deactivate = best_move.getIndexes()[0];
            size_t first_index_to_activate = best_move.getIndexes()[1];
            size_t second_index_to_activate = best_move.getIndexes()[2];

            // deactivation
            solution.deactivateVar(to_deactivate, instance);

            // activation
            solution.activateVar(first_index_to_activate, instance);
            solution.activateVar(second_index_to_activate, instance);
        }


        // 2-1 exchange
        if(best_move.getMoveType() == TabuMoveType::OneTwo){

            size_t first_index_to_deactivate = best_move.getIndexes()[0];
            size_t second_index_to_deactivate = best_move.getIndexes()[1];
            size_t to_activate = best_move.getIndexes()[2];

            // deactivation
            solution.deactivateVar(first_index_to_deactivate, instance);
            solution.deactivateVar(second_index_to_deactivate, instance);

            // activation
            solution.activateVar(to_activate, instance);
        }

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}

        else{solution.setStatus(Status::INFEASIBLE);}


    }


}