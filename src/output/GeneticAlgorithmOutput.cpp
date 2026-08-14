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
 * @file GeneticAlgorithmOutput.cpp
 * @author Josué Tambwe
 * @date 14 August 2026
 */

 
#include "output/GeneticAlgorithmOutput.hpp"

namespace spp{


    void printHeaderGeneticAlgorithm(const Params &params, 
                                     const Instance &instance){

                const InstanceStatistics& stats = instance.getStatistics();

                // line 1
                std::cout << YELLOW;
                std::cout << "  " <<  "Instance statistics" << std::string(27, ' ') 
                        << "Algorithm" << std::string(14, ' ')
                        << "General Settings" << std::string(22, ' ')
                        << "Operator Settings"
                        << RESET << "\n";

                // line 2
                std::cout << "  "  << std::string(19, '-') << std::string(27, ' ') 
                        << std::string(9, '-') << std::string(14, ' ')
                        << std::string(16, '-') << std::string(22, ' ')
                        << std::string(17, '-')
                        << "\n";

                // line 3
                std::cout << "  " << std::setw(22) << std::left << "number of variables" 
                        <<  std::setw(2) << std::right << " : " 
                        <<  std::setw(14) <<  std::left << stats.nb_vars << std::setw(3) << std::left << " "

                        << MAGENTA << "Genetic Algorithm" << RESET << std::setw(10) << std::left << " "

                        << std::setw(19) << std::left << "SIMD vectorization" <<  std::setw(2) << std::right << " : ";
                        if(params.use_simd){std::cout << GREEN << std::setw(14) << std::left << "Enabled" << RESET;}
                        else{std::cout << RED << std::setw(14) << std::left << "Disabled" << RESET;}
                        std::cout << std::setw(2) << std::left << " "

                        << std::setw(19) << std::left << "pruning rate" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.pruning_rate
                        << 1 << "\n"; 

                //line 4
                std::cout << "  " << std::setw(22) << std::left << "number of constraints"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.nb_constraints << std::setw(28) << std::left << " "

                        
                        << std::setw(19) << std::left << "intensification" <<  std::setw(2) << std::right << " : ";
                        if(params.use_intensification){std::cout << GREEN << std::setw(14) << std::left << "Enabled" << RESET;}
                        else{std::cout << RED  << std::setw(14) << std::left << "Disabled" << RESET;}
                        std::cout << std::setw(2) << std::left << " "

                        << std::setw(19) << std::left << "restart interval" <<  std::setw(2) << std::right << " : "
                        << params.restart_interval 
                        << "\n";

                // line 5
                std::cout << std::setw(71) << std::left << " "
                        << std::setw(19) << std::left << "population size" << std::setw(3)  << std::right << " : "
                        <<  std::setw(14) <<  std::left << params.population_size 
                        << "\n";

                // line 6
                std::cout << "  " << std::setw(22) << std::left << "profit range"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left 
                        << (std::to_string(stats.min_profit) + " - " + std::to_string(stats.max_profit))
                        << std::setw(5) << std::left << " "

                        << YELLOW
                        << "Hardware"
                        << RESET
                        << std::setw(53) << std::left << " "

                        << std::setw(19) << std::left << "tabu tenure" <<  std::setw(2) << std::right << " : "
                        << params.tabu_tenure 
                        << "\n";

                                

                // line 7
                std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row
                        << std::setw(5) << std::left << " "

                        << std::string(8, '-')
                        <<  "\n";

                // line 8
                std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                        << std::setw(5) << std::left << " "

                        << std::setw(18) << std::left << "CPU ISA" <<  std::setw(2) << std::right << " : ";
                        if(HAS_X86){std::cout << std::setw(15) << std::left << "x86";} 
                        else{std::cout << std::setw(15) << std::left << "ARM";}
                        std::cout << std::setw(25) << std::left << " "

                        << std::setw(19) << std::left << "initial temperature" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.initial_temperature  
                        <<  "\n";



                // line 9
                std::string simd_value;

                        if(HAS_X86){
                                if(HAS_AVX2){simd_value = "AVX2 (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "AVX2 (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }

                        else{
                                if(HAS_NEON){simd_value = "NEON (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "NEON (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }


                std::cout << std::setw(48) << std::left << " ";
                        if(HAS_X86){
                                if(HAS_AVX2){simd_value = "AVX2 (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "AVX2 (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }

                        else{
                                if(HAS_NEON){simd_value = "NEON (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                else{simd_value = "NEON (" + std::string(RED) + "Not detected" + RESET + ")";}
                        }

                        

                        std::cout << std::setw(18) << std::left  << "SIMD extension" << std::setw(3)  << std::right << " : "
                        << std::setw(30) << std::left  << simd_value 
                        << std::setw(19) << std::left << " "

                        << std::setw(19) << std::left << "final temperature" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.final_temperature         
                        <<  "\n";

                //line 10
                std::cout << std::setw(48) << std::left << " "
                        << std::setw(15) << std::left << "CPU physical cores" << std::setw(3)  << std::right << " : "
                        <<  std::setw(21) <<  std::left << NUMBER_PHYSICAL_CORES
                        << std::setw(19) << std::left << " "

                        << std::setw(19) << std::left << "cooling factor" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.cooling_factor     
                        << "\n";



                // line 11
                std::cout << std::setw(48) << std::left << " "

                        << std::setw(18) << std::left << "threads used" <<  std::setw(2) << std::right << " : "
                        <<  std::setw(18) << std::left << params.nb_threads 
                        << std::setw(22) << std::left << " "
                        
                        << std::setw(19) << std::left << "cooling interval" <<  std::setw(2) << std::right << " : "
                        << params.cooling_interval        
                        <<  "\n\n";

                // line 12
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "crossover rate" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.crossover_rate          
                        <<  "\n";

                // line 13
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "mutation rate" <<  std::setw(2) << std::right << " : "
                        << params.mutation_rate          
                        <<  "\n";

                // line 14
                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "survivor rate" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.survivor_rate        
                        <<  "\n";


                // line 15
                if(params.use_max_iterations){

                std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "maximum generations" <<  std::setw(2) << std::right << " : "
                        << params.nb_max_iterations << "\n";
                }


                // line 16
                if(params.use_time_limit){

                    std::cout << std::setw(109) << std::left << " "
                        << std::setw(19) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
                        << std::fixed << std::setprecision(2) << params.time_limit << "\n";  
                
                }  

                std::cout << "\n\n";
    }


}