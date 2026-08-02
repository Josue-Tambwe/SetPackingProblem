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
 * @file RunSimulatedAnnealing.cpp
 * @author Josué Tambwe
 * @date 2 August 2026
 */

#include "algorithms/simulatedAnnealing/RunSimulatedAnnealing.hpp"

 namespace spp{



    double computeRelativeImprovementSimulatedAnnealing(std::int64_t new_objective_value, 
                                                        std::int64_t old_objective_value){

        return std::max(0.0,
                            ((new_objective_value - old_objective_value) * 100.0 / 
                             (old_objective_value + 1e-6)
                            )
                        );
    }





    double computeRelativeDegradationSimulatedAnnealing(std::int64_t new_objective_value, 
                                                        std::int64_t old_objective_value){

        return std::max(0.0,
                            ((old_objective_value - new_objective_value) * 100.0 / 
                             (old_objective_value + 1e-6)
                            )
                        );
    }





    
    void runSimulatedAnnealing(const Params &params){

        printHeader();

        // building the instance
        const Instance instance(params);

        //printHeaderTabuSearch(params, instance);
        
        Timer timer = Timer();
        Logger log;
        timer.start();

        // construction
        Solution current_solution = randomizedConstruction(params.alpha, instance);
        //double construction_time = timer.getElapsedTime();
        std::int64_t construction_objective_value = current_solution.getObjectiveValue(instance);

        // initialization of the best solution
        Solution best_solution = current_solution;
        std::int64_t best_solution_objective_value = construction_objective_value;


        size_t current_iteration = 0;
        size_t improvement_acceptance = 0;
        size_t worsening_acceptance = 0;
        size_t rejection = 0;
        double current_temperature = params.initial_temperature;

        double relative_cumulative_improvement = 0.0;
        double relative_cumulative_degradation = 0.0;

        while(!stoppingCriteriaSimulatedAnnealing(timer.getElapsedTime(), 
                                                  current_iteration, 
                                                  current_temperature, 
                                                  params)){

            current_iteration += 1;

            std::int64_t old_current_solution_objective_value = current_solution.getObjectiveValue(instance);

            // temporary solution
            Solution temporary_solution = current_solution;

            bool move_found = findMoveRandomly(temporary_solution,
                                               params,
                                               instance);

            std::int64_t new_current_solution_objective_value = temporary_solution.getObjectiveValue(instance);


            

            if(move_found){

                // computation of the cumulative improvement gap of the incumbent (current) solution
                relative_cumulative_improvement += computeRelativeImprovementSimulatedAnnealing(new_current_solution_objective_value, 
                                                                                                old_current_solution_objective_value);


                // computation of the cumulative degradation gap of the incumbent (current) solution
                relative_cumulative_degradation += computeRelativeDegradationSimulatedAnnealing(new_current_solution_objective_value, 
                                                                                                old_current_solution_objective_value);


                std::int64_t degradation = old_current_solution_objective_value - new_current_solution_objective_value;

                // case of the improvement of the current solution
                if(degradation <= 0){

                    improvement_acceptance += 1;

                    current_solution = temporary_solution;

                    // case of the improvement of the best solution
                    if(new_current_solution_objective_value > best_solution_objective_value){

                        best_solution = temporary_solution;
                        best_solution_objective_value = new_current_solution_objective_value;
                    }

                }

                else{

                    bool acceptance = acceptanceCriterion(static_cast<double>(degradation), 
                                                          current_temperature);

                    // case of accepting current solution degradation
                    if(acceptance){

                        worsening_acceptance += 1;

                        current_solution = temporary_solution;
                    }

                    else{

                        rejection += 1;
                    }
                }

            }


            // update of the temperature (geometric cooling)
            if((current_iteration % params.cooling_interval) == 0){

                current_temperature = current_temperature * params.cooling_factor;

            }

        }

        log.info("Simulated Annealing algorithm completed. Final best known solution :");

        best_solution.print(instance);
    }


 }
