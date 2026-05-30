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
 * @file ConflictCheckerScalar.cpp
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #include "algorithms/localSearch/ConflictCheckerScalar.hpp"


 namespace spp{


    //------------------------------- 2-1 EXCHANGE -------------------------------------------

    bool checkConflictTwoOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){


        while((cursor + 3) < first_to_deactivate.size()){

            // loading 4 64-bits words
            std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
            std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];
            std::uint64_t occupied_after_removal_3 = consumed_resources[(cursor + 2)];
            std::uint64_t occupied_after_removal_4 = consumed_resources[(cursor + 3)];

            // first deactivation
            occupied_after_removal_1 &= ~first_to_deactivate[cursor];
            occupied_after_removal_2 &= ~first_to_deactivate[(cursor + 1)];
            occupied_after_removal_3 &= ~first_to_deactivate[(cursor + 2)];
            occupied_after_removal_4 &= ~first_to_deactivate[(cursor + 3)];

            // second deactivation
            occupied_after_removal_1 &= ~second_to_deactivate[cursor];
            occupied_after_removal_2 &= ~second_to_deactivate[(cursor + 1)];
            occupied_after_removal_3 &= ~second_to_deactivate[(cursor + 2)];
            occupied_after_removal_4 &= ~second_to_deactivate[(cursor + 3)];

            // checking conflict witn potential activation
            std::uint64_t conflict_1 = occupied_after_removal_1 & to_activate[cursor];
            std::uint64_t conflict_2 = occupied_after_removal_2 & to_activate[(cursor + 1)];
            std::uint64_t conflict_3 = occupied_after_removal_3 & to_activate[(cursor + 2)];
            std::uint64_t conflict_4 = occupied_after_removal_4 & to_activate[(cursor + 3)];

            // synchronization
            if(((conflict_1 | conflict_2) | (conflict_3 | conflict_4)) != 0ULL){return false;}

            cursor += 4;

        }

        return true;
    }



    bool checkConflictTwoOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        // loading 3 64-bits words
        std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
        std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];

        // first deactivation
        occupied_after_removal_1 &= ~first_to_deactivate[cursor];
        occupied_after_removal_2 &= ~first_to_deactivate[(cursor + 1)];

        // second deactivation
        occupied_after_removal_1 &= ~second_to_deactivate[cursor];
        occupied_after_removal_2 &= ~second_to_deactivate[(cursor + 1)];

        // checking conflict witn potential activation
        std::uint64_t conflict_1 = occupied_after_removal_1 & to_activate[cursor];
        std::uint64_t conflict_2 = occupied_after_removal_2 & to_activate[(cursor + 1)];

        cursor += 2;

        return ((conflict_1 | conflict_2) == 0ULL);
        

    }



    bool checkConflictTwoOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){
        // loading
        std::uint64_t occupied_after_removal = consumed_resources[cursor];

        // first deactivation
        occupied_after_removal &= ~first_to_deactivate[cursor];

        // second deactivation
        occupied_after_removal &= ~second_to_deactivate[cursor];

        // checking conflict witn potential activation
        std::uint64_t conflict = occupied_after_removal & to_activate[cursor];
        
        cursor += 1;

        return (conflict == 0ULL);

    }



    bool checkConflictTwoOneMove(const std::vector<std::uint64_t> &first_to_deactivate,
                                 const std::vector<std::uint64_t> &second_to_deactivate,
                                 const std::vector<std::uint64_t> &to_activate,
                                 std::vector<std::uint64_t> &consumed_resources){

        size_t cursor = 0;

        if(!checkConflictTwoOneMove_X4(cursor, 
                                      first_to_deactivate, 
                                      second_to_deactivate, 
                                      to_activate, 
                                      consumed_resources)){

            return true;
        }

        if((cursor + 1) < first_to_deactivate.size()){

            if(!checkConflictTwoOneMove_X2(cursor, 
                                          first_to_deactivate, 
                                          second_to_deactivate, 
                                          to_activate, 
                                          consumed_resources)){

                return true;
            }

        }

        if(cursor < first_to_deactivate.size()){

            if(!checkConflictTwoOneMove_X1(cursor, 
                                          first_to_deactivate, 
                                          second_to_deactivate, 
                                          to_activate, 
                                          consumed_resources)){

                return true;
            }
            
        }

        return false;
    }





    //------------------------------- 1-1 EXCHANGE -------------------------------------------


    bool checkConflictOneOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        while((cursor + 3) < to_deactivate.size()){

            // loading 4 64-bits words
            std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
            std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];
            std::uint64_t occupied_after_removal_3 = consumed_resources[(cursor + 2)];
            std::uint64_t occupied_after_removal_4 = consumed_resources[(cursor + 3)];

            // deactivation
            occupied_after_removal_1 &= ~to_deactivate[cursor];
            occupied_after_removal_2 &= ~to_deactivate[(cursor + 1)];
            occupied_after_removal_3 &= ~to_deactivate[(cursor + 2)];
            occupied_after_removal_4 &= ~to_deactivate[(cursor + 3)];

            // checking conflict witn potential activation
            std::uint64_t conflict_1 = occupied_after_removal_1 & to_activate[cursor];
            std::uint64_t conflict_2 = occupied_after_removal_2 & to_activate[(cursor + 1)];
            std::uint64_t conflict_3 = occupied_after_removal_3 & to_activate[(cursor + 2)];
            std::uint64_t conflict_4 = occupied_after_removal_4 & to_activate[(cursor + 3)];

            // synchronization
            if(((conflict_1 | conflict_2) | (conflict_3 | conflict_4)) != 0ULL){return false;}

            cursor += 4;

        }

        return true;
                                    
                                    
    }



    bool checkConflictOneOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){
                                    
        // loading 3 64-bits words
        std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
        std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];

        // deactivation
        occupied_after_removal_1 &= ~to_deactivate[cursor];
        occupied_after_removal_2 &= ~to_deactivate[(cursor + 1)];

        // checking conflict witn potential activation
        std::uint64_t conflict_1 = occupied_after_removal_1 & to_activate[cursor];
        std::uint64_t conflict_2 = occupied_after_removal_2 & to_activate[(cursor + 1)];

        cursor += 2;

        return ((conflict_1 | conflict_2) == 0ULL);

    }



    bool checkConflictOneOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        // loading
        std::uint64_t occupied_after_removal = consumed_resources[cursor];

        // deactivation
        occupied_after_removal &= ~to_deactivate[cursor];

        // checking conflict witn potential activation
        std::uint64_t conflict = occupied_after_removal & to_activate[cursor];
        
        cursor += 1;

        return (conflict == 0ULL);

    }


    bool checkConflictOneOneMove(const std::vector<std::uint64_t> &to_deactivate,
                              const std::vector<std::uint64_t> &to_activate,
                              std::vector<std::uint64_t> &consumed_resources){

        size_t cursor = 0;

        if(!checkConflictOneOneMove_X4(cursor, 
                                      to_deactivate, 
                                      to_activate, consumed_resources)){

            return true;
        }

        if((cursor + 1) < to_deactivate.size()){

            if(!checkConflictOneOneMove_X2(cursor, 
                                          to_deactivate, 
                                          to_activate, 
                                          consumed_resources)){

                return true;
            }

        }

        if(cursor < to_deactivate.size()){

            if(!checkConflictOneOneMove_X1(cursor, 
                                          to_deactivate, 
                                          to_activate, 
                                          consumed_resources)){

                return true;
            }
            
        }

        return false;

    }






    //------------------------------- 0-1 EXCHANGE -------------------------------------------


    bool checkConflictZeroOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        while((cursor + 3) < to_activate.size()){

            // checking conflict witn potential activation
            std::uint64_t conflict_1 = consumed_resources[cursor] & to_activate[cursor];
            std::uint64_t conflict_2 = consumed_resources[(cursor + 1)] & to_activate[(cursor + 1)];
            std::uint64_t conflict_3 = consumed_resources[(cursor + 2)] & to_activate[(cursor + 2)];
            std::uint64_t conflict_4 = consumed_resources[(cursor + 3)]  & to_activate[(cursor + 3)];

            // synchronization
            if(((conflict_1 | conflict_2) | (conflict_3 | conflict_4)) != 0ULL){return false;}

            cursor += 4;

        }

        return true;
                                    
                                    
    }



    bool checkConflictZeroOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){
                                    
        // checking conflict witn potential activation
        std::uint64_t conflict_1 = consumed_resources[cursor] & to_activate[cursor];
        std::uint64_t conflict_2 = consumed_resources[(cursor + 1)] & to_activate[(cursor + 1)];

        cursor += 2;

        return ((conflict_1 | conflict_2) == 0ULL);

    }



    bool checkConflictZeroOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){


        // checking conflict witn potential activation
        std::uint64_t conflict = consumed_resources[cursor] & to_activate[cursor];
        
        cursor += 1;

        return (conflict == 0ULL);

    }


    bool checkConflictZeroOneMove(const std::vector<std::uint64_t> &to_activate,
                                  std::vector<std::uint64_t> &consumed_resources){

        size_t cursor = 0;

        if(!checkConflictZeroOneMove_X4(cursor, 
                                        to_activate, 
                                        consumed_resources)){

            return true;
        }

        if((cursor + 1) < to_activate.size()){

            if(!checkConflictZeroOneMove_X2(cursor, 
                                            to_activate, 
                                            consumed_resources)){

                return true;
            }

        }

        if(cursor < to_activate.size()){

            if(!checkConflictZeroOneMove_X1(cursor,  
                                            to_activate, 
                                            consumed_resources)){

                return true;
            }
            
        }

        return false;

    }




    //------------------------------- 1-2 EXCHANGE -------------------------------------------


    bool checkConflictOneTwoMove_X4(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources){

        while((cursor + 3) < first_to_activate.size()){

            // loading 4 64-bits words
            std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
            std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];
            std::uint64_t occupied_after_removal_3 = consumed_resources[(cursor + 2)];
            std::uint64_t occupied_after_removal_4 = consumed_resources[(cursor + 3)];

            // deactivation
            occupied_after_removal_1 &= ~to_deactivate[cursor];
            occupied_after_removal_2 &= ~to_deactivate[(cursor + 1)];
            occupied_after_removal_3 &= ~to_deactivate[(cursor + 2)];
            occupied_after_removal_4 &= ~to_deactivate[(cursor + 3)];


            // checking conflict witn potential activations
            std::uint64_t conflict_1 = occupied_after_removal_1 & (first_to_activate[cursor] & second_to_activate[cursor]);
            std::uint64_t conflict_2 = occupied_after_removal_2 & (first_to_activate[(cursor + 1)] & second_to_activate[(cursor + 1)]) ;
            std::uint64_t conflict_3 = occupied_after_removal_3 & (first_to_activate[(cursor + 2)] & second_to_activate[(cursor + 2)]);
            std::uint64_t conflict_4 = occupied_after_removal_4 & (first_to_activate[(cursor + 3)] & second_to_activate[(cursor + 3)]);

            // synchronization
            if(((conflict_1 | conflict_2) | (conflict_3 | conflict_4)) != 0ULL){return false;}

            cursor += 4;

        }

        return true;

    }



    bool checkConflictOneTwoMove_X2(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources){

        // loading 3 64-bits words
        std::uint64_t occupied_after_removal_1 = consumed_resources[cursor];
        std::uint64_t occupied_after_removal_2 = consumed_resources[(cursor + 1)];

        // deactivation
        occupied_after_removal_1 &= ~to_deactivate[cursor];
        occupied_after_removal_2 &= ~to_deactivate[(cursor + 1)];


        // checking conflict witn potential activations
        std::uint64_t conflict_1 = occupied_after_removal_1 & (first_to_activate[cursor] & second_to_activate[cursor]);
        std::uint64_t conflict_2 = occupied_after_removal_2 & (first_to_activate[(cursor + 1)] & second_to_activate[(cursor + 1)]) ;

        cursor += 2;

        return ((conflict_1 | conflict_2) == 0ULL);

    }




    bool checkConflictOneTwoMove_X1(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources){

        // loading
        std::uint64_t occupied_after_removal = consumed_resources[cursor];

        // deactivation
        occupied_after_removal &= ~to_deactivate[cursor];

        // checking conflict witn potential activations
        std::uint64_t conflict = occupied_after_removal & (first_to_activate[cursor] & second_to_activate[cursor]);
        
        cursor += 1;

        return (conflict == 0ULL);

    }




    bool checkConflictOneTwoMove(const std::vector<std::uint64_t> &first_to_activate,
                                const std::vector<std::uint64_t> &second_to_activate,
                                const std::vector<std::uint64_t> &to_deactivate,
                                std::vector<std::uint64_t> &consumed_resources){

        size_t cursor = 0;

        if(!checkConflictOneTwoMove_X4(cursor, 
                                      first_to_activate, 
                                      second_to_activate,
                                      to_deactivate, 
                                      consumed_resources)){

            return true;
        }

        if((cursor + 1) < first_to_activate.size()){

            if(!checkConflictOneTwoMove_X2(cursor, 
                                          first_to_activate, 
                                          second_to_activate,
                                          to_deactivate,  
                                          consumed_resources)){

                return true;
            }

        }

        if(cursor < first_to_activate.size()){

            if(!checkConflictOneTwoMove_X1(cursor, 
                                          first_to_activate, 
                                          second_to_activate,
                                          to_deactivate,   
                                          consumed_resources)){

                return true;
            }
            
        }

        return false;

    }

    

 }