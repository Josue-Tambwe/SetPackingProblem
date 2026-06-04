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
 * @file VariableNeighborhoodDescent.cpp
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"

 namespace spp{

    // ------------------------ 2-1 Exchange -----------------------------

    void twoOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance){
                            

        bool improvement = true;
        int first_index_to_deactivate = -1;
        int second_index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findTwoOneExchange(first_index_to_deactivate,
                                             second_index_to_deactivate,
                                             index_to_activate,
                                             sorted_activated_vars,
                                             sorted_deactivated_vars,
                                             solution,
                                             instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(first_index_to_deactivate, instance);
                solution.deactivateVar(second_index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }




    // ------------------------ 1-1 Exchange -----------------------------


    void oneOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance){

        bool improvement = true;
        int index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneOneExchange(index_to_deactivate,
                                             index_to_activate,
                                             sorted_activated_vars,
                                             sorted_deactivated_vars,
                                             solution,
                                             instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }




    // ------------------------ 0-1 Exchange -----------------------------

    void zeroOneNeighborhood(std::vector<float> &scores,
                             Solution &solution,
                             const Instance &instance){

        bool improvement = true;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findZeroOneExchange(index_to_activate,
                                             sorted_deactivated_vars,
                                             solution,
                                             instance);

    
            if(improvement){solution.activateVar(index_to_activate, instance);}
        }
        while(improvement);

    }


    // ------------------------ 1-2 Exchange -----------------------------

    void oneTwoNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance){

        bool improvement = true;
        int to_deactivate = -1;
        int first_index_to_activate = -1;
        int second_index_to_activate = -1;
        

        do{

            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneTwoExchange(to_deactivate,
                                             first_index_to_activate,
                                             second_index_to_activate,
                                             sorted_activated_vars,
                                             sorted_deactivated_vars,
                                             solution,
                                             instance);


            // update of the solution 
            if(improvement){
                // deactivation
                solution.deactivateVar(to_deactivate, instance);

                // activation
                solution.activateVar(first_index_to_activate, instance);
                solution.activateVar(second_index_to_activate, instance);
            }


        }
        while(improvement);
                                
    }




    #if HAS_AVX2

    void twoOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){
                            

        bool improvement = true;
        int first_index_to_deactivate = -1;
        int second_index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findTwoOneExchangeSIMDX86(first_index_to_deactivate,
                                                    second_index_to_deactivate,
                                                    index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(first_index_to_deactivate, instance);
                solution.deactivateVar(second_index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }





    void oneOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){

        bool improvement = true;
        int index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneOneExchangeSIMDX86(index_to_deactivate,
                                                    index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }




    void zeroOneNeighborhoodSIMDX86(std::vector<float> &scores,
                             Solution &solution,
                             const Instance &instance){

        bool improvement = true;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);
            improvement = findZeroOneExchangeSIMDX86(index_to_activate,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

    
            if(improvement){solution.activateVar(index_to_activate, instance);}
        }
        while(improvement);

    }



    void oneTwoNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){

        bool improvement = true;
        int to_deactivate = -1;
        int first_index_to_activate = -1;
        int second_index_to_activate = -1;
        

        do{

            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneTwoExchangeSIMDX86(to_deactivate,
                                                    first_index_to_activate,
                                                    second_index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);


            // update of the solution 
            if(improvement){
                // deactivation
                solution.deactivateVar(to_deactivate, instance);

                // activation
                solution.activateVar(first_index_to_activate, instance);
                solution.activateVar(second_index_to_activate, instance);
            }


        }
        while(improvement);
                                
    }
    #endif


    

    #if HAS_NEON

    void twoOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){
                            

        bool improvement = true;
        int first_index_to_deactivate = -1;
        int second_index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findTwoOneExchangeSIMDARM(first_index_to_deactivate,
                                                    second_index_to_deactivate,
                                                    index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(first_index_to_deactivate, instance);
                solution.deactivateVar(second_index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }



    void oneOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){

        bool improvement = true;
        int index_to_deactivate = -1;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneOneExchangeSIMDARM(index_to_deactivate,
                                                    index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

            // update of the solution 
            if(improvement){

                // deactivation
                solution.deactivateVar(index_to_deactivate, instance);

                // activation
                solution.activateVar(index_to_activate, instance);
            }


        }
        while(improvement);

    }




    void zeroOneNeighborhoodSIMDARM(std::vector<float> &scores,
                             Solution &solution,
                             const Instance &instance){

        bool improvement = true;
        int index_to_activate = -1;

        do{
            
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);
            improvement = findZeroOneExchangeSIMDARM(index_to_activate,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);

    
            if(improvement){solution.activateVar(index_to_activate, instance);}
        }
        while(improvement);

    }



    void oneTwoNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance){

        bool improvement = true;
        int to_deactivate = -1;
        int first_index_to_activate = -1;
        int second_index_to_activate = -1;
        

        do{

            std::vector<int> sorted_activated_vars = sortNonZeroVars(solution, scores);
            std::vector<int> sorted_deactivated_vars = sortZeroVars(solution, scores);

            improvement = findOneTwoExchangeSIMDARM(to_deactivate,
                                                    first_index_to_activate,
                                                    second_index_to_activate,
                                                    sorted_activated_vars,
                                                    sorted_deactivated_vars,
                                                    solution,
                                                    instance);


            // update of the solution 
            if(improvement){
                // deactivation
                solution.deactivateVar(to_deactivate, instance);

                // activation
                solution.activateVar(first_index_to_activate, instance);
                solution.activateVar(second_index_to_activate, instance);
            }


        }
        while(improvement);
                                
    }

    #endif




    void variableNeighborhoodDescent(Params &params, 
                                     Solution &solution, 
                                     const Instance &instance){

        std::vector<float> scores = computeVariableScores(instance);

        if(params.use_simd){

            #if HAS_AVX2

                oneTwoNeighborhoodSIMDX86(scores, solution, instance);
                oneOneNeighborhoodSIMDX86(scores, solution, instance);
                twoOneNeighborhoodSIMDX86(scores, solution, instance);
                zeroOneNeighborhoodSIMDX86(scores, solution, instance);

            #elif HAS_NEON

                oneTwoNeighborhoodSIMDARM(scores, solution, instance);
                oneOneNeighborhoodSIMDARM(scores, solution, instance);
                twoOneNeighborhoodSIMDARM(scores, solution, instance);
                zeroOneNeighborhoodSIMDARM(scores, solution, instance);
                

            #endif
        }

        else{

            oneTwoNeighborhood(scores, solution, instance);
            oneOneNeighborhood(scores, solution, instance);
            twoOneNeighborhood(scores, solution, instance);
            zeroOneNeighborhood(scores, solution, instance);

        }

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}
        else{solution.setStatus(Status::INFEASIBLE);}
       

    }


 }
 