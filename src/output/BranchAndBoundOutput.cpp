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
 * @file BranchAndBoundOutput.cpp 
 * @author Josué Tambwe
 * @date 26 June 2026
 */

 
#include "output/BranchAndBoundOutput.hpp"

namespace spp{

        void printHeaderBaB(const Params &params, 
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

                        << MAGENTA << "Branch & Bound" << RESET << std::setw(5) << std::left << " "

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
                        << "\n";

                        

                // line 7
                std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row

                        << std::setw(66) << std::left << " "
                        << std::setw(18) << std::left << "LP Solver" <<  std::setw(2) << std::right << " : ";
                        if(params.milp_solver == 'g'){std::cout << BRIGHT_CYAN << "Gurobi" << RESET;}
                        else{std::cout << BRIGHT_CYAN << "Highs" << RESET;}
                std::cout <<  "\n";

                // line 8
                std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                        << std::setw(2) << std::right << " : " 
                        <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                        << std::setw(66) << std::left << " "

                        << std::setw(18) << std::left << "nodes exploration" <<  std::setw(2) << std::right << " : ";
                        if(params.exploration_strategy == 'b'){std::cout << "Best First";}
                        else{std::cout << "Depth First";}
                std::cout <<  "\n";

                // line 9
                std::cout << std::setw(109) << std::left << " "
                          << std::setw(18) << std::left << "branching rule" <<  std::setw(2) << std::right << " : ";
                          if(params.branching_value == 0.0){std::cout << "near  0.0";}
                          else if(params.branching_value == 0.5){std::cout << "near  0.5";}
                          else{std::cout << "near  1.0";}
                std::cout <<  "\n";

                // line 10
                std::cout << std::setw(109) << std::left << " "
                          << std::setw(18) << std::left << "target gap (%)" <<  std::setw(2) << std::right << " : "
                          << std::fixed << std::setprecision(3) << params.optimality_gap << "\n";

                // line 11
                std::cout << std::setw(109) << std::left << " "
                          << std::setw(18) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
                          << std::fixed << std::setprecision(2) << params.time_limit << "\n\n";                
        }




        void printHeaderLineBaB(){
        
                std::cout << "\n";
                // line 1
                std::cout << YELLOW;
                std::cout << std::right << std::setw(2)   << ""

                << std::left  << std::setw(12) << "Time (s)"
                << std::right << std::setw(9)  << ""

                << std::left  << std::setw(14) << "Processed nodes"
                << std::right << std::setw(15)  << ""

                << std::left  << std::setw(14) << "Open nodes"
                << std::right << std::setw(13)   << ""

                << std::left  << std::setw(16) << "Dual bound"
                << std::right << std::setw(6)   << ""

                << std::left  << std::setw(16) << "Primal bound"
                << std::right << std::setw(6)   << ""

                << std::left  << std::setw(16) << "Current gap (%)"
                << RESET
                << "\n";

                // line 2
                std::cout << std::right << std::setw(2)   << ""

                << std::left  << std::setw(12)   << std::string(8, '-')
                << std::right << std::setw(9)   << ""

                << std::left  << std::setw(14)  << std::string(15, '-')
                << std::right << std::setw(15)   << ""

                << std::left  << std::setw(14)    << std::string(10, '-')
                << std::right << std::setw(13)   << ""

                << std::left  << std::setw(16)  << std::string(10, '-')
                << std::right << std::setw(6)   << ""

                << std::left  << std::setw(16) << std::string(12, '-')
                << std::right << std::setw(6)   << ""

                << std::left  << std::setw(16) << std::string(15, '-')
                << "\n";

        }




        void printBaBIteration(double time,
                                size_t processed_nodes,
                                size_t open_nodes,
                                double dual_bound,
                                double primal_bound){

                // line 2
                std::cout << std::right << std::setw(2)   << ""

                << std::right  << std::setw(8) << std::fixed << std::setprecision(2)  << time
                << std::right << std::setw(9)   << ""

                << std::right << std::setw(19)  << processed_nodes
                << std::right << std::setw(11)   << ""

                << std::right  << std::setw(14)    << open_nodes
                << std::right << std::setw(11)   << ""

                << std::right  << std::setw(16)  << std::setprecision(3)
                << dual_bound
                << std::right << std::setw(6)   << ""

                << std::right  << std::setw(18) << std::setprecision(0)
                << primal_bound
                << std::right << std::setw(4)   << ""

                << std::right  << std::setw(21) << std::fixed << std::setprecision(4)
                << (((dual_bound - primal_bound)/dual_bound) * 100.0)
                << "\n\n";
        }



        void finalStatisticsBaB(double preprocessing_time,
                                double preprocessing_obj_value,
                                double dual_bound,
                                double primal_bound,
                                double total_time,
                                Status status,
                                size_t processed_nodes,
                                size_t open_nodes,
                                size_t nodes_fathomed_by_optimality,
                                size_t nodes_fathomed_by_dominance,
                                size_t nodes_fathomed_by_infeasibility,
                                const Params &params){

                std::cout << std::right << std::setw(2)   << ""
                << std::right  << YELLOW << "Preprocessing" << RESET << "\n"
                << std::string(2, ' ')
                << std::string(13, '-')
                << "\n";

                std::cout << std::string(2, ' ')
                << std::right << std::setw(2) 
                << std::left << std::setw(19)  << "time"
                << std::right  << " :  " 
                << std::left << preprocessing_time << std::setprecision(4) << " (s)"
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "objective value"
                << std::right  << " :  "
                << std::setprecision(0) 
                << std::left << preprocessing_obj_value
                << "\n\n\n";

                std::cout << std::right << std::setw(2)   << ""
                << std::right  << YELLOW << "Final Solution" << RESET << "\n"
                << std::string(2, ' ')
                << std::string(14, '-')
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "total time"
                << std::right  << " :  " 
                << std::setprecision(4)
                << std::left << total_time  << " (s)"
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "dual bound"
                << std::right  << " :  " 
                << std::left << std::setprecision(2) << dual_bound 
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "optimality gap (%)"
                << std::right  << " :  " 
                << std::left << MAGENTA <<  std::setprecision(3) << (((dual_bound - primal_bound) / dual_bound) * 100.0)
                << RESET
                << std::setprecision(4)
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "primal bound"
                << std::right  << " :  " 
                << std::setprecision(0)
                << std::left << BRIGHT_YELLOW << primal_bound << RESET
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(19)   << "status"
                << std::right  << " :  "
                << std::left << status
                << "\n\n\n";

                std::cout << std::right << std::setw(2)   << ""
                << std::right  << YELLOW << "Node Exploration" << RESET << "\n"
                << std::string(2, ' ')
                << std::string(16, '-')
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "strategy"
                << std::right  << " :  ";
                if(params.exploration_strategy == 'b'){std::cout << std::left << "Best First";} 
                else{std::cout << std::left << "Depth First";}

                std::cout << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "branching rule"
                << std::right  << " :  ";
                if(params.branching_value == 0.0){std::cout << std::left << "near  0.0";}
                else if(params.branching_value == 0.5){std::cout << std::left << "near  0.5";} 
                else{std::cout << std::left << "near  1.0";}

                std::cout << "\n\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "generated nodes"
                << std::right  << " :  " 
                << std::left << (processed_nodes + open_nodes)
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "processed nodes"
                << std::right  << " :  " 
                << std::left << processed_nodes
                << "\n\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "nodes fathomed by infeasibility"
                << std::right  << " :  " 
                << std::left << nodes_fathomed_by_infeasibility
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "nodes fathomed by dominance"
                << std::right  << " :  " 
                << std::left << nodes_fathomed_by_dominance
                << "\n"

                << std::string(2, ' ')
                << std::left << std::setw(32)   << "nodes fathomed by optimality"
                << std::right  << " :  " 
                << std::left << nodes_fathomed_by_optimality
                << "\n\n";

        }




}
