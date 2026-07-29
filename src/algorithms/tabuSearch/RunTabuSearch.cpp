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
 * @file RunTabuSearch.cpp
 * @author Josué Tambwe
 * @date 27 July 2026
 */


#include "algorithms/tabuSearch/RunTabuSearch.hpp"


namespace spp{


    void runTabuSearch(const Params &params){

        printHeader();

        // building the instance
        const Instance instance(params);

        printHeaderTabuSearch(params, instance);
        
        Timer timer = Timer();
        //Logger log;
        timer.start();

        // construction
        Solution current_solution = randomizedConstruction(params.alpha, instance);

        // initialization of the best solution
        Solution best_solution = current_solution;
        std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

        // initialization of the tabu list
        TabuList tabu_list(params.tabu_tenure);

        size_t current_iteration = 0;

        while(!stoppingCriteriaTabuSearch(timer.getElapsedTime(), current_iteration, params)){

            current_iteration += 1;

            // finding the best non-tabu move
            TabuMove best_move = findBestMove(tabu_list,
                                              current_solution,
                                              params,
                                              instance);

            // performing the exchange
            updateSolutionAfterLocalSearch(current_solution, 
                                           best_move, 
                                           instance);

            // adding the best non-tabu move to the tabu list
            updateTabuList(best_move, tabu_list);

            // updating the best solution
            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            if(current_solution_objective_value > best_solution_objective_value){

                best_solution_objective_value = current_solution_objective_value;
                best_solution = current_solution;
            }

            std::cout << "\n  iteration : " << current_iteration << "\n ";

            tabu_list.print();

            std::cout << "\n";



            

        }


        best_solution.print(instance);
        std::cout << " status : " << best_solution.getStatus() << "  time : " << timer.getElapsedTime() << "(s) \n\n";

    
    }
}