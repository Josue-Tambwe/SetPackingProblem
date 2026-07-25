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
 * @file TabuList.hpp
 * @brief defines the data structures for the management of short term memory in the Tabu Search algorithm
 * @author Josué Tambwe
 * @date 25 July 2026
 */

#pragma once

#include "output/Logger.hpp"
#include <vector>
#include <queue>
#include <iostream>


namespace spp{


    enum TabuMoveType{

        ZeroOne,
        OneOne,
        OneTwo,
        TwoOne,
        Unknown
    };




    /**
     * @class TabuMove
     */

    class TabuMove{

        private :

            TabuMoveType type;
            std::vector<size_t> indexes;

        public :

            // constructors

            TabuMove();

            TabuMove(TabuMoveType t, std::vector<size_t> idx);

            // destructor

            ~TabuMove() = default;

            // getters

            TabuMoveType getMoveType() const;

            std::vector<size_t> getIndexes() const;

            bool equals(const TabuMove &other) const;

            void print() const;

    };




    /**
     * @class TabuList
     */

    class TabuList{

        private : 

            size_t tabu_tenure;
            std::queue<TabuMove> list;

        public : 

            // constructor

            TabuList() = default;

            // destructor

            ~TabuList() = default;



            bool contains(const TabuMove &move) const;

            void addTabuMove(const TabuMove &move);

            void print() const;
    };


}