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
 * @file RunMilpSolver.cpp
 * @author Josué Tambwe
 * @date 27 June 2026
 */


#if USE_MILP


#include "algorithms/milpSolver/RunMilpSolver.hpp"

namespace spp{


    Solution generateInitialSolution(const Params &params, const Instance &instance){

        // construction
        Solution solution = deterministicConstruction(instance);

        // local search
        variableNeighborhoodDescent(params.use_intensification,
                                    params, 
                                    solution, 
                                    instance);

        return solution;
    }




    std::vector<double> computeWarmStart(Solution &solution){

        std::vector<double> warm_start(solution.getNbVars(), 0.0);
        std::vector<int> non_zero_indexes =  solution.getNonZeroVarsIndexes();

        for(int index : non_zero_indexes){warm_start[index] = 1.0;}

        return warm_start;
    }



    Solution extractMilpSolution(std::vector<double> &milp_sol, const Instance &instance){

        Solution solution(instance);

        for(int j = 0; j < static_cast<int>(milp_sol.size()); j++){

            if(std::abs(1.0 - milp_sol[j]) < tolerance){solution.activateVar(j, instance);}
        }

        // feasibility certification

        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}

        else{solution.setStatus(Status::INFEASIBLE);}

        return solution;

    }




    void runMilpSolver(const Params &params){

        printHeader();

        if(params.milp_solver == 'g'){
            #if HAS_GUROBI
            GurobiBackend gurobi;
            if(params.warm_start){genericMilpWithWarmStart(params, gurobi);}
            else{genericMilpWithoutWarmStart(params, gurobi);}
            #endif
            
        }

        else if(params.milp_solver == 'h'){
            #if HAS_HIGHS
            HighsBackend highs;
            if(params.warm_start){genericMilpWithWarmStart(params, highs);}
            else{genericMilpWithoutWarmStart(params, highs);}
            #endif
        
        }

        else{
            #if HAS_HEXALY
            HexalyBackend hexaly;
            if(params.warm_start){genericMilpWithWarmStart(params, hexaly);}
            else{genericMilpWithoutWarmStart(params, hexaly);}
            #endif
        }

    }

}


#endif