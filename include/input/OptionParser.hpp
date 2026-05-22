/*------------------------------------------------------------------------------
 * SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
 *              and efficient BitVector techniques.
 *------------------------------------------------------------------------------
 * Copyright (c) 2025 Josué Tambwe
 *               Nantes Université, France
 *------------------------------------------------------------------------------
 * This software is released under the MIT License.
 * See the LICENSE file for more details.
 *------------------------------------------------------------------------------
 */


/** 
 * @file OptionParser.hpp
 * @brief parses the options and flags defines by the user
 * @author Josué Tambwe
 * @date 23 May 2026
 */

 #pragma once

 #include "configuration/GeneralConfig.hpp"
 #include "configuration/OutputConfig.hpp"
 #include "dataStructures/Parameters.hpp"
 #include "dataStructures/Algorithm.hpp"
 #include "output/Logger.hpp"
 #include <iostream>
 #include <string>

 namespace spp{


    /**
    * @brief checks that all required options have been given by the user
    */
    void checkRequiredOptions(int argc, char** argv);


    /**
    * @brief checks that no unsupported options are given for the greedy algorithm
    */
    void unsupportedOptionsGreedy(int argc, char** argv);



    /**
    * @brief performs the parsing of all options given by the user
    */
    Params parseOptions(int argc, char** argv);

 }