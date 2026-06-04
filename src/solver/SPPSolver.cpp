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

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);

      if(params.algorithm == Algorithm::Greedy){runGreedy(params);}
      
      


      

      return 0;
   }