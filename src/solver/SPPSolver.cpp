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
 * @author Josué Tambwe
 * @date 20 May 2026
 */
   
   
   #include "dataStructures/Parameters.hpp"
   #include "input/OptionParser.hpp"
   #include "dataStructures/Parameters.hpp"
   #include "dataStructures/Instance.hpp" // to remove
   #include <iostream> // to remove
   #include "algorithms/greedy/RunGreedy.hpp"
   #include "algorithms/grasp/RunGRASP.hpp"
   #include "algorithms/tabuSearch/RunTabuSearch.hpp"

   #include <iostream> // to remove

   #if USE_BRANCH_AND_BOUND
   #include "algorithms/branchAndBound/RunBranchAndBound.hpp"
   #endif

   #if USE_MILP
   #include "algorithms/milpSolver/RunMilpSolver.hpp"
   #endif

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);

      if(params.algorithm == Algorithm::Greedy){runGreedy(params);}

      else if(params.algorithm == Algorithm::Grasp){runGRASP(params);}

      else if(params.algorithm == Algorithm::BranchAndBound){

         #if USE_BRANCH_AND_BOUND
         runBaB(params);
         #endif
      }

      else if(params.algorithm == Algorithm::Milp){
         
         #if USE_MILP
         runMilpSolver(params);
         #endif
      }


      else if(params.algorithm == Algorithm::TabuSearch){runTabuSearch(params);}

         
      return 0;
   }