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
 * @file DepthFirst.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#include "algorithms/branchAndBound/DepthFirst.hpp"


namespace spp{

    // getters

    double DepthFirst::getHighestDualBound() const {

        if(node_list.empty()){return 0.0;}

        double dual_bound = node_list.top().getDualBound();
        std::stack<spp::BaBNode> copy = node_list;

        while(!copy.empty()){

            spp::BaBNode current_node = copy.top();

            dual_bound = std::max(dual_bound, current_node.getDualBound());
            
            copy.pop();

        }

        return dual_bound;
    }


    spp::BaBNode DepthFirst::pop(){

        spp::BaBNode node = node_list.top();
        node_list.pop();
        return node;
    }


    bool DepthFirst::isEmpty() const {return node_list.empty();}


    size_t DepthFirst::getSize() const{return node_list.size();}
    



    // Setter 

    void DepthFirst::add(spp::BaBNode node){node_list.push(node);}



    void DepthFirst::print() const{

        int counter = 0;
        std::stack<spp::BaBNode> copy = node_list;

        while(!copy.empty()){

            counter += 1;
            std::cout << " node id : " << counter << " --- ";
            spp::BaBNode node = copy.top();
            node.print();
            copy.pop();
        }
    }



}