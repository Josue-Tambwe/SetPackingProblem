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
 * @file GurobiBackend.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */


#if HAS_GUROBI

#include "algorithms/milpSolver/GurobiBackend.hpp"

namespace spp{

    // constructor

    GurobiBackend::GurobiBackend(){

        env = std::make_unique<GRBEnv>(true); 
        env->set(GRB_IntParam_OutputFlag, 0); // disable verbose mode
        env->start();
        model = std::make_unique<GRBModel>(*env);
    }



    
    void GurobiBackend::solveIntegerModel(double time_limit){

        model->set(GRB_DoubleParam_TimeLimit, time_limit);
        model->optimize();

    }



    void GurobiBackend::buildIntegerModel(const spp::Instance &instance){

        // initialization of decision variables vector
        variables.resize(instance.getNbVars());

        // decision variables
        for(size_t j = 0; j < instance.getNbVars(); j++){

            // lb, ub, obj coef, name
            variables[j] = model->addVar(0.0,
                                        1.0,
                                        0.0,
                                        GRB_BINARY,
                                        "x" + std::to_string(j));

        }

        const std::vector<int> & profit_vector = instance.getProfitVector();


        // ojective function

        GRBLinExpr z = 0;

        for(size_t j = 0; j < instance.getNbVars(); j++) {z += profit_vector[j] * variables[j];}

        model->setObjective(z, GRB_MAXIMIZE);


        // constraints

        const std::vector<std::vector<int>>& constraint_matrix = instance.getConstraintMatrix();

        for(size_t i = 0; i < instance.getNbConstraints(); i++){

            GRBLinExpr constraint = 0;

            for(int j : constraint_matrix[i]){constraint += variables[j];}

            model->addConstr(constraint <= 1);

        }

        model->update();

    }




    void GurobiBackend::solveContinuousModel(){model->optimize();}



    void GurobiBackend::buildContinuousModel(const spp::Instance &instance){

        // initialization of decision variables vector
        variables.resize(instance.getNbVars());

        // decision variables
        for(size_t j = 0; j < instance.getNbVars(); j++){

            // lb, ub, obj coef, name
            variables[j] = model->addVar(0.0,
                                        1.0,
                                        0.0,
                                        GRB_CONTINUOUS,
                                        "x" + std::to_string(j));

        }

        const std::vector<int> & profit_vector = instance.getProfitVector();


        // ojective function

        GRBLinExpr z = 0;

        for(size_t j = 0; j < instance.getNbVars(); j++) {z += profit_vector[j] * variables[j];}

        model->setObjective(z, GRB_MAXIMIZE);


        // constraints

        const std::vector<std::vector<int>>& constraint_matrix = instance.getConstraintMatrix();

        for(size_t i = 0; i < instance.getNbConstraints(); i++){

            GRBLinExpr constraint = 0;

            for(int j : constraint_matrix[i]){constraint += variables[j];}

            model->addConstr(constraint <= 1);

        }

        model->update();

    }



    void GurobiBackend::addConstraints(spp::BaBNode &node){

        for(int index : node.getIndexesFixedToOne()){

            variables[index].set(GRB_DoubleAttr_LB, 1.0);
            variables[index].set(GRB_DoubleAttr_UB, 1.0);
        }

        for(int index : node.getIndexesFixedToZero()){

            variables[index].set(GRB_DoubleAttr_LB, 0.0);
            variables[index].set(GRB_DoubleAttr_UB, 0.0);
        }

        model->update();

    }



    void GurobiBackend::resetLinearModel(spp::BaBNode &node){

        for(int index : node.getIndexesFixedToOne()){

            variables[index].set(GRB_DoubleAttr_LB, 0.0);
            variables[index].set(GRB_DoubleAttr_UB, 1.0);
        }

        for(int index : node.getIndexesFixedToZero()){

            variables[index].set(GRB_DoubleAttr_LB, 0.0);
            variables[index].set(GRB_DoubleAttr_UB, 1.0);
        }

        model->update();

    }



    void GurobiBackend::setWarmStart(std::vector<double> &warm_start){

        for(size_t index = 0; index < warm_start.size(); index++){

            variables[index].set(GRB_DoubleAttr_Start, warm_start[index]);
        }

        model->update();
    }



    bool GurobiBackend::isInFeasible(){return model->get(GRB_IntAttr_Status) == GRB_INFEASIBLE;}

    bool GurobiBackend::isOptimal(){return model->get(GRB_IntAttr_Status) == GRB_OPTIMAL;}



    std::vector<double> GurobiBackend::getSolution(){

        std::vector<double> solution(variables.size());

        for(size_t i = 0; i < solution.size(); i++){solution[i] = variables[i].get(GRB_DoubleAttr_X);}

        return solution;
    }


    double GurobiBackend::getObjectiveValue(){return model->get(GRB_DoubleAttr_ObjVal);}
}

#endif