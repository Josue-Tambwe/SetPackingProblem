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
 * @file Logger.hpp
 * @class Logger
 * @brief Management of logging messages 
 * @author Josué Tambwe
 * @date 22 May 2026
 */



 #pragma once 

 #include "configuration/OutputConfig.hpp"
 #include <iostream>
 #include <string>

 namespace spp{

    class Logger{

        public : 

            // constructor
            Logger() = default;

            // destructor
            ~Logger() = default;

            void info(const std::string &message);
            void warning(const std::string &message);
            void error(const std::string &message);
    };
 }