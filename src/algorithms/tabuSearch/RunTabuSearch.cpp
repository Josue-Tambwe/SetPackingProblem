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


    double computeRelativeImprovement(std::int64_t new_objective_value, 
                                      std::int64_t old_objective_value){

        return std::max(0.0,
                            ((new_objective_value - old_objective_value) * 100.0 / 
                             (old_objective_value + 1e-6)
                            )
                        );
    }





    double computeRelativeDegradation(std::int64_t new_objective_value, 
                                      std::int64_t old_objective_value){

        return std::max(0.0,
                            ((old_objective_value - new_objective_value) * 100.0 / 
                             (old_objective_value + 1e-6)
                            )
                        );
    }






    void runTabuSearch(const Params &params){

        printHeader();

        // building the instance
        const Instance instance(params);

        printHeaderTabuSearch(params, instance);
        
        Timer timer = Timer();
        Logger log;
        timer.start();

        // construction
        Solution current_solution = randomizedConstruction(params.alpha, instance);
        double construction_time = timer.getElapsedTime();
        std::int64_t construction_objective_value = current_solution.getObjectiveValue(instance);

        // initialization of the best solution
        Solution best_solution = current_solution;
        std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

        // initialization of the tabu list
        TabuList tabu_list(params.tabu_tenure);


        size_t current_iteration = 0;

        double relative_cumulative_improvement = 0.0;
        double relative_cumulative_degradation = 0.0;
        size_t iteration_count = 0;

        while(!stoppingCriteriaTabuSearch(timer.getElapsedTime(), current_iteration, params)){

            current_iteration += 1;
            iteration_count += 1;

            std::int64_t old_current_solution_objective_value = current_solution.getObjectiveValue(instance);

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

            
            std::int64_t new_current_solution_objective_value = current_solution.getObjectiveValue(instance);

            // computation of the cumulative improvement gap of the incumbent (current) solution
            relative_cumulative_improvement += computeRelativeImprovement(new_current_solution_objective_value, 
                                                                          old_current_solution_objective_value);


            // computation of the cumulative degradation gap of the incumbent (current) solution
            relative_cumulative_degradation += computeRelativeDegradation(new_current_solution_objective_value, 
                                                                          old_current_solution_objective_value);

            if(current_iteration == 1 || (current_iteration % 50) == 0){

                printTabuSearchIterations(timer.getElapsedTime(), 
                                          current_iteration,
                                          iteration_count,
                                          new_current_solution_objective_value,
                                          best_solution_objective_value,
                                          relative_cumulative_improvement,
                                          relative_cumulative_degradation);

                iteration_count = 0;
                relative_cumulative_improvement = 0.0;
                relative_cumulative_degradation = 0.0;

            }


            // updating the best solution
            if(new_current_solution_objective_value > best_solution_objective_value){

                best_solution_objective_value = new_current_solution_objective_value;
                best_solution = current_solution;
            }


            // periodic restart: rebuild a new initial solution to diversify the search
            if((current_iteration % params.restart_interval) == 0){

                current_solution = randomizedConstruction(params.alpha, instance);

                // reset of the tabu list
                tabu_list = TabuList(params.tabu_tenure);
            }

        }

        log.info("Tabu Search algorithm completed. Final best known solution :");

        best_solution.print(instance);

        printSummaryTabuSearch(construction_time, 
                              timer.getElapsedTime(),
                              current_iteration,
                              construction_objective_value,
                              best_solution.getObjectiveValue(instance),
                              best_solution.getStatus());

    }
}