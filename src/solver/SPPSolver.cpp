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
   #include "algorithms/branchAndBound/RunBranchAndBound.hpp" // to remove

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);

      //if(params.algorithm == Algorithm::Greedy){runGreedy(params);}

      const Instance instance(params);
      Timer timer;

      timer.start();
      runBaB(params);
      timer.stop();

      std::cout << " time : " << timer.getElapsedTime() << " (s) \n\n";
                  
      
      
      


      

      return 0;
   }