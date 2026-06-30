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
 * @file RandomizedConstruction.cpp
 * @author Josué Tambwe
 * @date 29 June 2026
 */

#include "algorithms/greedy/RandomizedConstruction.hpp"

namespace spp{


    std::vector<float> computeScore(const Instance &instance){

        std::vector<float> scores(instance.getNbVars());
        const std::vector<int>& profit_vector = instance.getProfitVector();
        const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

        for(size_t var = 0; var < instance.getNbVars(); var++){

            float nb_resource_required = static_cast<float>(resource_requirements[var].getNbNonZeroBits());

            scores[var] = profit_vector[var] / (nb_resource_required + epsilon);
        }

        return scores;
    }




    void findMinAndMaxScores(float &min_score, 
                             float &max_score,
                             int &max_score_index, 
                             std::vector<float> &scores, 
                             std::unordered_set<int> &free_variables){

        min_score = std::numeric_limits<float>::max();
        max_score = 0.0f;
        max_score_index = -1;

        for(int var : free_variables){

            if(scores[var] < min_score){min_score = scores[var];}

            if(scores[var] > max_score){max_score = scores[var]; max_score_index = var;}
        }

    }




    std::vector<int> buildRestrictedCandidatesList(float min_score, 
                                                   float max_score,
                                                   float alpha, 
                                                   std::vector<float> &scores, 
                                                   std::unordered_set<int> &free_variables){
        
        // the restricted candidates list
        std::vector<int> RCL;
        RCL.resize(free_variables.size());

        float threshold = min_score + (alpha * (max_score - min_score));
        size_t counter = 0;

        for(int var : free_variables){

            if(scores[var] >= threshold){RCL[counter] = var; counter += 1;}
        }

        RCL.resize(counter);

        return RCL;
    }




    int selectVariableRandomdlyInRCL(int max_score_index, 
                                     float alpha, 
                                     std::vector<int> &RCL){

        if(alpha == 1.0f || RCL.size() == 0){return max_score_index;}

        // initialization of the thread random number generator
        auto& rng = getThreadLocalRng();

        // initialization of the uniform (discrete) distribution on variables in the RCL
        std::uniform_int_distribution<int> dist(0, (RCL.size() - 1));

        return RCL[dist(rng)];

    }




    void updateSolutionRandomizedConstruction(int selected_var,
                                              std::unordered_set<int> &free_variables,
                                              spp::Solution &solution,
                                              const spp::Instance &instance){

        // setting the selected variable to 'one'
        solution.activateVar(selected_var, instance);

        std::vector<int> conflicting_vars = instance.getAllConflictingVarsIndexes(selected_var);

        // removing those conflicting variables from the free variables set
        for(int var : conflicting_vars){free_variables.erase(var);}                
    }




    spp::Solution randomizedConstruction(float alpha, const spp::Instance &instance){

        // initialization
        std::vector<float> scores = computeScore(instance);
        std::unordered_set<int> free_variables;
        // adding the index of the variable to the free variables set
        for(int var = 0; var < static_cast<int>(instance.getNbVars()); var++){free_variables.insert(var);}

        spp::Solution solution(instance);
        spp::Logger log;

        while(!free_variables.empty()){

            float min_score;
            float max_score;
            int max_score_index;

            findMinAndMaxScores(min_score, 
                                max_score,
                                max_score_index, 
                                scores, 
                                free_variables);

            // building the Restricted Candidates List (RCL)
            std::vector<int> RCL = buildRestrictedCandidatesList(min_score, 
                                                                 max_score,
                                                                 alpha, 
                                                                 scores, 
                                                                 free_variables);

            int selected_var = selectVariableRandomdlyInRCL(max_score_index, 
                                                            alpha, 
                                                            RCL);

            if(selected_var == -1){

                log.warning("The randomized greedy construction heuristic failed!");
                solution.setStatus(spp::Status::INFEASIBLE);
                return solution;
            }

            /* setting to 'one' the selected variable index and setting to 'zero' and removing from
             * the free variables set all variables in conflict with the selected variable index
            */
            updateSolutionRandomizedConstruction(selected_var, 
                                                 free_variables, 
                                                 solution, 
                                                 instance);

            // removing the selected variable index from the free variable set
            free_variables.erase(selected_var);
        }

        // feasibility certification
        if(solution.isFeasible(instance)){solution.setStatus(Status::FEASIBLE);}
        
        else{solution.setStatus(Status::INFEASIBLE);}
        
        return solution;
    }


}
