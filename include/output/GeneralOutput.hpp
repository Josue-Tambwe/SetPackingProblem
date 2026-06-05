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
 * @file GeneralOutput.hpp 
 * @author Josué Tambwe
 * @date 6 June 2026
 */

 #pragma once

 #include <iostream>
 #include <string>
 #include <iomanip>

 namespace spp{

    void printHeader();

    void printUsage();

    void printRequiredOptions();

    void printOptionalOptions();

    void printExampleRun();

    void printHelp();

 }