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
 * @file Timer.hpp
 * @class Timer
 * @brief Management of time mesurement
 * @author Josué Tambwe
 * @date 27 May 2026
 */

 #pragma once 

 #include <chrono>


 namespace spp{

    class Timer{
        private : 

            bool running;
            std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
            double elapsed;

        public : 

            // constructor 
            Timer();

            // destructor
            ~Timer() = default;

            void start();
            void stop();
            void reset();

            // getters
            double getElapsedTime();
            bool isRunning();
    };

 }