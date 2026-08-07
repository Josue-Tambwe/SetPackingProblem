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
 * @file LowlevelMetaheuristics.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/LowLevelMetaheuristics.hpp"

namespace spp{


    // ------------- Variable Neighborhood Descent -----------------

    void variableNeighborhoodDescentImprovement(Solution &solution,
                                                const Params &params,
                                                const Instance &instance){

        bool intensification = true;

        restrictedVariableNeighborhoodDescent(intensification,
                                              params, 
                                              solution, 
                                              instance);
    }




    // ------------- Tabu Search -----------------

    void tabuSearchImprovement(float alpha,
                               Solution &solution,
                               const Params &params,
                               const Instance &instance){

        Timer timer = Timer();
        timer.start();

       // initialization of the local current solution
        Solution local_current_solution = solution;
        std::int64_t local_current_solution_objective_value = local_current_solution.getObjectiveValue(instance);

        // initialization of the local best solution
        Solution local_best_solution = local_current_solution;
        std::int64_t local_best_solution_objective_value = local_current_solution_objective_value;

        // initialization of the tabu list
        TabuList tabu_list(params.tabu_tenure);

        size_t iteration = 0;

        while(timer.getElapsedTime() < params.improvement_time){

            iteration += 1;

            // finding the best non-tabu move
            TabuMove best_move = findBestMove(tabu_list,
                                              local_current_solution,
                                              params,
                                              instance);

            // performing the exchange
            updateSolutionAfterLocalSearch(local_current_solution, 
                                           best_move, 
                                           instance);

            // adding the best non-tabu move to the tabu list
            updateTabuList(best_move, tabu_list);

            
            local_current_solution_objective_value = local_current_solution.getObjectiveValue(instance);

            // updating the best solution
            if(local_current_solution_objective_value > local_best_solution_objective_value){

                local_best_solution_objective_value = local_current_solution_objective_value;
                local_best_solution = local_current_solution;
            }


            // periodic restart: rebuild a new initial solution to diversify the search
            if((iteration % params.restart_interval) == 0){

                local_current_solution = randomizedConstruction(alpha, instance);

                // reset of the tabu list
                tabu_list = TabuList(params.tabu_tenure);
            }

        }

        // final solution
        solution = local_best_solution;

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}

        else{solution.setStatus(Status::INFEASIBLE);}
    }





    // ------------- Simulated Annealing -----------------

    void simulatedAnnealingImprovement(float alpha,
                                       Solution &solution,
                                       const Params &params,
                                       const Instance &instance){

        Timer timer = Timer();
        timer.start();

        // initialization of the local current solution
        Solution local_current_solution = solution;

        // initialization of the local best solution
        Solution local_best_solution = local_current_solution;
        std::int64_t local_best_solution_objective_value = local_current_solution.getObjectiveValue(instance);

        size_t iteration = 0;
        double current_temperature = params.initial_temperature;

        while(timer.getElapsedTime() < params.improvement_time){

            iteration += 1;

            std::int64_t old_local_current_solution_objective_value = local_current_solution.getObjectiveValue(instance);

            // temporary solution
            Solution temporary_solution = local_current_solution;

            bool move_found = findMoveRandomly(temporary_solution,
                                               params,
                                               instance);

            std::int64_t new_local_current_solution_objective_value = temporary_solution.getObjectiveValue(instance);

            if(move_found){

                std::int64_t degradation = old_local_current_solution_objective_value - new_local_current_solution_objective_value;

                // case of the improvement of the current solution
                if(degradation <= 0){

                    local_current_solution = temporary_solution;

                    // case of the improvement of the best solution
                    if(new_local_current_solution_objective_value > local_best_solution_objective_value){

                        local_best_solution = temporary_solution;
                        local_best_solution_objective_value = new_local_current_solution_objective_value;
                    }

                }

                else{

                    bool acceptance = acceptanceCriterion(static_cast<double>(degradation), 
                                                          current_temperature);

                    // case of accepting current solution degradation
                    if(acceptance){local_current_solution = temporary_solution;}

                }

            }
            
            // update of the temperature (geometric cooling)
            if((iteration % params.cooling_interval) == 0){

                current_temperature = current_temperature * params.cooling_factor;
            }

            // periodic restart: rebuild a new initial solution to diversify the search
            if((iteration % params.restart_interval) == 0){

                local_current_solution = randomizedConstruction(alpha, instance);

            }

        }

        // final solution
        solution = local_best_solution;

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}

        else{solution.setStatus(Status::INFEASIBLE);}
    }


}
