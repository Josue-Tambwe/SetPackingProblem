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
 * @file HighsBackend.hpp
 * @class HighsBackend
 * @brief defines an API for the Highs backend
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#if HAS_HIGHS

#include "Highs.h"
#include "dataStructures/Instance.hpp"
#include "dataStructures/BaBNode.hpp"
#include "algorithms/milpSolver/LPSolver.hpp"
#include <vector>


namespace spp{

    class HighsBackend : public LPSolver{

        private : 

            Highs optimizer;
            std::vector<HighsInt> variables;


        public : 

            // constructor
            HighsBackend(){optimizer.setOptionValue("log_to_console", false);};

            // destructor 
            ~HighsBackend() = default;

            void solveContinuousModel() override;
            void solveIntegerModel(double time_limit) override; 


            void buildContinuousModel(const spp::Instance &instance) override;
            void buildIntegerModel(const spp::Instance &instance) override;



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