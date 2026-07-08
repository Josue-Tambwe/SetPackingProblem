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
 * @file OptionParser.cpp
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #include "input/OptionParser.hpp"


 namespace spp{



    void checkRequiredOptions(int argc, char** argv, const Params &params){

        Logger log; 
        bool has_algorithm = false;
        bool has_instance = false;
        bool use_milp_solver = false;
        bool has_update_interval = false;
        

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];
            if (arg.find("--instance=") == 0) {has_instance = true;}
            if (arg.find("--algorithm=") == 0) {has_algorithm = true;}
            if (arg.find("--solver=") == 0) {use_milp_solver = true;}
            if (arg.find("--update-interval=") == 0) {has_update_interval = true;}
        }

        if(!has_algorithm){log.error(" Missing required option : --algorithm=value");}

        if(!has_instance){log.error(" Missing required option : --instance=value");}

        if((params.algorithm == Algorithm::BranchAndBound ||
            params.algorithm == Algorithm::Milp) &&
            !use_milp_solver){

            log.error(" Missing required option : --solver=value");

        }

        if(params.algorithm == Algorithm::Grasp && !has_update_interval){

            log.error(" Missing required option : --update-interval=value");

        }


        
    }





    void unsupportedOptionsGreedy(int argc, char** argv){
        
        Logger log; 

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];

            if (arg.find("--time-limit=") == 0) {log.error(" --time-limit option is not supported by the greedy algorithm! ");}
            if (arg.find("--warm-start") == 0) {log.error(" --warm-start flag is not supported by the greedy algorithm! ");}
            if (arg.find("--solver=") == 0) {log.error(" --solver option is not supported by the greedy algorithm! ");}
            if (arg.find("--gap=") == 0) {log.error(" --gap option is not supported by the greedy algorithm! ");}
            if (arg.find("--branching-rule=") == 0) {log.error(" --branching-rule option is not supported by the greedy algorithm! ");}
            if (arg.find("--iterations=") == 0) {log.error(" --iterations option is not supported by the greedy algorithm! ");}
            if (arg.find("--nb-cycles=") == 0) {log.error(" --nb-cycles option is not supported by the greedy algorithm! ");}
            if (arg.find("--nb-threads=") == 0) {log.error(" --iterations option is not supported by the greedy algorithm! ");}
            if (arg.find("--exploration=") == 0) {log.error(" --exploration option is not supported by the greedy algorithm! ");}
            if (arg.find("--biais=") == 0) {log.error(" --biais option is not supported by the greedy algorithm! ");}
            if (arg.find("--update-interval=") == 0) {log.error(" --update-interval option is not supported by the greedy algorithm! ");}
            if (arg.find("--path-relinking=") == 0) {log.error(" --path-relinking option is not supported by the greedy algorithm! ");}
            
        }


    }




    void unsupportedOptionsBaB(int argc, char** argv){
        
        Logger log; 

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];

            if (arg.find("--warm-start") == 0) {log.error(" --warm-start flag is not supported by the bab algorithm! ");}
            if (arg.find("--iterations=") == 0) {log.error(" --iterations option is not supported by the bab algorithm! ");}
            if (arg.find("--nb-cycles=") == 0) {log.error(" --nb-cycles option is not supported by the bab algorithm! ");}
            if (arg.find("--nb-threads=") == 0) {log.error(" --iterations option is not supported by the bab algorithm! ");}
            if (arg.find("--biais=") == 0) {log.error(" --biais option is not supported by the bab algorithm! ");}
            if (arg.find("--update-interval=") == 0) {log.error(" --update-interval option is not supported by the bab algorithm! ");}
            if (arg.find("--path-relinking=") == 0) {log.error(" --path-relinking option is not supported by the bab algorithm! ");}
        }

    }




    void unsupportedOptionsMilp(int argc, char** argv){
        
        Logger log; 

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];

            if (arg.find("--nb-cycles=") == 0) {log.error(" --nb-cycles option is not supported by the milp algorithm! ");}
            if (arg.find("--gap=") == 0) {log.error(" --gap option is not supported by the milp algorithm! ");}
            if (arg.find("--exploration=") == 0) {log.error(" --exploration option is not supported by the milp algorithm! ");}
            if (arg.find("--branching-rule=") == 0) {log.error(" --branching-rule option is not supported by the milp algorithm! ");}
            if (arg.find("--iterations=") == 0) {log.error(" --iterations option is not supported by the milp algorithm! ");}
            if (arg.find("--nb-threads=") == 0) {log.error(" --iterations option is not supported by the milp algorithm! ");}
            if (arg.find("--biais=") == 0) {log.error(" --biais option is not supported by the milp algorithm! ");}
            if (arg.find("--update-interval=") == 0) {log.error(" --update-interval option is not supported by the milp algorithm! ");}
            if (arg.find("--path-relinking=") == 0) {log.error(" --path-relinking option is not supported by the milp algorithm! ");}
        }

    }




    void unsupportedOptionsGRASP(int argc, char** argv){
        
        Logger log; 

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];

            if (arg.find("--gap=") == 0) {log.error(" --gap option is not supported by the grasp algorithm! ");}
            if (arg.find("--exploration=") == 0) {log.error(" --exploration option is not supported by the grasp algorithm! ");}
            if (arg.find("--branching-rule=") == 0) {log.error(" --branching-rule option is not supported by the grasp algorithm! ");}
            if (arg.find("--iterations=") == 0) {log.error(" --iterations option is not supported by the grasp algorithm! ");}
            if (arg.find("--solver=") == 0) {log.error(" --solver option is not supported by the grasp algorithm! ");}
            if (arg.find("--warm-start") == 0) {log.error(" --warm-start flag is not supported by the grasp algorithm! ");}
        }

    }




    Params parseOptions(int argc, char** argv){

        Params params;
        Logger log;

        for(int i = 1; i < argc; i++){

            std::string arg = argv[i];

            // case of Help
            if(arg == "--help"){printHelp(); break;}

            // Checking if the option contains '=' 
            std::size_t position = arg.find('=');

            bool hasValue = (position != std::string::npos);

            // case of a flag
            if(!hasValue){

                if(arg == "--verbose"){params.verbose = true; continue;}
                if(arg == "--simd"){params.use_simd = true; continue;}
                if(arg == "--intensification"){params.use_intensification = true; continue;}
                if(arg == "--warm-start"){params.warm_start = true; continue;}
                if(arg == "--path-relinking"){params.use_path_relinking = true; continue;}

                else{log.error(" Unknown flag: " + arg);}
            }

            // case of an option with argument
            else{

                std::string name = arg.substr(0, position);
                std::string value = arg.substr(position + 1);

                if(name == "--algorithm"){

                    if(value == "greedy"){params.algorithm = Algorithm::Greedy; continue;}
                    else if(value == "grasp"){params.algorithm = Algorithm::Grasp; continue;}
                    else if(value == "ts"){params.algorithm = Algorithm::TabuSearch; continue;}
                    else if(value == "sa"){params.algorithm = Algorithm::SimulatedAnnealing; continue;}
                    else if(value == "genetic"){params.algorithm = Algorithm::GeneticAlgorithm; continue;}
                    else if(value == "bab"){params.algorithm = Algorithm::BranchAndBound; continue;}
                    else if(value == "milp"){params.algorithm = Algorithm::Milp; continue;}
                    else{log.error(" Unknown algorithm: " + name);}

                }

                if(name == "--instance"){params.instance_path = value; continue;}

                if(name == "--nb-threads"){
                    params.nb_threads = std::stoi(value);
                    if(params.nb_threads < 1){log.error(" nb-threads must be >= 1");}
                    continue;
                }

                if(name == "--time-limit"){
                    params.time_limit = std::stod(value);
                    if(params.time_limit < 0.0){log.error(" time-limit must be > 0");}
                    params.use_time_limit = true;
                    continue;
                }


                if(name == "--iterations"){
                    params.nb_max_iterations = std::stoi(value);
                    if(params.nb_max_iterations < 1){log.error(" iterations must be >= 1");}
                    params.use_max_iterations = true;
                    continue;
                }


                if(name == "--nb-cycles"){
                    params.nb_max_iterations = std::stoi(value);
                    if(params.nb_max_iterations < 1){log.error(" nb-cycles must be >= 1");}
                    params.use_max_iterations = true;
                    continue;
                }



                if(name == "--update-interval"){
                    params.update_interval = std::stoi(value);
                    if(params.update_interval < 1){log.error(" update-interval must be >= 1");}
                    continue;
                }


                
                if(name == "--solver"){

                    if(value == "gurobi"){
                        if(!HAS_GUROBI){log.error(" The solver has not been built with Gurobi");}
                        params.milp_solver = 'g'; 
                        continue;
                    }


                    if(value == "hexaly"){
                        if(!HAS_HEXALY){log.error(" The solver has not been built with Hexaly");}
                        params.milp_solver = 'x'; 
                        continue;
                    }


                    if(value == "highs"){
                        if(!HAS_HIGHS){log.error(" The solver has not been built with Highs");}
                        params.milp_solver = 'h'; 
                        continue;
                    }

                    log.error(" Unsupported milp solver : " + value); 
                    break;
                    
                }


                if(name == "--exploration"){

                    if(value == "bfs"){params.exploration_strategy = 'b'; continue;}
                    if(value == "dfs"){params.exploration_strategy = 'd'; continue;}
                    log.error(" Unknown nodes exploration strategy : " + value); 
                    break;
                    
                }


                if(name == "--branching-rule"){

                    if(value == "one"){params.branching_value = 1.0; continue;}
                    if(value == "zero"){params.branching_value = 0.0; continue;}
                    if(value == "fractional"){params.branching_value = 0.5; continue;}

                    log.error(" Unknown branching rule : " + value); 
                    break;
                    
                }

                if(name == "--gap"){
                    params.optimality_gap = std::stod(value);
                    if(params.optimality_gap < 0.0 || params.optimality_gap > 1.0){

                        log.error(" gap must be in the interval [0,1]");
                    }
                    continue;
                }

                if(name == "--biais"){
                    params.biais = std::stod(value);
                    if(params.biais < 0.0 || params.biais > 1.0){

                        log.error(" biais must be in the interval [0,1]");
                    }
                    continue;
                }

                // case of invalid option
                log.error("Unknown option: " + name);
                break;
            }

            
        }

        if(params.algorithm == Algorithm::Greedy){unsupportedOptionsGreedy(argc, argv);}

        else if(params.algorithm == Algorithm::BranchAndBound){unsupportedOptionsBaB(argc, argv);}

        else if(params.algorithm == Algorithm::Milp){unsupportedOptionsMilp(argc, argv);}

        else if(params.algorithm == Algorithm::Grasp){unsupportedOptionsGRASP(argc, argv);}

        if(params.algorithm == Algorithm::BranchAndBound &&
           params.milp_solver == 'x'){

            log.error(" Hexaly must not be used for linear relaxation in the Branch And Bound algorithm! "); 
        }

        if(params.algorithm == Algorithm::Milp && !params.warm_start){

            if(params.use_simd){log.error(" --simd must not be used when --warm-start is not enabled in the milp algorithm! ");}

            if(params.use_intensification){log.error(" --intensification must not be used when --warm-start is not enabled in the milp algorithm! ");}

        }

        /*if(params.algorithm == gap::Algorithm::AntColonyOptimizer){unsupportedOptionsACO(argc, argv);}

        if(params.algorithm == gap::Algorithm::BranchAndBound){unsupportedOptionsBaB(argc, argv);}

        if(params.algorithm == gap::Algorithm::Greedy){unsupportedOptionsGreedy(argc, argv);}

        if(params.algorithm == gap::Algorithm::Milp){unsupportedOptionsMilp(argc, argv);}*/

        

        /*if(params.algorithm == gap::Algorithm::AntColonyOptimizer &&
           !params.use_time_limit && !params.use_max_iterations){

                params.use_time_limit = true;

        }

        if(params.algorithm == gap::Algorithm::BranchAndBound &&
           params.milp_solver == 'x'){

            gap::Logger log; 
            log.error(" Hexaly must not be used for linear relaxation in the Branch And Bound algorithm! "); 

        }*/

        if(params.use_simd){
            if(HAS_X86 &&  !HAS_AVX2){log.error(" --simd flag is used but AVX2 extension not detected");}
            else if(HAS_ARM && !HAS_NEON){log.error(" --simd flag is used but NEON extension not detected");}
        }

        
        checkRequiredOptions(argc, argv, params);
        return params;

    }

 }

