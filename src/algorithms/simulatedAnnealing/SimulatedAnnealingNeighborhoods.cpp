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
 * @file SimulatedAnnealingNeighborhoods.cpp
 * @author Josué Tambwe
 * @date 1 August 2026
 */

#include "algorithms/simulatedAnnealing/SimulatedAnnealingNeighborhoods.hpp"


namespace spp{

    // ------------------- 2-1 Exchange ----------------------

    bool findRestrictedTwoOneExchangeScalarSimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
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

        return false;
    }



    #if HAS_X86 && HAS_AVX2

    bool findRestrictedTwoOneExchangeSIMDX86SimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
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

        return false;
    }

    #endif





    #if HAS_ARM && HAS_NEON

    bool findRestrictedTwoOneExchangeSIMDARMSimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
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

        return false;
    }

    #endif





    bool findRestrictedTwoOneExchangeSimulatedAnnealing(Solution &solution,
                                                        std::vector<float> &scores,
                                                        const Params &params,
                                                        const Instance &instance){

        // initialization

        int first_index_to_deactivate = -1;
        int second_index_to_deactivate = -1;
        int index_to_activate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        if(params.use_simd){

            #if HAS_AVX2
                
                return  findRestrictedTwoOneExchangeSIMDX86SimulatedAnnealing(first_index_to_deactivate,
                                                                            second_index_to_deactivate,
                                                                            index_to_activate,
                                                                            sorted_activated_vars,
                                                                            sorted_deactivated_vars,
                                                                            solution,
                                                                            params,
                                                                            instance);

            #elif HAS_NEON

                return  findRestrictedTwoOneExchangeSIMDARMSimulatedAnnealing(first_index_to_deactivate,
                                                                            second_index_to_deactivate,
                                                                            index_to_activate,
                                                                            sorted_activated_vars,
                                                                            sorted_deactivated_vars,
                                                                            solution,
                                                                            params,
                                                                            instance);
                

            #endif

        }

        else{

            return  findRestrictedTwoOneExchangeScalarSimulatedAnnealing(first_index_to_deactivate,
                                                                        second_index_to_deactivate,
                                                                        index_to_activate,
                                                                        sorted_activated_vars,
                                                                        sorted_deactivated_vars,
                                                                        solution,
                                                                        params,
                                                                        instance);
        }

    }






    // ------------------ 1-1 Exchange --------------------

    bool findOneOneExchangeScalarSimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}

        
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

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

        return false;
    }




    #if HAS_X86 && HAS_AVX2

    bool findOneOneExchangeSIMDX86SimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}

        
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

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

        return false;
    }

    #endif




    #if HAS_ARM && HAS_NEON

    bool findOneOneExchangeSIMDARMSimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_activated_vars.size() < 1 || sorted_deactivated_vars.size() < 1){return false;}


        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();
        const size_t nb_words = solution.getNbConsumedResourceWords();


        for(size_t i = 0; i < sorted_deactivated_vars.size(); i++){

            for(size_t j = 0; j < sorted_activated_vars.size(); j++){

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

        return false;
    }

    #endif





    bool findOneOneExchangeSimulatedAnnealing(Solution &solution,
                                            std::vector<float> &scores,
                                            const Params &params,
                                            const Instance &instance){

        // initialization

        int index_to_deactivate = -1;
        int index_to_activate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        if(params.use_simd){

            #if HAS_AVX2
                
                return  findOneOneExchangeSIMDX86SimulatedAnnealing(index_to_deactivate,
                                                                    index_to_activate,
                                                                    sorted_activated_vars,
                                                                    sorted_deactivated_vars,
                                                                    solution,
                                                                    instance);

            #elif HAS_NEON

                return  findOneOneExchangeSIMDARMSimulatedAnnealing(index_to_deactivate,
                                                                    index_to_activate,
                                                                    sorted_activated_vars,
                                                                    sorted_deactivated_vars,
                                                                    solution,
                                                                    instance);
                

            #endif

        }

        else{

            return  findOneOneExchangeScalarSimulatedAnnealing(index_to_deactivate,
                                                               index_to_activate,
                                                               sorted_activated_vars,
                                                               sorted_deactivated_vars,
                                                               solution,
                                                               instance);
        }

    }









    // ------------------ 0-1 Exchange --------------------

    bool findZeroOneExchangeScalarSimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

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




    #if HAS_X86 && HAS_AVX2

    bool findZeroOneExchangeSIMDX86SimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

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

    #endif





    #if HAS_ARM && HAS_NEON

    bool findZeroOneExchangeSIMDARMSimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance){

        // preprocessing
        if(sorted_deactivated_vars.size() < 1){return false;}

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

    #endif





    bool findZeroOneExchangeSimulatedAnnealing(Solution &solution,
                                            std::vector<float> &scores,
                                            const Params &params,
                                            const Instance &instance){

        // initialization

        int index_to_activate = -1;
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        if(params.use_simd){

            #if HAS_AVX2
                
                return findZeroOneExchangeSIMDX86SimulatedAnnealing(index_to_activate,
                                                                    sorted_deactivated_vars,
                                                                    solution,
                                                                    instance);

            #elif HAS_NEON

                return findZeroOneExchangeSIMDARMSimulatedAnnealing(index_to_activate,
                                                                    sorted_deactivated_vars,
                                                                    solution,
                                                                    instance);
                

            #endif

        }

        else{

            return findZeroOneExchangeScalarSimulatedAnnealing(index_to_activate,
                                                               sorted_deactivated_vars,
                                                               solution,
                                                               instance);
        }

    }









    // ------------------ 1-2 Exchange --------------------

    bool findRestrictedOneTwoExchangeScalarSimulatedAnnealing(int &first_index_to_activate,
                                                            int &second_index_to_activate,
                                                            int &to_deactivate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
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

        return false;
    }





    #if HAS_X86 && HAS_AVX2

    bool findRestrictedOneTwoExchangeSIMDX86SimulatedAnnealing(int &first_index_to_activate,
                                                            int &second_index_to_activate,
                                                            int &to_deactivate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
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

                    // case of a feasible move
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

        return false;
    }

    #endif





    #if HAS_ARM && HAS_NEON

    bool findRestrictedOneTwoExchangeSIMDARMSimulatedAnnealing(int &first_index_to_activate,
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

                    // case of a feasible move
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

        return false;
    }

    #endif





    bool findRestrictedOneTwoExchangeSimulatedAnnealing(Solution &solution,
                                                        std::vector<float> &scores,
                                                        const Params &params,
                                                        const Instance &instance){


        // initialization

        int first_index_to_activate = -1;
        int second_index_to_activate = -1;
        int to_deactivate = -1;

        std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
        std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

        if(params.use_simd){

            #if HAS_AVX2
                
                return  findRestrictedOneTwoExchangeSIMDX86SimulatedAnnealing(first_index_to_activate,
                                                                            second_index_to_activate,
                                                                            to_deactivate,
                                                                            sorted_activated_vars,
                                                                            sorted_deactivated_vars,
                                                                            solution,
                                                                            params,
                                                                            instance);

            #elif HAS_NEON

                return  findRestrictedOneTwoExchangeSIMDARMSimulatedAnnealing(first_index_to_activate,
                                                                            second_index_to_activate,
                                                                            to_deactivate,
                                                                            sorted_activated_vars,
                                                                            sorted_deactivated_vars,
                                                                            solution,
                                                                            params,
                                                                            instance);
                

            #endif

        }

        else{

            return  findRestrictedOneTwoExchangeScalarSimulatedAnnealing(first_index_to_activate,
                                                                        second_index_to_activate,
                                                                        to_deactivate,
                                                                        sorted_activated_vars,
                                                                        sorted_deactivated_vars,
                                                                        solution,
                                                                        params,
                                                                        instance);
        }

    
    }




}