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
 * @file Solution.cpp
 * @author Josué Tambwe
 * @date 25 May 2026
 */

 #include "dataStructures/Solution.hpp"

 namespace spp{

    // constructor 
    Solution::Solution(size_t nb_vars, size_t nb_constraints)
     : solution_vector(nb_vars),
       consumed_resources(nb_constraints){

        this->status = Status::UNKNOWN;
    }

        
    // getters 
    size_t Solution::getNbVars() const {return solution_vector.getNbLogicalBits();}
    size_t Solution::getNbNonZeroVars() const {return solution_vector.getNbNonZeroBits();}
    size_t Solution::getNbVarWords() const {return solution_vector.getNbWords();}

    size_t Solution::getNbConstraints() const{return consumed_resources.getNbLogicalBits();}
    size_t Solution::getNbConsumedResources() const {return consumed_resources.getNbNonZeroBits();}
    size_t Solution::getNbConsumedResourceWords() const {return consumed_resources.getNbWords();}

    Status Solution::getStatus() const {return status;}

    // setters
    void Solution::activateVar(int index) {solution_vector.activate(index);}
    void Solution::deactivateVar(int index) {solution_vector.deactivate(index);}

    void Solution::consumeResource(int index) {consumed_resources.activate(index);}
    void Solution::freeResource(int index) {consumed_resources.deactivate(index);}


    void Solution::printVars() const {solution_vector.print();}
    void Solution::printConsumedResources() const {consumed_resources.print();}
    void Solution::print() const{}
 }