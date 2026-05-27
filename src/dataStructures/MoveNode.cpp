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
 * @file MoveNode.cpp
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #include "dataStructures/MoveNode.hpp"

 namespace spp{

    // constructor 
    MoveNode::MoveNode(int id, float score): id(id), score(score){}

    // getters 
    int MoveNode::getId() const {return this->id;}

    float MoveNode::getScore() const {return this->score;}

    // operators overloading 
    bool MoveNode::operator<(const MoveNode &other) const{

        if(this->score != other.score){return this->score < other.score;}

        // case of equals heuristic scores
        return (this->id < other.id);
        
    }

    bool MoveNode::operator>(const MoveNode &other) const{

        if(this->score != other.score){return this->score > other.score;}

        // case of equals heuristic scores
        return (this->id > other.id);
        
    }


    void MoveNode::print() const{

        std::cout << " id : " << this->id 
                  << " - score : " << this->score << " \n";
    }
 }