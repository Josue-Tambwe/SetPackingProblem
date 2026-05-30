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

            improvement = findTwoOneExchange(to_deactivate,
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


 }
 