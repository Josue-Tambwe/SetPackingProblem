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
   #include "dataStructures/Instance.hpp" // to remove
   #include <iostream> // to remove
   #include "dataStructures/Solution.hpp" // to remove 
   #include "algorithms/greedy/DeterministicConstruction.hpp" // to remove
   #include "dataStructures/Timer.hpp" // to remove
   #include "algorithms/localSearch/VariableNeighborhoodDescent.hpp" // to remove

   
   using namespace spp; 

   int main(int argc, char** argv){

      Params params = parseOptions(argc, argv);
      const Instance instance(params); // to remove
      //instance.print(); // to remove

      Timer timer;
      timer.start();
      Solution solution = deterministicConstruction(instance);
      timer.stop();

      solution.print(instance);
      std::cout << " \n \n";
      std::cout << " construction time : " << timer.getElapsedTime() << "\n";
      std::cout << " status            : " << solution.getStatus() << " \n \n"; 
      std::cout << " \n \n";
      
      timer.reset();
      timer.start();
      variableNeighborhoodDescent(params, solution, instance);
      timer.stop();
   
      solution.print(instance);
      std::cout << " \n \n";
      std::cout << " improment time : " << timer.getElapsedTime() << "\n"; 
      std::cout << " status         : " << solution.getStatus() << " \n \n"; 

      /*if(HAS_X86){std::cout << " Has X86 ISA \n";}
      if(HAS_AVX2){std::cout << " Has AVX2 SIMD extension \n";}
      if(HAS_ARM){std::cout << " Has ARM ISA \n";}
      if(HAS_NEON){std::cout << " Has NEON SIMD extension \n";}*/
      
      


      

      return 0;
   }