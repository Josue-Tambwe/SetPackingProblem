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
 * @file TabuSearchNeighborhoods.hpp
 * @brief defines  neighborhoods used in the Tabu Search local search
 * @author Josué Tambwe
 * @date 25 July 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/TabuList.hpp"
#include "algorithms/localSearch/RestrictedNeighborhoods.hpp"
#include <vector>
#include <cstdint>
#include <limits>

namespace spp{


    // ------------------ 2-1 Exchange --------------------

    /**
     * @brief finds the best non-tabu move in the 2-1 neighborhood
     */

    bool findRestrictedTwoOneExchangeScalarTabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);

    #if HAS_X86 && HAS_AVX2

    bool findRestrictedTwoOneExchangeSIMDX86TabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findRestrictedTwoOneExchangeSIMDARMTabuSearch(int &first_index_to_deactivate,
                                                    int &second_index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);

    #endif



    void findRestrictedTwoOneExchangeTabuSearch(std::int64_t &best_improvement,
                                                TabuMove &best_move,
                                                Solution &solution,
                                                std::vector<float> &scores,
                                                const TabuList &tabu_list,
                                                const Params &params,
                                                const Instance &instance);
    






    // ------------------ 1-1 Exchange --------------------

    /**
     * @brief finds the best non-tabu move in the 1-1 neighborhood
     */

    bool findOneOneExchangeScalarTabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);


    #if HAS_X86 && HAS_AVX2

    bool findOneOneExchangeSIMDX86TabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findOneOneExchangeSIMDARMTabuSearch(int &index_to_deactivate,
                                            int &index_to_activate,
                                            std::vector<int> &sorted_activated_vars,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);

    #endif




    void findOneOneExchangeTabuSearch(std::int64_t &best_improvement,
                                      TabuMove &best_move,
                                      Solution &solution,
                                      std::vector<float> &scores,
                                      const TabuList &tabu_list,
                                      const Params &params,
                                      const Instance &instance);








    // ------------------ 0-1 Exchange --------------------

    /**
     * @brief finds the best non-tabu move in the 0-1 neighborhood
     */

    bool findZeroOneExchangeScalarTabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);


    
    #if HAS_X86 && HAS_AVX2

    bool findZeroOneExchangeSIMDX86TabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findZeroOneExchangeSIMDARMTabuSearch(int &index_to_activate,
                                            std::vector<int> &sorted_deactivated_vars,
                                            Solution &solution,
                                            const TabuList &tabu_list,
                                            const Instance &instance);

    #endif




    void findZeroOneExchangeTabuSearch(std::int64_t &best_improvement,
                                       TabuMove &best_move,
                                       Solution &solution,
                                       std::vector<float> &scores,
                                       const TabuList &tabu_list,
                                       const Params &params,
                                       const Instance &instance);











    // ------------------ 1-2 Exchange --------------------

    /**
     * @brief finds the best non-tabu move in the 1-2 neighborhood
     */


    bool findRestrictedOneTwoExchangeScalarTabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);



    #if HAS_X86 && HAS_AVX2

    bool findRestrictedOneTwoExchangeSIMDX86TabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findRestrictedOneTwoExchangeSIMDARMTabuSearch(int &first_index_to_activate,
                                                    int &second_index_to_activate,
                                                    int &to_deactivate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const TabuList &tabu_list,
                                                    const Params &params,
                                                    const Instance &instance);

    #endif




    void findRestrictedOneTwoExchangeTabuSearch(std::int64_t &best_improvement,
                                                TabuMove &best_move,
                                                Solution &solution,
                                                std::vector<float> &scores,
                                                const TabuList &tabu_list,
                                                const Params &params,
                                                const Instance &instance);




    


                                                



    /**
     * @brief Identifies the best admissible move in the neighborhoods which means the best non‑tabu move that improves
     *        or least degrades the current solution according to the evaluation criteria.
     */
    TabuMove findBestMove(TabuList &tabu_list,
                          Solution &solution,
                          const Params &params,
                          const Instance &instance);



    /**
     * @brief updates a solution by performing the exchange of the best move found in the neighborhoods
     */
    void updateSolutionAfterLocalSearch(Solution &solution, 
                                        const TabuMove &best_move, 
                                        const Instance &instance);

}