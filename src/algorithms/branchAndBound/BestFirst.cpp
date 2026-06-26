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
 * @file BestFirst.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#include "algorithms/branchAndBound/BestFirst.hpp"

namespace spp{


    // getters 

    double BestFirst::getHighestDualBound() const {

        if(!node_list.empty()){
            
            return node_list.top().getDualBound();
        }

        return 0.0;
    }


    spp::BaBNode BestFirst::pop(){

        spp::BaBNode node = node_list.top();
        node_list.pop();
        return node;
    }


    bool BestFirst::isEmpty() const {return node_list.empty();}


    size_t BestFirst::getSize() const {return node_list.size();}



    // Setter

    void BestFirst::add(spp::BaBNode node){node_list.push(node);}



    void BestFirst::print() const{

        int counter = 0;
        std::priority_queue<spp::BaBNode> copy = node_list;

        while(!copy.empty()){

            counter += 1;
            std::cout << " node id : " << counter << " --- ";
            spp::BaBNode node = copy.top();
            node.print();
            copy.pop();
        }
    }


}