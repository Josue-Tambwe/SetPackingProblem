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
 * @file GeneralOutput.cpp 
 * @author Josué Tambwe
 * @date 6 June 2026
 */

 
 #include "output/GeneralOutput.hpp"

 namespace spp{

    void printHeader(){

        std::cout << "\n";
        std::cout << std::setw(46) << std::right << " " << std::string(39, '=') << "\n";
        std::cout << std::setw(85) << std::right << "SPP-Solver : Set Packing Problem Solver" << "\n";
        std::cout << std::setw(46) << std::right << " " << std::string(39, '=') << "\n\n";
    }


    void printUsage(){

                std::cout << " Usage:\n";
                std::cout << " ------\n\n";
                std::cout << "  spp_solver OPTIONS\n\n\n";
    }


    void printRequiredOptions(){

        std::cout << " Required options:\n" << " " << std::string(17, '-') << "\n\n"

                  << "  "  << "--algorithm=value" << std::string(5, ' ')
                  << "Algorithm to use (values: bab, genetic, grasp, greedy, milp, sa, ts)\n \n"

                  << "  "  << "--instance=path" << std::string(7, ' ')
                  << "Path to the instance file\n\n"

                  << "  " << "--pop-size=value" << std::string(6, ' ')
                  << "Population size for the Genetic Algorithm \n\n"

                  << "  " << "--solver=value" << std::string(8, ' ')
                  << "Solver backend to use (values: gurobi, highs, hexaly). Hexaly is only used for a MILP resolution approach \n\n\n";
    }


    void printOptionalOptions(){

        std::cout << " Optional options:\n" << std::string(17, '-') << "\n\n"

                << "  "  << "--nb-threads=value" << std::string(5, ' ')
                << "Number of threads (default: number of physical CPU cores)\n \n"

                << "  " << "--simd" << std::string(17, ' ')
                << "Enable SIMD-accelerated kernels (AVX2/NEON)\n \n"

                << "  " << "--verbose" << std::string(14, ' ')
                << "Enable verbose mode\n \n"

                << "  " << "--intensification" << std::string(6, ' ')
                << "Enable an intensified Variable Neighborhood Descent local search\n \n"

                << "  "  << "--time-limit=value" << std::string(5, ' ')
                << "Set the time limit in seconds (default: 10)\n \n"

                << "  "  << "--iterations=value" << std::string(5, ' ')
                << "Set the number of iterations for GRASP, Genetic Algorithm, Simulated Annealing and Tabu Search\n \n";    
    }



    void printExampleRun(){

        std::cout << " Examples:\n" << " " << std::string(9, '-') << "\n\n"

            << "    spp_solver --algorithm=greedy "
            << "--instance=../benchmarks/gap_a/a05100  --intensification  --simd\n\n";



    }


    void printHelp(){

        printHeader();
        printUsage();
        printRequiredOptions();
        printOptionalOptions();
        printExampleRun();
        std::exit(0);
    }

}