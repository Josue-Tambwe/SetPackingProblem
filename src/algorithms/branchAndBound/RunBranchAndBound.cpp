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
 * @file RunBranchAndBound.cpp
 * @author Josué Tambwe
 * @date 26 June 2026
 */


#if USE_BRANCH_AND_BOUND


#include "algorithms/branchAndBound/RunBranchAndBound.hpp"

namespace spp{

    Solution initializePrimalSolution(const Params &params, 
                                      const Instance &instance){

        // construction
        Solution solution = deterministicConstruction(instance);

        // local search
        variableNeighborhoodDescent(params.use_intensification,
                                    params, 
                                    solution, 
                                    instance);

        return solution;
    }



    double setInitialPrimalBound(Solution &primal_solution,
                                 const Instance &instance){

        if(primal_solution.getStatus() != Status::FEASIBLE
           && primal_solution.getStatus() != Status::OPTIMAL){

            return 0.0;
        }

        return static_cast<double>(primal_solution.getObjectiveValue(instance));
    }



    std::unique_ptr<LPSolver> setLPSolver(const Params &params){

        #if HAS_HIGHS
        if(params.milp_solver == 'h'){return std::make_unique<HighsBackend>();}
        #endif
        
        #if HAS_GUROBI
        if(params.milp_solver == 'g'){return std::make_unique<GurobiBackend>();}
        #endif

        throw std::runtime_error("No LP solver enabled");

    }



    std::unique_ptr<LPSolver> buildLinearModel(const Params &params,
                                               const Instance &instance){

        std::unique_ptr<LPSolver> solver = setLPSolver(params);
        solver->buildContinuousModel(instance);
        return solver;
    }




    Solution updateSolution(std::vector<double> &sol, const Instance &instance){

        Solution primal_solution(instance);

        for(int j = 0; j < static_cast<int>(sol.size()); j++){

            if(std::abs(1.0 - sol[j]) < tolerance){

                primal_solution.activateVar(j, instance);
            }
        }

        // feasibility certification

        if(primal_solution.isFeasible(instance)){

            primal_solution.setStatus(Status::FEASIBLE);
        }
        else{primal_solution.setStatus(Status::INFEASIBLE);}

        return primal_solution;
    }




    void branchAndBoundBestFirst(const Params &params){

        // initialization
        const Instance instance(params);
        printHeaderBaB(params, instance);

        Timer timer = Timer();
        Logger log;
        timer.start();

        // primal solution (greedy construction + VND local search)
        Solution primal_solution = initializePrimalSolution(params, instance);

        double primal_bound = setInitialPrimalBound(primal_solution, instance);
        double preprocessing_time = timer.getElapsedTime();
        double preprocessing_obj_value = primal_bound;

        // initialization of the LP solver
        std::unique_ptr<LPSolver> solver = buildLinearModel(params, instance);
        solver->solveContinuousModel();

        // infeasibility test
        if(!solver->isOptimal()){

            log.warning("the problem is infeasible.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return;
        }

        // initialization of the dual bound
        double dual_bound = solver->getObjectiveValue();

        // initialization of the list of open nodes
        BestFirst open_nodes;

        // initialization of the root node
        open_nodes.add(BaBNode(dual_bound));

        printHeaderLineBaB();

        size_t processed_nodes = 0;
        size_t nodes_fathomed_by_infeasibility = 0;
        size_t nodes_fathomed_by_dominance = 0;
        size_t nodes_fathomed_by_optimality = 0;

        while(!open_nodes.isEmpty() &&
              !stoppingCriteriaBestFirst(primal_bound, dual_bound, timer.getElapsedTime(), params)){

            BaBNode current_node = open_nodes.pop();

            processed_nodes += 1;

            solver->addConstraints(current_node);
            solver->solveContinuousModel();

            if(solver->isInFeasible()){

                nodes_fathomed_by_infeasibility += 1;

                // undoing variables fixation
                solver->resetLinearModel(current_node);
                continue;
            }

            std::vector<double> relaxed_solution = solver->getSolution();

            // dominance test
            if(solver->getObjectiveValue() < primal_bound){

                nodes_fathomed_by_dominance += 1;

                // undoing variables fixation
                solver->resetLinearModel(current_node); 
                continue;
            }

            // integrality test
            if(isInteger(relaxed_solution)){

                nodes_fathomed_by_optimality += 1;
                
                if(solver->getObjectiveValue() > primal_bound){

                    primal_bound = solver->getObjectiveValue();
                    primal_solution = updateSolution(relaxed_solution, instance);
                }

                // undoing variables fixation
                solver->resetLinearModel(current_node);

                continue;
            }

            // branching

            int index = branchingVariableIndex(relaxed_solution, params);

            // x_ij = 0
            BaBNode left_branch_node(solver->getObjectiveValue(), current_node);
            left_branch_node.fixToZero(index);

            // x_ij = 1
            BaBNode right_branch_node(solver->getObjectiveValue(), current_node);
            right_branch_node.fixToOne(index);

            // adding nodes into the open nodes list
            open_nodes.add(left_branch_node);
            open_nodes.add(right_branch_node);

            // upadting the global dual bound
            if(!open_nodes.isEmpty()){

               dual_bound = open_nodes.getHighestDualBound();
            }
            else{dual_bound = primal_bound;}

            if(processed_nodes % 1000 == 0 || processed_nodes < 10){

                printBaBIteration(timer.getElapsedTime(),
                                  processed_nodes,
                                  open_nodes.getSize(),
                                  dual_bound,
                                  primal_bound);
            }

            // undoing variables fixation
            solver->resetLinearModel(current_node);  
        }

        std::cout << "\n";
        log.info("Branch and Bound algorithm completed. Final best known solution :");

        // update of solution status
        if((std::abs(dual_bound - primal_bound) / primal_bound) <= tolerance 
            || open_nodes.isEmpty()){

                primal_solution.setStatus(Status::OPTIMAL);
        }

        // display of the final solution
        primal_solution.print(instance);

        finalStatisticsBaB(preprocessing_time,
                           preprocessing_obj_value,
                           dual_bound,
                           primal_bound,
                           timer.getElapsedTime(),
                           primal_solution.getStatus(),
                           processed_nodes,
                           open_nodes.getSize(),
                           nodes_fathomed_by_optimality,
                           nodes_fathomed_by_dominance,
                           nodes_fathomed_by_infeasibility,
                           params);




    }




    void branchAndBoundDepthFirst(const Params &params){

        // initialization
        const Instance instance(params);
        printHeaderBaB(params, instance);

        Timer timer = Timer();
        Logger log;
        timer.start();

        // primal solution (greedy construction + VND local search)
        Solution primal_solution = initializePrimalSolution(params, instance);

        double primal_bound = setInitialPrimalBound(primal_solution, instance);
        double preprocessing_time = timer.getElapsedTime();
        double preprocessing_obj_value = primal_bound;

        // initialization of the LP solver
        std::unique_ptr<LPSolver> solver = buildLinearModel(params, instance);
        solver->solveContinuousModel();

        // infeasibility test
        if(!solver->isOptimal()){

            log.warning("the problem is infeasible.");
            std::cout << " \n Processing time : " << timer.getElapsedTime() << " (s) \n\n" << std::endl;
            return;
        }

        // initialization of the dual bound
        double dual_bound = solver->getObjectiveValue();

        // initialization of the list of open nodes
        DepthFirst open_nodes;

        // initialization of the root node
        open_nodes.add(BaBNode(dual_bound));

        printHeaderLineBaB();

        size_t processed_nodes = 0;
        size_t nodes_fathomed_by_infeasibility = 0;
        size_t nodes_fathomed_by_dominance = 0;
        size_t nodes_fathomed_by_optimality = 0;

        while(!open_nodes.isEmpty() &&
              !stoppingCriteriaDepthFirst(timer.getElapsedTime(), params)){

            BaBNode current_node = open_nodes.pop();

            processed_nodes += 1;

            solver->addConstraints(current_node);
            solver->solveContinuousModel();

            if(solver->isInFeasible()){

                nodes_fathomed_by_infeasibility += 1;

                // undoing variables fixation
                solver->resetLinearModel(current_node);
                continue;
            }

            std::vector<double> relaxed_solution = solver->getSolution();

            // dominance test
            if(solver->getObjectiveValue() < primal_bound){

                nodes_fathomed_by_dominance += 1;

                // undoing variables fixation
                solver->resetLinearModel(current_node); 
                continue;
            }

            // integrality test
            if(isInteger(relaxed_solution)){

                nodes_fathomed_by_optimality += 1;
                
                if(solver->getObjectiveValue() > primal_bound){

                    primal_bound = solver->getObjectiveValue();
                    primal_solution = updateSolution(relaxed_solution, instance);
                }

                // undoing variables fixation
                solver->resetLinearModel(current_node);

                continue;
            }

            // branching

            int index = branchingVariableIndex(relaxed_solution, params);

            // x_ij = 0
            BaBNode left_branch_node(solver->getObjectiveValue(), current_node);
            left_branch_node.fixToZero(index);

            // x_ij = 1
            BaBNode right_branch_node(solver->getObjectiveValue(), current_node);
            right_branch_node.fixToOne(index);

            // adding nodes into the open nodes list
            open_nodes.add(left_branch_node);
            open_nodes.add(right_branch_node);

            // upadting the global dual bound
            if(!open_nodes.isEmpty()){

               dual_bound = open_nodes.getHighestDualBound();
            }
            else{dual_bound = primal_bound;}

            if(processed_nodes % 1000 == 0 || processed_nodes < 10){

                printBaBIteration(timer.getElapsedTime(),
                                  processed_nodes,
                                  open_nodes.getSize(),
                                  dual_bound,
                                  primal_bound);
            }

            // undoing variables fixation
            solver->resetLinearModel(current_node);  
        }

        std::cout << "\n";
        log.info("Branch and Bound algorithm completed. Final best known solution :");

        // update of solution status
        if((std::abs(dual_bound - primal_bound) / primal_bound) <= tolerance 
            || open_nodes.isEmpty()){

                primal_solution.setStatus(Status::OPTIMAL);
        }

        // upadting the global dual bound
        if(!open_nodes.isEmpty()){

            dual_bound = open_nodes.getHighestDualBound();
        }
        else{dual_bound = primal_bound;}

        // display of the final solution
        primal_solution.print(instance);

        finalStatisticsBaB(preprocessing_time,
                           preprocessing_obj_value,
                           dual_bound,
                           primal_bound,
                           timer.getElapsedTime(),
                           primal_solution.getStatus(),
                           processed_nodes,
                           open_nodes.getSize(),
                           nodes_fathomed_by_optimality,
                           nodes_fathomed_by_dominance,
                           nodes_fathomed_by_infeasibility,
                           params);




    }



    void runBaB(const Params &params){
        
        printHeader();

        if(params.exploration_strategy == 'b'){

            branchAndBoundBestFirst(params);
        }

        else{branchAndBoundDepthFirst(params);}

    }


}

#endif

