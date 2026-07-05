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
   #include "dataStructures/Solution.hpp" // to remove 
   #include "algorithms/greedy/DeterministicConstruction.hpp" // to remove
   #include "dataStructures/Timer.hpp" // to remove
   #include "algorithms/localSearch/VariableNeighborhoodDescent.hpp" // to remove
   #include "algorithms/greedy/RunGreedy.hpp"
   #include "dataStructures/BaBNode.hpp"
   #include "algorithms/branchAndBound/BestFirst.hpp" // to remove
   #include "algorithms/branchAndBound/DepthFirst.hpp" // to remove
   #include "algorithms/milpSolver/GurobiBackend.hpp" // to remove
   #include "algorithms/milpSolver/HighsBackend.hpp" // to remove
   #include "algorithms/milpSolver/HexalyBackend.hpp" // to remove
   #include "algorithms/greedy/RandomizedConstruction.hpp" // to remove
   #include "algorithms/grasp/ReactiveGRASP.hpp" // to remove

   #if USE_BRANCH_AND_BOUND
   #include "algorithms/branchAndBound/RunBranchAndBound.hpp"
   #endif

   #if USE_MILP
   #include "algorithms/milpSolver/RunMilpSolver.hpp"
   #endif

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);
      const Instance instance(params);

      Timer timer;

      float alpha = 1.0f;

      /*// construction
        timer.start();
        Solution solution = constructAndImproveSolution(alpha, 
                                                        params, 
                                                        instance);
        solution.print(instance);
        std::cout << " time : " << timer.getElapsedTime() << " (s) " << "   status : " << solution.getStatus() << " \n\n";*/


      if(params.algorithm == Algorithm::Greedy){runGreedy(params);}

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

   
         
      return 0;
   }