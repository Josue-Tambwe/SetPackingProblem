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

#include "dataStructures/TabuList.hpp"

namespace spp{

    // ------------------ TabuMove -------------------

    // constructors

    TabuMove::TabuMove(){this->type = TabuMoveType::Unknown;}


    TabuMove::TabuMove(TabuMoveType t, std::vector<size_t> idx) : type(t), indexes(idx) {}

    // getters

    TabuMoveType TabuMove::getMoveType() const {return this->type;}


    std::vector<size_t> TabuMove::getIndexes() const {return this->indexes;}


    bool TabuMove::equals(const TabuMove &other) const {

        if(this->type == TabuMoveType::ZeroOne && other.type == TabuMoveType::ZeroOne){

            return (this->indexes[0] == other.indexes[0]);

        }

        if(this->type == TabuMoveType::OneOne && other.type == TabuMoveType::OneOne){

            return (this->indexes[0] == other.indexes[0] && 
                    this->indexes[1] == other.indexes[1]);

        }

        if((this->type == TabuMoveType::OneTwo && other.type == TabuMoveType::OneTwo) ||
                (this->type == TabuMoveType::TwoOne && other.type == TabuMoveType::TwoOne)){

            return (this->indexes[0] == other.indexes[0] && 
                    this->indexes[1] == other.indexes[1] && 
                    this->indexes[2] == other.indexes[2]);

        }

        return false;

    }



    void TabuMove::print() const{

        std::cout << " type : ";

        if(this->type == TabuMoveType::ZeroOne){

            std::cout << "zero-one"
                << " to activate : " << this->indexes[0] << "\n\n";
        }

        else if(this->type == TabuMoveType::OneOne){

            std::cout << "one-one"
                << " to deactivate : " << this->indexes[0] << " --- "
                << " to activate : " << this->indexes[1] << "\n\n";

        }

        else if(this->type == TabuMoveType::OneTwo){

            std::cout << "one-two"
                << " to deactivate : " << this->indexes[0] << " --- "
                << " to activate : " << this->indexes[1] << " - " << this->indexes[2] << "\n\n";

        }

        else if(this->type == TabuMoveType::TwoOne){

            std::cout << "two-one"
                << " to deactivate : " << this->indexes[0]  << " - " << this->indexes[1] << " --- "
                << " to activate : " << this->indexes[2] << "\n\n";

        }

        else{

            Logger log;
            log.error("Empty Tabu move");
        }
    }





    // ------------------ TabuList -------------------

    bool TabuList::contains(const TabuMove &move) const{

        // copy of the current tabu list
        std::queue<TabuMove> copied_list = this->list;

        while(!copied_list.empty()){

            TabuMove current_move = copied_list.front();
            
            if(current_move.equals(move)){return true;}

            copied_list.pop();
        }

        return false;
    }



    void TabuList::addTabuMove(const TabuMove &move){

        // push the new tabu move at the end of the FIFO list
        list.push(move);

        // if the tabu list exceeds the allowed tenure then the oldest move is removed
        if(this->list.size() > this->tabu_tenure){list.pop();}
    }


    void TabuList::print() const{

        // copy of the current tabu list
        std::queue<TabuMove> copied_list = this->list;

        size_t counter = 0;

        while(!copied_list.empty()){

            counter += 1;

            TabuMove current_move = copied_list.front();

            std::cout << " move " << counter << " ";
            current_move.print();

            copied_list.pop();
        }

    }


}