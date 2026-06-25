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
 * @file LPSolver.hpp
 * @class LPSolver
 * @brief defines a generic class for LP solvers backend
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/BaBNode.hpp"
#include <vector>

namespace spp{

    class LPSolver{

        public:

            // destructor
            virtual ~LPSolver() = default;

            virtual void buildContinuousModel(const spp::Instance &instance) = 0;
            virtual void buildIntegerModel(const spp::Instance &instance) = 0;
            virtual void solveContinuousModel() = 0;
            virtual void solveIntegerModel(double time_limit) = 0;


            virtual void addConstraints(spp::BaBNode &node) = 0;
            virtual void resetLinearModel(spp::BaBNode &node) = 0;


            virtual bool isInFeasible() = 0;
            virtual bool isOptimal() = 0;
            

            virtual std::vector<double> getSolution() = 0;
            virtual double getObjectiveValue() = 0;


    };
}