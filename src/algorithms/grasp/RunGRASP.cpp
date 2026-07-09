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
 * @file RunGRASP.cpp
 * @author Josué Tambwe
 * @date 8 July 2026
 */

#include "algorithms/grasp/RunGRASP.hpp"


namespace spp{



    bool stoppingCriteriaGRASP(double current_time, 
                               size_t current_iteration, 
                               const Params &params){

        if(params.use_time_limit && !params.use_max_iterations){return current_time >= params.time_limit;}

        if(!params.use_time_limit && params.use_max_iterations){return current_iteration >= params.nb_max_iterations;}

        else{
            return (current_iteration >= params.nb_max_iterations) || (current_time >= params.time_limit);
        }

    }




    void runReactiveGRASP(const Params &params){

        printHeader();

        // building the instance
        const Instance instance(params);

        printHeaderGRASP(params, instance);

        Timer timer = Timer();
        Logger log;
        timer.start();

        // initialization of the elite solution
        Solution elite_solution = initializeEliteSolution(params, instance);
        std::int64_t elite_objective_value = elite_solution.getObjectiveValue(instance);

        // initialization of alpha values probailities (1/10 for each alpha value)
        std::array<float, 10> alpha_probabilities;
        alpha_probabilities.fill(0.1f);

        printGRASPInitialElite(timer.getElapsedTime(), 
                               1,
                               elite_objective_value,
                               alpha_values,
                               alpha_probabilities);


        size_t current_iteration = 0;

        while(!stoppingCriteriaGRASP(timer.getElapsedTime(), current_iteration, params)){

            Solution best_solution = runMultiThreadIterations(alpha_probabilities,
                                                              elite_solution,
                                                              params,
                                                              instance);

            std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

            current_iteration += 1;

            printGRASPIteration(timer.getElapsedTime(), 
                                (current_iteration * params.update_interval),
                                elite_objective_value,
                                best_solution_objective_value,
                                alpha_values,
                                alpha_probabilities);

            // update of the elite solution
            if(best_solution_objective_value > elite_objective_value){

                elite_solution = best_solution;
                elite_objective_value = best_solution_objective_value;
            }

        }

        log.info("Reactive GRASP algorithm completed. Final best known solution :");


        elite_solution.print(instance);
        std::cout << " time : "  << timer.getElapsedTime() << " (s)" 
                  << " status : " << elite_solution.getStatus() << "\n\n";


        
    }


}