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

        std::cout << " Required options:\n" << " " << std::string(16, '-') << "\n\n"

                  << "  "  << "--algorithm=value" << std::string(11, ' ')
                  << "Algorithm to use (values: bab, genetic, grasp, greedy, milp, sa, ts)\n \n"

                  << "  "  << "--instance=path" << std::string(13, ' ')
                  << "Path to the instance file\n\n"

                  << "  " << "--update-interval=value" << std::string(5, ' ')
                  << "Number of iterations before updating alpha probabilities for the GRASP algorithm \n\n"

                  << "  " << "--tabu-tenure=value" << std::string(9, ' ')
                  << "Number of iterations that a move remains tabu for the Tabu Search Algorithm \n\n"

                  << "  " << "--restart-interval=value" << std::string(4, ' ')
                  << "Number of iterations before rebuilding the current solution for the Tabu Search and Simulated Annealing\n\n"

                  << "  " << "--cooling-interval=value" << std::string(4, ' ')
                  << "Number of iterations between two temperature updates in the Simulated Annealing schedule\n\n"

                  << "  " << "--pop-size=value" << std::string(12, ' ')
                  << "Population size for the Genetic Algorithm \n\n"

                  << "  " << "--solver=value" << std::string(14, ' ')
                  << "Solver backend to use (values: gurobi, highs, hexaly). Hexaly is only used for a MILP resolution approach \n\n\n";
    }


    void printOptionalOptions(){

        std::cout << " Optional options:\n" << " " << std::string(23, '-') << "\n\n"

                << "  " << "--simd" << std::string(27, ' ')
                << "Enable SIMD-accelerated kernels (AVX2/NEON)\n \n"

                << "  " << "--warm-start" << std::string(21, ' ')
                << "Provide a feasible starting point for the MILP resolution approach\n \n"

                << "  " << "--verbose" << std::string(24, ' ')
                << "Enable verbose mode\n \n"

                << "  " << "--intensification" << std::string(16, ' ')
                << "Enable an intensified Variable Neighborhood Descent local search\n \n"

                << "  " << "--path-relinking" << std::string(17, ' ')
                << "Enable the Path-Relinking procedure to intensify the GRASP algorithm\n \n"

                << "  "  << "--nb-threads=value" << std::string(15, ' ')
                << "Number of threads to use (default: number of physical CPU cores)\n \n"

                << "  "  << "--time-limit=value" << std::string(15, ' ')
                << "Set the time limit in seconds (default: 10)\n \n"

                << "  "  << "--branching-rule=value" << std::string(11, ' ')
                << "Branching rule strategy for the Branch and Bound algorithm (values: one, zero, fractional)\n \n"

                << "  "  << "--exploration=value" << std::string(14, ' ')
                << "Node exploration strategy for the Branch and Bound algorithm (values: bfs, dfs)\n \n"

                << "  "  << "--gap=value" << std::string(22, ' ')
                << "Target optimality gap in [0,1] for the Branch and Bound algorithm (default: 0.0)\n \n"

                << "  "  << "--iterations=value" << std::string(15, ' ')
                << "Number of iterations for Genetic Algorithm, Simulated Annealing and Tabu Search (default: 1)\n \n"

                << "  "  << "--nb-cycles=value" << std::string(16, ' ')
                << "Number of cycles for GRASP. Each cycle runs update-interval iterations before updating alpha probabilities\n \n"

                << "  "  << "--nb-elites=value" << std::string(16, ' ')
                << "Number of elite solutions within the elite solutions pool for Reactive GRASP with Path-Relinking\n \n"
                
                
                << "  "  << "--bias=value" << std::string(21, ' ')
                << "Bias factor in [0,1] for GRASP and Genetic algorithm (low = 0.0, high = 1.0, default: 0.0)\n \n"

                << "  "  << "--alpha=value" << std::string(20, ' ')
                << "Greedy-randomness level in [0,1] (low = highly random, high = highly greedy, default: 0.5)\n\n"

                << "  "  << "--pruning-rate=value" << std::string(13, ' ')
                << "Proportion in [0,1] of elite inactive variables tested in restricted 1-2 and 2-1 neighborhoods\n\n"

                << "  "  << "--cooling-factor=value" << std::string(11, ' ')
                << "Geometric decay coefficient in [0,1] applied to the temperature at each cooling interval\n\n"

                << "  "  << "--initial-temperature=value" << std::string(6, ' ')
                << "Starting temperature of the Simulated Annealing schedule (higher values accept more worsening moves)\n\n"

                << "  "  << "--final-temperature=value" << std::string(8, ' ')
                << "Minimum temperature allowed by the cooling schedule. The algorithm stops when it is reached\n\n\n";

    }



    void printExampleRun(){

        std::cout << " Examples:\n" << " " << std::string(9, '-') << "\n\n"

            << "    ./bin/spp_solver --algorithm=greedy "
            << "--instance=benchmarks/pb_1000rnd0700.dat  --intensification  --simd  --verbose\n\n"

            << "    ./bin/spp_solver --algorithm=grasp "
            << "--instance=benchmarks/pb_1000rnd0700.dat  --update-interval=100  --time-limit=30 --bias=0.7 --simd \n\n"

            << "    ./bin/spp_solver --algorithm=grasp "
            << "--instance=benchmarks/pb_100rnd0100.dat  --update-interval=50  --time-limit=30  --simd --path-relinking\n\n"

            << "    ./bin/spp_solver --algorithm=ts "
            << "--instance=benchmarks/pb_1000rnd0700.dat  --tabu-tenure=10 --restart-interval=50  --time-limit=60  --simd\n\n"

            << "    ./bin/spp_solver --algorithm=sa "
            << "--instance=benchmarks/pb_1000rnd0700.dat --cooling-interval=100 --alpha=0.75 --cooling-factor=0.95  --simd\n\n"

            << "    ./bin/spp_solver --algorithm=bab "
            << "--instance=benchmarks/pb_1000rnd0700.dat  --intensification  --solver=gurobi --exploration=dfs\n\n"

            << "    ./bin/spp_solver --algorithm=milp "
            << "--instance=benchmarks/pb_1000rnd0700.dat  --solver=highs  --simd  --time-limit=30 --warm-start\n\n";




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