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
 * @file RestrictedNeighborhoods.hpp
 * @brief defines restricted 1-2 and 2-1 neighborhoods 
 * @author Josué Tambwe
 * @date 18 July 2026
 */

#pragma once

#include "dataStructures/MoveNode.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/localSearch/NeighborhoodUtils.hpp"
#include "algorithms/localSearch/ConflictCheckerScalar.hpp"

#if HAS_X86 && HAS_AVX2
#include "hpc/simd/ConflictCheckerTwoOneSIMDX86.hpp" // 
#include "hpc/simd/ConflictCheckerOneOneSIMDX86.hpp" //
#include "hpc/simd/ConflictCheckerZeroOneSIMDX86.hpp" //
#include "hpc/simd/ConflictCheckerOneTwoSIMDX86.hpp" //
#endif

#if HAS_ARM && HAS_NEON
#include "hpc/simd/ConflictCheckerTwoOneSIMDARM.hpp"
#include "hpc/simd/ConflictCheckerOneOneSIMDARM.hpp"
#include "hpc/simd/ConflictCheckerZeroOneSIMDARM.hpp"
#include "hpc/simd/ConflictCheckerOneTwoSIMDARM.hpp"
#endif

#include <vector>
#include <algorithm>


namespace spp{



    /**
     * @brief searches a feasible 2-1 exchange move : 2 variables to deactivate and 1 variable to activate
     *        the vector of currently activated variables (nonzero variables) is sorted in increasing order of the heuristic score
     *        the vector of currently deactivated variables (zero variables) is sorted in decreasing order of the heuristic score
     *        heuristic score : profit / nb resources required
     *
     *        To reduce the neighborhood size, only a given percentage of the best deactivated variables is considered.
     *        A second linear pruning is applied inside the inner loop, significantly lowering the complexity of the search.
     */
    bool findRestrictedTwoOneExchange(int &first_index_to_deactivate,
                                    int &second_index_to_deactivate,
                                    int &index_to_activate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Params &params,
                                    const Instance &instance);




    #if HAS_X86 && HAS_AVX2

    bool findRestrictedTwoOneExchangeSIMDX86(int &first_index_to_deactivate,
                                            int &second_index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findRestrictedTwoOneExchangeSIMDARM(int &first_index_to_deactivate,
                                            int &second_index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance);

    #endif















    /**
    * @brief searches a feasible 1-2 exchange move : 1 variable to deactivate and 2 variables to activate
    *
    *        Only a given percentage of the highest‑scoring deactivated variables is explored.
    *        A quadratic pruning strategy limits the search over deactivated candidates, while
    *        a linear pruning is applied to the activated variables. These pruning mechanisms
    *        significantly reduce the neighborhood size and improve efficiency.
    */
    bool findRestrictedOneTwoExchange(int &first_index_to_activate,
                                    int &second_index_to_activate,
                                    int &to_deactivate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Params &params,
                                    const Instance &instance);



    #if HAS_X86 && HAS_AVX2

    bool findRestrictedOneTwoExchangeSIMDX86(int &first_index_to_activate,
                                            int &second_index_to_activate,
                                            int &to_deactivate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance);

    #endif




    #if HAS_ARM && HAS_NEON

    bool findRestrictedOneTwoExchangeSIMDARM(int &first_index_to_activate,
                                            int &second_index_to_activate,
                                            int &to_deactivate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const Params &params,
                                            const Instance &instance);

    #endif


}


