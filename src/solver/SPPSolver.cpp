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
   
   
   #include "configuration/GeneralConfig.hpp"
   #include "dataStructures/Parameters.hpp"
   #include "input/OptionParser.hpp"
   #include "dataStructures/Instance.hpp" // to remove
   #include <iostream> // to remove
   #include <vector> // to remove
   #include "dataStructures/BitVector.hpp" // to remove 
   #include "dataStructures/Solution.hpp" // to remove 
   #include "algorithms/greedy/DeterministicConstruction.hpp" // to remove
   #include "dataStructures/Timer.hpp" // to remove
   #include "dataStructures/MoveNode.hpp" // to remove
   #include "algorithms/localSearch/NeighborhoodUtils.hpp" // to remove
   
   using namespace spp; 

   int main(int argc, char** argv){

      Params params = parseOptions(argc, argv);
      const Instance instance(params); // to remove
      //instance.print(); // to remove
      Timer timer;
      timer.start();
      Solution solution = greedy::deterministicConstruction(instance);
      timer.stop();
      solution.print(instance);
      std::cout << " \n \n";
      std::cout << " construction time : " << timer.getElapsedTime(); 
      std::cout << " \n \n";

      std::vector<float> scores = computeVariableScores(instance);
      std::vector<int> active = sortNonZeroVars(solution, scores);
      std::cout << " active index : [ ";
      for(int index : active){std::cout << "(" << index << " - " << scores[index] << ") ";}
      std::cout << "] \n \n";

      std::vector<int> inactive = sortZeroVars(solution, scores);

      std::cout << " inactive index : [ ";
      for(int index : inactive){std::cout << "(" << index << " - " << scores[index] << ") ";}
      std::cout << "] \n";
      //solution.printConsumedResources();

      
      /*if(USE_SIMD){std::cout << " Use SIMD \n";}
      if(HAS_SSE2){std::cout << " Has SSE2 \n";}
      if(HAS_AVX2){std::cout << " Has AVX2 \n";}
      if(HAS_AVX512F){std::cout << " Has AVX512F \n";}
      if(HAS_NEON){std::cout << " Has NEON \n";}*/

      /*Solution sol(9, 7);
      sol.activateVar(3, instance);
      sol.activateVar(5, instance);
      sol.print(instance);
      sol.printConsumedResources();

      std::cout << "\n index 6 deactivation \n\n";
      sol.deactivateVar(5, instance);
      sol.print(instance);
      sol.printConsumedResources();*/
      

      //sol.activateVar(9);
      //instance.printResourceRequirements();

      /*BitVector b1(70);
      b1.activate(2);
      std::cout << "\n \n activate 3 : ";
      b1.print();

      b1.activate(6);
      std::cout << " activate 7 : ";
      b1.print();

      b1.activate(9);
      std::cout << " activate 10 : ";
      b1.print();

      b1.activate(59);
      std::cout << " activate 60 : ";
      b1.print();

      b1.activate(69);
      std::cout << " activate 70 : ";
      b1.print();

      std::cout << " nb activated bits : " << b1.getNbNonZeroBits() << "\n\n";

      b1.printNonZeroIndexes();
      b1.printZeroIndexes();

      //b1.deactivate(6);
      //std::cout << " deactivate 7 : ";
      //b1.print();

      //std::cout << " nb activated bits : " << b1.getNbNonZeroBits() << "\n";*/

      



      

      return 0;
   }