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
 * @file GreedyOutput.cpp 
 * @author Josué Tambwe
 * @date 6 June 2026
 */

 
#include "output/GreedyOutput.hpp"

namespace spp{


    void printHeaderGreedy(const Params &params, 
                           const Instance &instance){

        const InstanceStatistics& stats = instance.getStatistics();

        // line 1
        std::cout << YELLOW;
        std::cout << "  " <<  "Instance statistics" << std::string(17, ' ') 
                << "Algorithm" << std::string(15, ' ')
                << "Hardware" << std::string(18, ' ')
                << "Settings" << std::string(22, ' ')
                << "Flags"  << RESET << "\n";

        // line 2
        std::cout << "  "  << std::string(19, '-') << std::string(17, ' ') 
                << std::string(9, '-') << std::string(15, ' ')
                << std::string(8, '-') << std::string(18, ' ')
                << std::string(8, '-') << std::string(22, ' ')
                << std::string(5, '-')  << "\n";


                 
        
    }
}