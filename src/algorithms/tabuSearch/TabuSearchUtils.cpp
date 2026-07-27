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
 * @file TabuSearchUtils.cpp
 * @author Josué Tambwe
 * @date 27 July 2026
 */


#include "algorithms/tabuSearch/TabuSearchUtils.hpp"


namespace spp{


    void updateTabuList(const TabuMove &best_move, TabuList &tabu_list){

        if(best_move.getMoveType() != TabuMoveType::UnknownMove){

            tabu_list.addTabuMove(best_move);
        }
    }



    bool stoppingCriteriaTabuSearch(double current_time, 
                                    size_t current_iteration, 
                                    const Params &params){

        if(params.use_time_limit && !params.use_max_iterations){return current_time >= params.time_limit;}

        if(!params.use_time_limit && params.use_max_iterations){return current_iteration >= params.nb_max_iterations;}

        else{
            return (current_iteration >= params.nb_max_iterations) || (current_time >= params.time_limit);
        }

    }

}
