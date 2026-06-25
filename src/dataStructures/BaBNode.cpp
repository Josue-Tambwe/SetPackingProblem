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
 * @file BaBNode.cpp
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#include "dataStructures/BaBNode.hpp"

namespace spp{

    // constructors

    BaBNode::BaBNode(double value) : dual_bound(value){}

    BaBNode::BaBNode(double value, BaBNode &other): 
        dual_bound(value),
        fixed_to_one(other.fixed_to_one),
        fixed_to_zero(other.fixed_to_zero)
    {}


    // getters

    double BaBNode::getDualBound() const {return this->dual_bound;}
    std::vector<int> BaBNode::getIndexesFixedToOne() const {return this->fixed_to_one;}
    std::vector<int> BaBNode::getIndexesFixedToZero() const {return this->fixed_to_zero;}

    // setters

    void BaBNode::fixToOne(int index) {

        this->fixed_to_one.resize(this->fixed_to_one.size() + 1);
        this->fixed_to_one[fixed_to_one.size() - 1] = index;
    }


    void BaBNode::fixToZero(int index) {
        
        this->fixed_to_zero.resize(this->fixed_to_zero.size() + 1);
        this->fixed_to_zero[fixed_to_zero.size()- 1] = index;
    }

    // operator overloading

    bool BaBNode::operator<(const BaBNode &other) const{return this->dual_bound > other.dual_bound;}


    void BaBNode::print() const{

        std::cout << " dual bound : " << dual_bound << "  ---  ones : [ ";
        for(int index : fixed_to_one){std::cout << index << " ";}
        std::cout << "] --- zeros : [ ";
        for(int index : fixed_to_zero){std::cout << index << " ";}
        std::cout << "] \n" << std::endl;
    }


}
