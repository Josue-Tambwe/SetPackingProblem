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
 * @file HexalyBackend.hpp
 * @class HexalyBackend
 * @brief defines an API for the Hexaly backend
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#if HAS_HEXALY

#include "optimizer/hexalyoptimizer.h"
#include "dataStructures/Instance.hpp"
#include "dataStructures/BaBNode.hpp"
#include <vector>

namespace spp{

    class HexalyBackend{

        private : 
            hexaly::HexalyOptimizer optimizer;
            hexaly::HxModel model;
            std::vector<hexaly::HxExpression> variables;

        public :
            // constructor
            HexalyBackend();

            // destructor
            ~HexalyBackend() = default;

            void buildIntegerModel(const spp::Instance &instance); 
            void solveIntegerModel(double time_limit);
            void setWarmStart(std::vector<double> &warm_start); 


            bool isInFeasible();
            bool isOptimal();


            std::vector<double> getSolution();
            double getObjectiveValue();


    };
}


#endif