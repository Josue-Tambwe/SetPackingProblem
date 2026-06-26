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
 * @file RunMilpSolver.hpp
 * @brief implements functions for Mixed Integer Programming resolution approch
 * @author Josué Tambwe
 * @date 27 June 2026
 */

#pragma once

#if USE_MILP

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Timer.hpp"
#include "output/Logger.hpp"
#include "dataStructures/Parameters.hpp"
#include "output/GeneralOutput.hpp"
#include "output/MilpSolverOutput.hpp"
#include "algorithms/greedy/DeterministicConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/branchAndBound/BaBUtils.hpp"

#if HAS_GUROBI
#include "algorithms/milpSolver/GurobiBackend.hpp"
#endif

#if HAS_HEXALY
#include "algorithms/milpSolver/HexalyBackend.hpp"
#endif
 
#if HAS_HIGHS
#include "algorithms/milpSolver/HighsBackend.hpp"
#endif

#include <vector>

namespace spp{


    /**
     * @brief generates an initial solution with greedy construction + VND local search
     */
    Solution generateInitialSolution(const Params &params, const Instance &instance);



    /**
     * @brief convert a solution (SPP Solution) into a 1D array in order to set an initial
     *        solution for a MILP solver (warm start)
     */
    std::vector<double> computeWarmStart(Solution &solution);



    /**
     * @brief converts a 1D solution vector into a SPP solution 
     */
    Solution extractMilpSolution(std::vector<double> &milp_sol, const Instance &instance);




    /**
     * @brief runs the MILP resolution without a warm start solution
     */

    template <typename Backend>

    void genericMilpWithoutWarmStart(const Params &params, Backend &solver){

        // initialization
        const Instance instance(params);
        printHeaderMilp(params, instance);
        solver.buildIntegerModel(instance);
        Timer timer;
        Logger log;

        timer.start();
        solver.solveIntegerModel(params.time_limit);
        timer.stop();

        // feasibility test
        if(solver.isInFeasible()){

            log.warning("the problem is infeasible.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return;
        }

        // retrieving the model solution vector
        std::vector<double> milp_sol = solver.getSolution();

        // integrality test
        if(isInteger(milp_sol)){

            Solution solution = extractMilpSolution(milp_sol, instance);

            if(solution.getStatus() != Status::FEASIBLE){
               log.warning("the solution found is infeasible.");
               std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
               return;
            }

            if(solver.isOptimal()){solution.setStatus(Status::OPTIMAL);}

            log.info("MILP solver algorithm completed. Final best known solution :");
            solution.print(instance);

            finalStatisticsMilp(0.0,
                                timer.getElapsedTime(),
                                solution.getObjectiveValue(instance),
                                solution.getStatus());
        }

        else{

            log.warning("the solver has not found an integer solution.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return; 
        }

    }




    /**
     * @brief runs the MILP resolution with a warm start solution
     */

    template <typename Backend>

    void genericMilpWithWarmStart(const Params &params, Backend &solver){

        // initialization
        Instance instance(params);
        printHeaderMilp(params, instance);
        solver.buildIntegerModel(instance);
        Timer timer;
        Logger log;

        timer.start();
        Solution solution = generateInitialSolution(params, instance);
        timer.stop();

        double preprocessing_time = timer.getElapsedTime();
        timer.reset();

        // case of a failed feasible primal solution
        if(!solution.isFeasible(instance)){
            log.warning("the computation of a warm start solution failed.");
        }

        else{
            std::vector<double> warm_start = computeWarmStart(solution);
            solver.setWarmStart(warm_start);
        }

        timer.start();
        solver.solveIntegerModel((params.time_limit - preprocessing_time));
        timer.stop();

        // feasibility test
        if(solver.isInFeasible()){
            log.warning("the problem is infeasible.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return;
        }

        // retrieving the model solution vector
        std::vector<double> milp_sol = solver.getSolution();

        // integrality test
        if(isInteger(milp_sol)){

            Solution solution = extractMilpSolution(milp_sol, instance);
            
            if(solution.getStatus() != Status::FEASIBLE){
               log.warning("the solution found is infeasible.");
               std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
               return;
            }

            if(solver.isOptimal()){solution.setStatus(Status::OPTIMAL);}

            log.info("MILP solver algorithm completed. Final best known solution :");
            solution.print(instance);

            finalStatisticsMilp(preprocessing_time,
                                (timer.getElapsedTime() + preprocessing_time),
                                solution.getObjectiveValue(instance),
                                solution.getStatus());
      
        }

        else{

            log.warning("the solver has not found an integer solution.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return; 
        }


    }



    /**
     * @brief runs the MILP resolution
     */
    void runMilpSolver(const Params &params);

}


#endif