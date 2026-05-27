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
 * @file Timer.cpp
 * @author Josué Tambwe
 * @date 27 May 2026
 */

 #include "dataStructures/Timer.hpp"


 namespace spp{

    // constructor
    Timer::Timer(): running(false), elapsed(0.0){}


    void Timer::start(){

        if (!running) { 
            running = true; 
            start_time = std::chrono::high_resolution_clock::now(); 
        }
    }



    void Timer::stop(){

        if (running) {
            elapsed += std::chrono::duration<double>( std::chrono::high_resolution_clock::now() - start_time ).count(); 
            running = false; 
        }
    }



    void Timer::reset(){running = false; elapsed = 0.0;}



    double Timer::getElapsedTime(){

        if (running) { 
            return elapsed + std::chrono::duration<double>( std::chrono::high_resolution_clock::now() - start_time ).count(); 
        }

        else{return elapsed;}   
    }



    bool Timer::isRunning(){
        return running;
    }


 }