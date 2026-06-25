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
 * @file HexalyBackend.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */


#if HAS_HEXALY

#include "algorithms/milpSolver/HexalyBackend.hpp"

namespace spp{

    // constructor
    HexalyBackend::HexalyBackend():
        optimizer(),
        model(optimizer.getModel())
    {
            // disable verbose mode
            optimizer.getParam().setVerbosity(0);
    }





    void HexalyBackend::solveIntegerModel(double time_limit){

        // setting the time limit
        optimizer.getParam().setTimeLimit(time_limit);

        // solving the model
        optimizer.solve();
    }



    void HexalyBackend::buildIntegerModel(const spp::Instance &instance){

        // initialization of decision variables vector
        variables.resize(instance.getNbVars());

        for(size_t j = 0; j < variables.size(); j++){

            // adding binary variables  to the model
            variables[j] = model.boolVar();
        }

        // objective function
        hexaly::HxExpression obj = model.sum();

        const std::vector<int> & profit_vector = instance.getProfitVector();

        for(size_t j = 0; j < variables.size(); j++){

            hexaly::HxExpression expr = profit_vector[j] * variables[j];
            obj.addOperand(expr);
        }

        // adding the objective expression to the model
        model.maximize(obj);


        // constraints 

        const std::vector<std::vector<int>>& constraint_matrix = instance.getConstraintMatrix();

        for(size_t i = 0; i < instance.getNbConstraints(); i++){

            hexaly::HxExpression constraint = model.sum();

            for(int j : constraint_matrix[i]){

                hexaly::HxExpression constraint_expr = variables[j];
                constraint.addOperand(constraint_expr);

            }
            // adding the constraint expression to the model
            model.constraint(constraint <= 1);
        }

        model.close();
    }




    void HexalyBackend::setWarmStart(std::vector<double> &warm_start){

        for(size_t index = 0; index < warm_start.size(); index++){

            hexaly::hxint value;
            
            if(warm_start[index] > 0.5){value = 1;}
            else{value = 0;}

            variables[index].setValue(value);

        }

    }



    bool HexalyBackend::isInFeasible(){return optimizer.getSolution().getStatus() == hexaly::SS_Infeasible;}


    bool HexalyBackend::isOptimal(){return optimizer.getSolution().getStatus() == hexaly::SS_Optimal;}



    std::vector<double> HexalyBackend::getSolution(){

        std::vector<double> solution(variables.size());

        for(size_t index = 0; index < variables.size(); index++){

            solution[index] = variables[index].getValue();
        }

        return solution;
    }




    double HexalyBackend::getObjectiveValue(){

        // 0 is the index of the objective : first objective function in multi-objective optimization
        return model.getObjective(0).getValue();
    }

}


#endif