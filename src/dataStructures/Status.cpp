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
 * @file Status.cpp
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #include "dataStructures/Status.hpp"



 namespace spp{

    std::ostream& operator<<(std::ostream &output_stream, const Status &status){

        switch(status){

            case FEASIBLE : 
                output_stream << GREEN << "Feasible" << RESET;
                return output_stream;

            case OPTIMAL : 
                output_stream << BRIGHT_CYAN << "Optimal" << RESET ;
                return output_stream;

            case INFEASIBLE : 
                output_stream << RED << "Infeasible" << RESET ;
                return output_stream;
                
            default : 
                output_stream << RED << "Unkwon" << RESET;
                return output_stream;
        }
    }
 }