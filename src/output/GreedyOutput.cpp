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
 * @file GreedyOutput.cpp 
 * @author Josué Tambwe
 * @date 6 June 2026
 */

 
#include "output/GreedyOutput.hpp"

namespace spp{


        void printHeaderGreedy(const Params &params, 
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
                        <<  std::setw(16) <<  std::left << stats.nb_vars << std::setw(7) << std::left << " "

                        << MAGENTA << "Greedy" << RESET << std::setw(9) << std::left << " "

                        << std::setw(18) << std::left << "CPU ISA" <<  std::setw(2) << std::right << " : ";
                        if(HAS_X86){std::cout << std::setw(15) << std::left << "x86";} 
                        else{std::cout << std::setw(15) << std::left << "ARM";}

                std::cout << std::setw(8) << std::left << " "
                        << std::setw(18) << std::left << "threads used" <<  std::setw(2) << std::right << " : "
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
                        << std::setw(18) << std::left << "verbose" <<  std::setw(2) << std::right << " : ";
                        if(params.verbose){std::cout << GREEN << "Enabled" << RESET;}
                        else{std::cout << RED << "Disabled" << RESET;}
                std::cout <<  "\n";

                // line 7
                std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row
                        <<  "\n";

                std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                        <<  "\n\n\n";
                        
        }




        std::string computeVariationString(double new_value, double old_value){

                double eps = 1e-9;
                double variation = 100.0 * (new_value - old_value) / (old_value + eps);

                 // Truncate to 5 decimals
                double truncated = std::floor(variation * 100000.0) / 100000.0;

                std::string sign;

                if(truncated >= 0.0){sign = "+ ";} else{sign = "- ";}


                return (sign + std::to_string(truncated));
        }




        void printStepsGreedy(double &one_two_exchange_time, 
                                double &one_one_exchange_time, 
                                double &two_one_exchange_time, 
                                double &zero_one_exchange_time,
                                std::int64_t &construction_objective,
                                std::int64_t &one_two_exchange_objective,
                                std::int64_t &one_one_exchange_objective,
                                std::int64_t &two_one_exchange_objective,
                                std::int64_t &zero_one_exchange_objective,
                                size_t &one_two_exchange_iterations,
                                size_t &one_one_exchange_iterations,
                                size_t &two_one_exchange_iterations,
                                size_t &zero_one_exchange_iterations,
                                const Params &params){

                std::cout << "\n";
                // line 1
                std::cout << YELLOW;
                std::cout << "  " <<  "Steps" << std::string(35, ' ') 
                        << "Time (s)" << std::string(20, ' ')
                        << "Iterations" << std::string(20, ' ')
                        << "Objective value" << std::string(8, ' ')
                        << "Variation (%)"
                        << RESET << "\n";

                // line 2
                std::cout << "  "  << std::string(5, '-') << std::string(35, ' ') 
                        << std::string(8, '-') << std::string(20, ' ')
                        << std::string(10, '-') << std::string(20, ' ')
                        << std::string(15, '-') << std::string(8, ' ')
                        << std::string(13, '-')
                        << "\n";

                // case of intensification : 1-1 -> 1-2 -> 2-1 -> 0-1
                if(params.use_intensification){
                        // 1-1 move
                        std::cout << "  " << std::setw(18) << std::left << "1-1 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << one_one_exchange_time
                                << std::setw(30) << std::right << one_one_exchange_iterations
                                << std::setw(35) << std::right << one_one_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(one_one_exchange_objective, construction_objective) 
                                << "\n\n";

                        // 1-2
                        std::cout << "  " << std::setw(18) << std::left << "1-2 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << one_two_exchange_time
                                << std::setw(30) << std::right << one_two_exchange_iterations
                                << std::setw(35) << std::right << one_two_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(one_two_exchange_objective, one_one_exchange_objective)  
                                << "\n\n";

                        // 2-1
                        std::cout << "  " << std::setw(18) << std::left << "2-1 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << two_one_exchange_time
                                << std::setw(30) << std::right << two_one_exchange_iterations
                                << std::setw(35) << std::right << two_one_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(two_one_exchange_objective, one_two_exchange_objective)  
                                << "\n\n";
                }

                // without intensification :  1-2 -> 1-1 -> 2-1 -> 0-1
                else{

                        // 1-2
                        std::cout << "  " << std::setw(18) << std::left << "1-2 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << one_two_exchange_time
                                << std::setw(30) << std::right << one_two_exchange_iterations
                                << std::setw(35) << std::right << one_two_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(one_two_exchange_objective, construction_objective)  
                                << "\n\n";

                        // 1-1 move
                        std::cout << "  " << std::setw(18) << std::left << "1-1 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << one_one_exchange_time
                                << std::setw(30) << std::right << one_one_exchange_iterations
                                << std::setw(35) << std::right << one_one_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(one_one_exchange_objective, one_two_exchange_objective) 
                                << "\n\n";

                        // 2-1
                        std::cout << "  " << std::setw(18) << std::left << "2-1 neighborhood"
                                << std::setw(30) << std::right << std::fixed << std::setprecision(5) << two_one_exchange_time
                                << std::setw(30) << std::right << two_one_exchange_iterations
                                << std::setw(35) << std::right << two_one_exchange_objective
                                << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                                << computeVariationString(two_one_exchange_objective, one_one_exchange_objective)  
                                << "\n\n";
                }

                // 0-1 move
                std::cout << "  " << std::setw(18) << std::left << "0-1 neighborhood"
                        << std::setw(30) << std::right << std::fixed << std::setprecision(5) << zero_one_exchange_time
                        << std::setw(30) << std::right << zero_one_exchange_iterations
                        << std::setw(35) << std::right << zero_one_exchange_objective
                        << std::setw(21) << std::right << std::fixed << std::setprecision(5)
                        << computeVariationString(zero_one_exchange_objective, two_one_exchange_objective) 
                        << "\n\n";

        }



        void printSummaryGreedy(double construction_time, 
                                double total_time,
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
                          << "  " <<  std::setw(29) << std::left << "objective value"
                          << " : " << BRIGHT_YELLOW << local_search_objective << RESET << "\n\n"
                        
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