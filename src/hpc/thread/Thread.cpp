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
 * @file Thread.hpp
 * @author Josué Tambwe
 * @date 30 June 2026
 */

#include "hpc/thread/Thread.hpp"
 

namespace spp{


    int start_index(int id, int work_size, int nb_threads){

        int base = work_size / nb_threads;
        int rest = work_size % nb_threads;
        int start;

        if (id < rest) {start = id * (base + 1);} 

        else {start = rest * (base + 1) + (id - rest) * base;}

        return start;
    }

    
    int end_index(int id, int work_size, int nb_threads){

        int base = work_size / nb_threads;
        int rest = work_size % nb_threads;
        int block_size;

        if (id < rest) {block_size = base + 1;}

        else {block_size = base;}

        int end = start_index(id, work_size, nb_threads) + block_size - 1;

        if (end >= work_size) {end = work_size - 1;}

        return end; 
    }


    
    std::mt19937& getThreadLocalRng() {

        thread_local std::mt19937 gen(
            std::hash<std::thread::id>{}(std::this_thread::get_id())
        );

    return gen;

    }


}