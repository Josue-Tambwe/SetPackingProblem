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
 * @file RunGreedy.cpp
 * @author Josué Tambwe
 * @date 4 June 2026
 */

 #include "algorithms/greedy/RunGreedy.hpp"

 namespace spp{


    void localSearchScalar(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance){

        Timer timer;
        std::vector<float> scores = computeVariableScores(instance);
        
        // case of intense local search
        if(params.use_intensification){

            // 1-1 exchange
            timer.start();
            oneOneNeighborhood(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);

            // 1-2 exchange
            timer.start();
            oneTwoNeighborhood(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);

        }

        else{

            // 1-2 exchange
            timer.start();
            oneTwoNeighborhood(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);

            // 1-1 exchange
            timer.start();
            oneOneNeighborhood(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);
            
        }

        

        // 2-1 exchange
        timer.start();
        twoOneNeighborhood(scores, solution, instance);
        timer.stop();
        two_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        two_one_exchange_objective = solution.getObjectiveValue(instance);

        // 0-1 exchange
        timer.start();
        zeroOneNeighborhood(scores, solution, instance);
        timer.stop();
        zero_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        zero_one_exchange_objective = solution.getObjectiveValue(instance);
        
    }



    #if HAS_X86 && HAS_AVX2

    void localSearchSIMDX86(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance){

        Timer timer;
        std::vector<float> scores = computeVariableScores(instance);

        // case of intense local search
        if(params.use_intensification){

            // 1-1 exchange
            timer.start();
            oneOneNeighborhoodSIMDX86(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);

            // 1-2 exchange
            timer.start();
            oneTwoNeighborhoodSIMDX86(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);
            
        }

        else{

            // 1-2 exchange
            timer.start();
            oneTwoNeighborhoodSIMDX86(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);

            // 1-1 exchange
            timer.start();
            oneOneNeighborhoodSIMDX86(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);
        }

        // 2-1 exchange
        timer.start();
        twoOneNeighborhoodSIMDX86(scores, solution, instance);
        timer.stop();
        two_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        two_one_exchange_objective = solution.getObjectiveValue(instance);
        
        // 0-1 exchange
        timer.start();
        zeroOneNeighborhoodSIMDX86(scores, solution, instance);
        timer.stop();
        zero_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        zero_one_exchange_objective = solution.getObjectiveValue(instance);
    }

    #endif



    #if HAS_ARM && HAS_NEON

    void localSearchSIMDARM(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance){

        Timer timer;
        std::vector<float> scores = computeVariableScores(instance);

        // case of intense local search
        if(params.use_intensification){

            // 1-1 exchange
            timer.start();
            oneOneNeighborhoodSIMDARM(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);
            
            // 1-2 exchange
            timer.start();
            oneTwoNeighborhoodSIMDARM(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);
        }

        else{

            // 1-2 exchange
            timer.start();
            oneTwoNeighborhoodSIMDARM(scores, solution, instance);
            timer.stop();
            one_two_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_two_exchange_objective = solution.getObjectiveValue(instance);
            
            // 1-1 exchange
            timer.start();
            oneOneNeighborhoodSIMDARM(scores, solution, instance);
            timer.stop();
            one_one_exchange_time = timer.getElapsedTime();
            timer.reset();
            one_one_exchange_objective = solution.getObjectiveValue(instance);
        }

        // 2-1 exchange
        timer.start();
        twoOneNeighborhoodSIMDARM(scores, solution, instance);
        timer.stop();
        two_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        two_one_exchange_objective = solution.getObjectiveValue(instance);
        
        // 0-1 exchange
        timer.start();
        zeroOneNeighborhoodSIMDARM(scores, solution, instance);
        timer.stop();
        zero_one_exchange_time = timer.getElapsedTime();
        timer.reset();
        zero_one_exchange_objective = solution.getObjectiveValue(instance);
    }

    #endif 




    void runGreedy(const Params &params){

        const Instance instance(params);
        Timer timer;
        Logger log;

        // construction

        timer.start();
        Solution solution = deterministicConstruction(instance);
        double construction_time = timer.getElapsedTime();

        if(solution.getStatus() != Status::FEASIBLE && 
           solution.getStatus() != Status::OPTIMAL) {

            log.warning("the greedy construction heuristic failed.");
            std::cout << " Processing time : " << construction_time << " (s) \n" << std::endl;
            return;
        }

        std::int64_t construction_objective = solution.getObjectiveValue(instance);
        log.info("the greedy construction heuristic succeed !");

        // local search

        double one_two_exchange_time;
        double one_one_exchange_time;
        double two_one_exchange_time;
        double zero_one_exchange_time;

        std::int64_t one_two_exchange_objective;
        std::int64_t one_one_exchange_objective;
        std::int64_t two_one_exchange_objective;
        std::int64_t zero_one_exchange_objective;

        if(params.use_simd){

            #if HAS_AVX2

                localSearchSIMDX86(one_two_exchange_time, 
                                   one_one_exchange_time, 
                                   two_one_exchange_time, 
                                   zero_one_exchange_time,
                                   one_two_exchange_objective,
                                   one_one_exchange_objective,
                                   two_one_exchange_objective,
                                   zero_one_exchange_objective,
                                   solution,
                                   params,
                                   instance);

            #elif HAS_NEON

                localSearchSIMDARM(one_two_exchange_time, 
                                   one_one_exchange_time, 
                                   two_one_exchange_time, 
                                   zero_one_exchange_time,
                                   one_two_exchange_objective,
                                   one_one_exchange_objective,
                                   two_one_exchange_objective,
                                   zero_one_exchange_objective,
                                   solution,
                                   params,
                                   instance);


            #endif
        }

        else{

            localSearchScalar(one_two_exchange_time, 
                              one_one_exchange_time, 
                              two_one_exchange_time, 
                              zero_one_exchange_time,
                              one_two_exchange_objective,
                              one_one_exchange_objective,
                              two_one_exchange_objective,
                              zero_one_exchange_objective,
                              solution,
                              params,
                              instance);
        }

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}
        else{solution.setStatus(Status::INFEASIBLE);}


        std::cout << " construction time : " << construction_time 
                  << "  objective : "  << construction_objective << " \n"
                  << " 1-2 time : " << one_two_exchange_time 
                  << "  objective : "  << one_two_exchange_objective << " \n"
                  << " 1-1 time : " << one_one_exchange_time 
                  << "  objective : "  << one_one_exchange_objective << " \n"
                  << " 2-1 time : " << two_one_exchange_time 
                  << "  objective : "  << two_one_exchange_objective << " \n"
                  << " 0-1 time : " << zero_one_exchange_time << "\n"
                  << "  objective : "  << zero_one_exchange_objective << " \n"
                  << "  status :" << solution.getStatus() << "\n\n";
                  



    }
 }