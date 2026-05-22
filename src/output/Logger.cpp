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
 * @file Logger.cpp
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #include "output/Logger.hpp"

 namespace spp{


    void Logger::info(const std::string &message){

        std::cout << GREEN << "    [INFO]" << RESET 
                  << " : " << message <<  "\n" << std::endl;
    }

    void Logger::warning(const std::string &message){

        std::cout <<  YELLOW << " [WARNING]" << RESET 
                  << " : " << message << "\n" << std::endl;
    }



    void Logger::error(const std::string &message){
        std::cout << "\n";
        std::cerr << RED << "   [ERROR]" << RESET << " : " 
                  << message << "\n" << std::endl;
        std::exit(1);
    }

 }
