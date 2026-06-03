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
 * @file DeterministicConstruction.cpp
 * @author Josué Tambwe
 * @date 26 May 2026
 */

 #include "algorithms/greedy/DeterministicConstruction.hpp"


 namespace spp{


    void initialization(std::vector<float> &scores,
                        std::unordered_set<int> &free_variables,
                        const spp::Instance &instance){

        const std::vector<int>& profit_vector = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        for(int var = 0; var < static_cast<int>(instance.getNbVars()); var++){

            // adding the index of the variable to the free variables set
            free_variables.insert(var);

            // computing the heuristic score of the variable
            float nb_resource_required = static_cast<float>(resource_requirements[var].getNbNonZeroBits());

            // The heuristic score : profit / nb resources required 
            scores[var] = profit_vector[var] / (nb_resource_required + epsilon);

        }

    }



    int findBestVariable(std::vector<float> &scores, 
                         std::unordered_set<int> &free_variables){

        int best_var = -1;
        float best_score = 0.0f;

        for(int var : free_variables){

            if(scores[var] >= best_score){best_score = scores[var]; best_var = var;}
        }

        return best_var;
    }




    void updateSolution(int best_var,
                        std::unordered_set<int> &free_variables,
                        spp::Solution &solution,
                        const spp::Instance &instance){

        // setting the best variable to 'one'
        solution.activateVar(best_var, instance);

        std::unordered_set<int> conflicting_vars = instance.getAllConflictingVarsIndexes(best_var);

        // removing those conflicting variables from the free variables set
        for(int var : conflicting_vars){

            free_variables.erase(var);
        }
                        
    }




    spp::Solution deterministicConstruction(const spp::Instance &instance){

        // initialization
        std::vector<float> scores(instance.getNbVars());
        std::unordered_set<int> free_variables;
        initialization(scores, free_variables, instance);

        spp::Solution solution(instance);
        spp::Logger log;

        while(!free_variables.empty()){

            int best_var = findBestVariable(scores, free_variables);

            if(best_var == -1){

                log.warning("The deterministic greedy heuristic failed!");
                solution.setStatus(spp::Status::INFEASIBLE);
                return solution;
            }

            /* setting to 'one' the best variable index and setting to 'zero' and removing from
             * the free variables set all variables in conflict with the best variable index
            */
            updateSolution(best_var, 
                           free_variables, 
                           solution, 
                           instance);

            // removing the best variable index from the free variable set
            free_variables.erase(best_var);
        }

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}
        else{solution.setStatus(Status::INFEASIBLE);}
        
        return solution;

    }

 }