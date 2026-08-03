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
 * @file SimulatedAnnealingOutput.cpp
 * @author Josué Tambwe
 * @date 3 August 2026
 */

 
#include "output/SimulatedAnnealingOutput.hpp"


namespace spp{


    void printHeaderSimulatedAnnealing(const Params &params, 
                                       const Instance &instance){

                const InstanceStatistics& stats = instance.getStatistics();

                // line 1
                std::cout << YELLOW;
                std::cout << "  " <<  "Instance statistics" << std::string(27, ' ') 
                        << "Algorithm" << std::string(8, ' ')
                        << "Hardware" << std::string(36, ' ')
                        << "Settings"
                        << RESET << "\n";

                // line 2
                std::cout << "  "  << std::string(19, '-') << std::string(27, ' ') 
                        << std::string(9, '-') << std::string(8, ' ')
                        << std::string(8, '-') << std::string(36, ' ')
                        << std::string(8, '-')
                        << "\n";

                // line 3
                std::cout << "  " << std::setw(22) << std::left << "number of variables" 
                        <<  std::setw(2) << std::right << " : " 
                        <<  std::setw(14) <<  std::left << stats.nb_vars << std::setw(2) << std::left << " "

                        << MAGENTA << "Simulated Annealing" << RESET << std::setw(3) << std::left << " "

                        << std::setw(18) << std::left << "CPU ISA" <<  std::setw(2) << std::right << " : ";
                        if(HAS_X86){std::cout << std::setw(15) << std::left << "x86";} 
                        else{std::cout << std::setw(15) << std::left << "ARM";}

                        std::cout << std::setw(8) << std::left << " "
                                << std::setw(19) << std::left << "threads used" <<  std::setw(2) << std::right << " : "
                                << 1 << "\n"; // always 1 thread

                //line 4
                std::cout << "  " << std::setw(22) << std::left << "number of constraints"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.nb_constraints << std::setw(22) << std::left << " ";

                        std::string simd_value;
                        if(HAS_X86){
                                if(HAS_AVX2){simd_value = "AVX2 (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "AVX2 (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }

                        else{
                                if(HAS_NEON){simd_value = "NEON (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "NEON (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }
                        std::cout << std::setw(18) << std::left  << "SIMD extension" << std::setw(3)  << std::right << " : "
                        << std::setw(30) << std::left  << simd_value << std::setw(2) << std::left << " "

                        << std::setw(19) << std::left << "SIMD vectorization" <<  std::setw(2) << std::right << " : ";
                        if(params.use_simd){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                        std::cout << "\n";

                // line 5
                std::cout << std::setw(65) << std::left << " "
                        << std::setw(15) << std::left << "CPU physical cores" << std::setw(3)  << std::right << " : "
                        <<  std::setw(4) <<  std::left << NUMBER_PHYSICAL_CORES << std::setw(19) << std::left << " "

                        << std::setw(19) << std::left << "intensification" <<  std::setw(2) << std::right << " : ";
                        if(params.use_intensification){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                        std::cout << "\n";

                // line 6
                std::cout << "  " << std::setw(22) << std::left << "profit range"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left 
                        << (std::to_string(stats.min_profit) + " - " + std::to_string(stats.max_profit))
                        << "\n";

                                

                // line 7
                std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row
                        << std::setw(66) << std::left << " "

                        << std::setw(19) << std::left << "alpha" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.alpha
                        <<  "\n";

                // line 8
                std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                        << std::setw(66) << std::left << " "

                        << std::setw(19) << std::left << "pruning rate" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.pruning_rate
                        <<  "\n";



                // line 9
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "restart interval" <<  std::setw(2) << std::right << " : "
                        << params.restart_interval          
                        <<  "\n\n";

                // line 10
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "initial temperature" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.initial_temperature         
                        <<  "\n";

                // line 11
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "final temperature" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.final_temperature          
                        <<  "\n";

                // line 12
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "cooling interval" <<  std::setw(2) << std::right << " : "
                        << params.cooling_interval          
                        <<  "\n";

                // line 13
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "cooling factor" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.cooling_factor         
                        <<  "\n";


                // line 14
                if(params.use_max_iterations){

                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "maximum iterations" <<  std::setw(2) << std::right << " : "
                        << params.nb_max_iterations << "\n";
                }


                // line 15
                if(params.use_time_limit){

                    std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.time_limit << "\n";  
                
                }  

                std::cout << "\n\n";
        }





        std::string formatImprovementSimulatedAnnealing(double value){
        
                double truncated = std::floor(value * 100000.0) / 100000.0;

                return "+ " + std::to_string(truncated);
        }





        std::string formatDegradationSimulatedAnnealing(double value){

                double truncated = std::floor(value * 100000.0) / 100000.0;

                return "- " + std::to_string(truncated);
        }





        void printSimulatedAnnealingIterations(double current_time, 
                                            size_t current_iteration,
                                            size_t variation_iteration_count,
                                            double current_temperature,
                                            std::int64_t current_solution_objective_value,
                                            std::int64_t best_solution_objective_value,
                                            double relative_cumulative_improvement,
                                            double relative_cumulative_degradation){

                double inverse_variation_iteration_count = 1.0 / (std::max(static_cast<size_t>(1), variation_iteration_count));

                // line 1
                std::cout << "  " << std::setw(13) << std::left << "time (s)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(21) <<  std::left << std::fixed << std::setprecision(3) 
                        << current_time

                        << std::setw(7) << std::left << " "
                        << std::setw(21) << std::left << "current solution"

                        << std::setw(10) << std::left << " "
                        << std::setw(23) << std::left << "average improvement (%)"

                        << std::setw(10) << std::left << " "
                        << std::setw(23) << std::left << "average degradation (%)"
                        <<  "\n";


                // line 2
                std::cout << "  " << std::setw(13) << std::left << "iteration"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(21) <<  std::left <<  current_iteration

                        << std::setw(7) << std::left << " "
                        << std::string(16, '-')

                        << std::setw(15) << std::left << " "
                        << std::string(23, '-')

                        << std::setw(10) << std::left << " "
                        << std::string(23, '-')



                        <<  "\n";

                // line 3
                std::cout << "  " << std::setw(13) << std::left << "temperature"
                        << std::setw(2) << std::right << " : " 
                        << std::setw(21) <<  std::left 
                        << std::fixed << std::setprecision(2)
                        <<  current_temperature

                        << std::setw(7) << std::left << " "
                        << std::setw(16) << std::right << current_solution_objective_value

                        << std::setw(28) << std::left << " "
                        << BRIGHT_CYAN
                        <<  std::setw(8) <<  std::right << std::fixed << std::setprecision(5)
                        << formatImprovementSimulatedAnnealing(relative_cumulative_improvement * inverse_variation_iteration_count)
                        << RESET

                        << std::setw(23) << std::left << " "
                        << SOFT_RED
                        <<  std::setw(8) <<  std::right << std::fixed << std::setprecision(5)
                        << formatDegradationSimulatedAnnealing(relative_cumulative_degradation * inverse_variation_iteration_count)
                        << RESET 
                        <<  "\n";


                // line 4
                std::cout << "  " << std::setw(13) << std::left << "best solution"
                        << std::setw(2) << std::right << " : " 
                        <<  BRIGHT_YELLOW 
                        << std::setw(21) <<  std::left 
                        <<  best_solution_objective_value
                        << RESET

                        <<  "\n\n\n";

        }






        void printSummarySimulatedAnnealing(double construction_time, 
                                            double total_time,
                                            double current_temperature,
                                            size_t iterations,
                                            std::int64_t construction_objective,
                                            std::int64_t local_search_objective,
                                            Status status){

                std::cout << "\n";
                std::cout << "  " << YELLOW <<  "Summary" << RESET << "\n"
                          << "  "  << std::string(7, '-') << "\n\n"

                          << "  " << std::setw(29) << std::left << "construction time (s)"
                          << " : " << std::fixed << std::setprecision(5) << construction_time << "\n"

                          << "  " << std::setw(29) << std::left << "objective value"
                          << " : " << construction_objective << "\n\n"

                          << "  " << std::setw(29) << std::left << "local search time (s)"
                          << " : " << std::fixed << std::setprecision(5) << (total_time - construction_time) << "\n"

                          << "  " << std::setw(29) << std::left << "current temperature"
                          << " : " << std::fixed << std::setprecision(2) << current_temperature << "\n"

                          << "  " << std::setw(29) << std::left << "total iterations"
                          << " : " << iterations << "\n\n"


                          << "  " <<  std::setw(29) << std::left << "objective value"
                          << " : " << BRIGHT_YELLOW << local_search_objective << RESET << "\n"
                        
                          << "  " << std::setw(29) << std::left << "absolute improvement gap"
                          << " : "  << (local_search_objective - construction_objective) << "\n"

                          << "  " << std::setw(29) << std::left << "relative improvement gap (%)"
                          << " : " << "+ "
                          <<  BRIGHT_CYAN 
                          << (100.0 * (local_search_objective - construction_objective) / (construction_objective + 1e-9))
                          << RESET
                          << "\n\n"

                          << "  " << std::setw(29) << std::left << "total elapsed time (s)"
                          << " : " << std::fixed << std::setprecision(5) << total_time  << "\n"
                          
                          << "  " <<  std::setw(29) << std::left << "solution status"
                          << " : " << status << "\n\n";

        }


}