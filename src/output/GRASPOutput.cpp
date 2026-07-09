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
 * @file GRASPOutput.cpp 
 * @author Josué Tambwe
 * @date 8 July 2026
 */

 
#include "output/GRASPOutput.hpp"


namespace spp{

        void printHeaderGRASP(const Params &params, 
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
                        <<  std::setw(16) <<  std::left << stats.nb_vars << std::setw(3) << std::left << " "

                        << MAGENTA << "Reactive GRASP" << RESET << std::setw(5) << std::left << " "

                        << std::setw(18) << std::left << "CPU ISA" <<  std::setw(2) << std::right << " : ";
                        if(HAS_X86){std::cout << std::setw(15) << std::left << "x86";} 
                        else{std::cout << std::setw(15) << std::left << "ARM";}

                        std::cout << std::setw(8) << std::left << " "
                                << std::setw(18) << std::left << "threads used" <<  std::setw(2) << std::right << " : "
                                << params.nb_threads << "\n"; // always 1 thread

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

                        << std::setw(18) << std::left << "SIMD vectorization" <<  std::setw(2) << std::right << " : ";
                        if(params.use_simd){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                        std::cout << "\n";

                // line 5
                std::cout << std::setw(65) << std::left << " "
                        << std::setw(15) << std::left << "CPU physical cores" << std::setw(3)  << std::right << " : "
                        <<  std::setw(4) <<  std::left << NUMBER_PHYSICAL_CORES << std::setw(19) << std::left << " "

                        << std::setw(18) << std::left << "intensification" <<  std::setw(2) << std::right << " : ";
                        if(params.use_intensification){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                        std::cout << "\n";

                // line 6
                std::cout << "  " << std::setw(22) << std::left << "profit range"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left 
                        << (std::to_string(stats.min_profit) + " - " + std::to_string(stats.max_profit))
                        << std::setw(66) << std::left << " "

                        
                        << BRIGHT_CYAN
                        << std::setw(18) << std::left  <<  "Path-Relinking" << RESET <<  std::setw(2) << std::right << " : ";
                        if(params.use_path_relinking){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                std::cout  << "\n";

                                

                // line 7
                std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row
                        <<  "\n";

                // line 8
                std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                        << std::setw(66) << std::left << " "

                        << std::setw(18) << std::left << "biais" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.biais
                        <<  "\n";

                // line 9
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(18) << std::left << "update interval" <<  std::setw(2) << std::right << " : "
                        << params.update_interval          
                        <<  "\n";

                // line 10
                if(params.use_max_iterations){

                std::cout << std::setw(109) << std::left << " "
                        << std::setw(18) << std::left << "number of cycles" <<  std::setw(2) << std::right << " : "
                        << params.nb_max_iterations << "\n";
                }

                // line 11
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(18) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.time_limit << "\n";               


                std::cout << "\n\n\n";

        }




        void printGRASPInitialElite(double current_time, 
                                size_t current_iteration,
                                std::int64_t elite_objective_value,
                                const std::array<float, 10> &alpha_values,
                                const std::array<float, 10> &alpha_probabilities){

                // line 1
                std::cout << "  " << std::setw(25) << std::left << "time (s)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) << std::left << std::fixed << std::setprecision(4) 
                        << current_time

                        << std::setw(3) << std::left << " "
                        << std::setw(20) << std::left << "alpha values"
                        << std::setw(2) << std::right << " : ";

                for(float alpha : alpha_values){

                        std::cout << std::fixed << std::setprecision(2) << alpha << " | ";
                }

                        
                std::cout <<  "\n";

                // line 2
                std::cout << "  " << std::setw(25) << std::left << "iteration"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left <<  current_iteration
                        <<  "\n";

                // line 3
                std::cout << "  " << std::setw(25) << std::left << "elite objective value"
                        << std::setw(2) << std::right << " : " 
                        <<  BRIGHT_YELLOW 
                        << std::setw(16) <<  std::left <<  elite_objective_value 
                        << RESET
                        <<  "\n\n\n";

        }




        void printGRASPIteration(double current_time, 
                                size_t current_iteration,
                                std::int64_t elite_objective_value,
                                std::int64_t local_best_solution_objective_value,
                                const std::array<float, 10> &alpha_values,
                                const std::array<float, 10> &alpha_probabilities){


                // line 1
                std::cout << "  " << std::setw(25) << std::left << "time (s)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(4) 
                        << current_time

                        << std::setw(3) << std::left << " "
                        << std::setw(20) << std::left << "alpha values"
                        << std::setw(2) << std::right << " : ";

                for(float alpha : alpha_values){

                        std::cout << std::fixed << std::setprecision(2) << alpha << " | ";
                }

                std::cout <<  "\n";

                // line 2
                std::cout << "  " << std::setw(25) << std::left << "iteration"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left <<  current_iteration
                        <<  "\n";

                // line 3
                std::cout << "  " << std::setw(25) << std::left << "elite objective value"
                        << std::setw(2) << std::right << " : " 
                        <<  BRIGHT_YELLOW 
                        << std::setw(16) <<  std::left <<  elite_objective_value 
                        << RESET
                        <<  "\n";

                double improvement_gap = ((local_best_solution_objective_value - elite_objective_value) * 100.0) / (elite_objective_value + 1e-9);
                improvement_gap = std::max(0.0, improvement_gap);

                // line 4
                std::cout << "  " << std::setw(25) << std::left << "relative improvement (%)"
                        << std::setw(2) << std::right << " : " << "+ "
                        << BRIGHT_CYAN
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(4) 
                        << improvement_gap
                        << RESET
                        <<  "\n\n\n";

        }
}
