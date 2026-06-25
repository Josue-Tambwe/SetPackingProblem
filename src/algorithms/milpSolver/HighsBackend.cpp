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
 * @file HighsBackend.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */


#if HAS_HIGHS

#include "algorithms/milpSolver/HighsBackend.hpp"

namespace spp{


    void HighsBackend::solveIntegerModel(double time_limit){

        // disable verbose mode
        optimizer.setOptionValue("output_flag", false);
        
        // setting the time budget
        optimizer.setOptionValue("time_limit", time_limit);

        // solving the model
        optimizer.run();

    }




    void HighsBackend::buildIntegerModel(const spp::Instance &instance){

        variables.resize(instance.getNbVars());

        const std::vector<int> & profit_vector = instance.getProfitVector();

        // adding decision variables 
        for(size_t j = 0; j < instance.getNbVars(); j++){

            HighsInt col = optimizer.getNumCol();

                // - profit, lb, ub, nb coef in the matrix constraints, index, value
                // the profit is negative for maximization
                optimizer.addCol(-static_cast<double>(profit_vector[j]),
                                0.0,
                                1.0,
                                0,
                                nullptr,
                                nullptr);

                // integrality constraint
                optimizer.changeColIntegrality(col, HighsVarType::kInteger);

                variables[j] = col;
        }

        // constraints

        const std::vector<std::vector<int>>& constraint_matrix = instance.getConstraintMatrix();

        for(size_t i = 0; i < instance.getNbConstraints(); i++){

            size_t non_zero_count = constraint_matrix[i].size();
            std::vector<HighsInt> non_zero_var_indexes(non_zero_count);
            std::vector<double> coefficient(non_zero_count, 1.0);

            for(size_t j = 0; j < non_zero_count; j++){

                non_zero_var_indexes[j] = variables[constraint_matrix[i][j]];
            }

            // lb, ub, nb items in the constraint, ptr to var indexes, ptr to coefficients
            optimizer.addRow(0.0, 
                             1.0, 
                             non_zero_count, 
                             non_zero_var_indexes.data(), 
                             coefficient.data());

        }


    }





    void HighsBackend::solveContinuousModel(){optimizer.run();}



    void HighsBackend::buildContinuousModel(const spp::Instance &instance){

        variables.resize(instance.getNbVars());
        
        const std::vector<int> & profit_vector = instance.getProfitVector();

        // adding decision variables 
        for(size_t j = 0; j < instance.getNbVars(); j++){

            HighsInt col = optimizer.getNumCol();

                // profit, lb, ub, nb coef in the matrix constraints, index, value
                // the profit is negative for maximization
                optimizer.addCol(-static_cast<double>(profit_vector[j]),
                                0.0,
                                1.0,
                                0,
                                nullptr,
                                nullptr);


                variables[j] = col;
        }

        // constraints

        const std::vector<std::vector<int>>& constraint_matrix = instance.getConstraintMatrix();

        for(size_t i = 0; i < instance.getNbConstraints(); i++){

            size_t non_zero_count = constraint_matrix[i].size();
            std::vector<HighsInt> non_zero_var_indexes(non_zero_count);
            std::vector<double> coefficient(non_zero_count, 1.0);

            for(size_t j = 0; j < non_zero_count; j++){

                non_zero_var_indexes[j] = variables[constraint_matrix[i][j]];
            }

            // lb, ub, nb items in the constraint, ptr to var indexes, ptr to coefficients
            optimizer.addRow(0.0, 
                             1.0, 
                             non_zero_count, 
                             non_zero_var_indexes.data(), 
                             coefficient.data());

        }


    }




    void HighsBackend::addConstraints(spp::BaBNode &node){

        for(int index : node.getIndexesFixedToOne()){

            // var index, lb, ub
            optimizer.changeColBounds(index, 1.0, 1.0);
        }

        for(int index : node.getIndexesFixedToZero()){

            // var index, lb, ub
            optimizer.changeColBounds(index, 0.0, 0.0);
        }

    }




    void HighsBackend::resetLinearModel(spp::BaBNode &node){

        for(int index : node.getIndexesFixedToOne()){

            // var index, lb, ub
            optimizer.changeColBounds(index, 0.0, 1.0);
        }

        for(int index : node.getIndexesFixedToZero()){

            // var index, lb, ub
            optimizer.changeColBounds(index, 0.0, 1.0);
        }

    }



    void HighsBackend::setWarmStart(std::vector<double> &warm_start){

        HighsSolution initial_solution;

        // indicates that the initial solution will be feasible
        initial_solution.value_valid = true;

        initial_solution.col_value.resize(optimizer.getNumCol());

        // injecting the warm start values into the initial solution 
        for(size_t index = 0; index < variables.size(); index++){

            initial_solution.col_value[variables[index]] = warm_start[index];
        }

        // injecting the initial solution into the optimizer 
        optimizer.setSolution(initial_solution);
    }



    bool HighsBackend::isOptimal(){return optimizer.getModelStatus() == HighsModelStatus::kOptimal;}


    bool HighsBackend::isInFeasible() {return optimizer.getModelStatus() == HighsModelStatus::kInfeasible;}



    std::vector<double> HighsBackend::getSolution(){

        std::vector<double>  solution(variables.size());
        const HighsSolution& highs_sol = optimizer.getSolution();

        for(size_t index = 0; index < variables.size(); index++){

            solution[index] = highs_sol.col_value[variables[index]];
        }

        return solution;

    }


    double HighsBackend::getObjectiveValue(){return -optimizer.getInfo().objective_function_value;}


}

#endif