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
 * @file MilpSolverOutput.cpp 
 * @author Josué Tambwe
 * @date 27 June 2026
 */

 
#include "output/MilpSolverOutput.hpp"


namespace spp{


    void printHeaderMilp(const Params &params, 
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
            <<  std::setw(16) <<  std::left << stats.nb_vars << std::setw(8) << std::left << " "

            << MAGENTA << "MILP" << RESET << std::setw(10) << std::left << " "
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
            << std::setw(18) << std::left << "warm start" <<  std::setw(2) << std::right << " : ";
            if(params.warm_start){std::cout << GREEN << "Enabled" << RESET;}
            else{std::cout << RED << "Disabled" << RESET;}
        std::cout << "\n";       

        // line 7
        std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
            << std::setw(2) << std::right << " : " 
            <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row

            << std::setw(66) << std::left << " "
            << std::setw(18) << std::left << "MILP Solver" <<  std::setw(2) << std::right << " : ";
            if(params.milp_solver == 'g'){std::cout << BRIGHT_CYAN << "Gurobi" << RESET;}
            else if(params.milp_solver == 'x'){std::cout << BRIGHT_CYAN << "Hexaly" << RESET;}
            else{std::cout << BRIGHT_CYAN << "Highs" << RESET;}
        std::cout <<  "\n";

        // line 8
        std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
            << std::setw(2) << std::right << " : " 
            <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
            << std::setw(66) << std::left << " "

            << std::setw(18) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
            << std::fixed << std::setprecision(2) << params.time_limit                
            <<  "\n\n\n";
    }




    void finalStatisticsMilp(double preprocessing_time,
                             double total_time,
                             std::int64_t obj_value,
                             Status status){

        std::cout << std::right << std::setw(2)   << ""
            << std::right  << YELLOW << "Summary" << RESET << "\n"
            << std::string(2, ' ')
            << std::string(7, '-')
            << "\n";

        std::cout << std::string(2, ' ')
            << std::right << std::setw(2) 
            << std::left << std::setw(19)  << "preprocessing time"
            << std::right  << " :  " 
            << std::left  << std::setprecision(4) << preprocessing_time << " (s)"
            << "\n\n"

            << std::string(2, ' ')
            << std::left << std::setw(19)  << "total elapsed time"
            << std::right  << " :  " 
            << std::left << std::setprecision(4) << total_time << " (s)"
            << "\n\n"

            << std::string(2, ' ')
            << std::left << std::setw(19)   << "objective value"
            << std::right  << " :  "
            << std::setprecision(0) << BRIGHT_YELLOW
            << std::left << obj_value << RESET
            << "\n\n"

            << std::string(2, ' ')
            << std::left << std::setw(19)   << "status"
            << std::right  << " :  "
            << std::left << status << "\n\n";
    }

}