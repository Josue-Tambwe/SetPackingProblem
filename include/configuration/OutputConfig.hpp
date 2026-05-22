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
 * @file OutputConfig.hpp
 * @brief Defines output format and manipulation in the standard output strean
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #pragma once

 #include <iostream>
 #include <iomanip>

 namespace spp{

    static constexpr const char* GREEN  = "\033[32m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BRIGHT_YELLOW  = "\033[93m";
    static constexpr const char* RED    = "\033[31m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN    = "\033[36m";
    static constexpr const char* BRIGHT_CYAN    = "\033[96m";
    static constexpr const char* RESET  = "\033[0m";
    
 }