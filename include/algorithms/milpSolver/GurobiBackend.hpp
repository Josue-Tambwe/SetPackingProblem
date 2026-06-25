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
 * @file GurobiBackend.hpp
 * @class GurobiBackend
 * @brief defines an API for the Gurobi backend
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#if HAS_GUROBI

#include "gurobi_c++.h"
#include "dataStructures/Instance.hpp"
#include "dataStructures/BaBNode.hpp"
#include "algorithms/milpSolver/LPSolver.hpp"
#include <vector>
#include <memory>


namespace spp{

    class GurobiBackend : public LPSolver{

        private:

            std::unique_ptr<GRBEnv> env;
            std::unique_ptr<GRBModel> model;
            std::vector<GRBVar> variables;
            

        public:

            // constructor
            GurobiBackend();

            // destruction
             ~GurobiBackend() = default;
            

            void buildIntegerModel(const spp::Instance &instance) override; 
            void buildContinuousModel(const spp::Instance &instance) override;


            void solveContinuousModel() override; 
            void solveIntegerModel(double time_limit) override; 


            void addConstraints(spp::BaBNode &node) override;
            void resetLinearModel(spp::BaBNode &node) override;

            void setWarmStart(std::vector<double> &warm_start);


            bool isInFeasible() override;
            bool isOptimal() override;

            std::vector<double> getSolution() override;
            double getObjectiveValue() override;

    };

}


#endif 