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
 * @file TabuList.cpp
 * @author Josué Tambwe
 * @date 25 July 2026
 */

#include "algorithms/tabuSearch/TabuSearch.hpp"

namespace spp{


    TabuMove findBestMove(TabuList &tabu_list,
                          Solution &solution,
                          const Params &params,
                          const Instance &instance){

        // initialization
        TabuMove best_move;
        //const std::vector<int>& profit = instance.getProfitVector();
        //std::int64_t best_improvement = 0;

        //size_t first_index_to_activate = -1;
        //size_t second_index_to_activate = -1;

        //size_t first_index_to_deactivate = -1;
        //size_t second_index_to_deactivate = -1;


        // 0-1 neighborhood

        return best_move;




    }
}