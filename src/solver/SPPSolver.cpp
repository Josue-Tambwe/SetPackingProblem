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
   #include "dataStructures/BitVector.hpp" // to remove 
   #include "dataStructures/Solution.hpp" // to remove 
   
   using namespace spp; 

   int main(int argc, char** argv){

      Params params = parseOptions(argc, argv);
      Instance instance(params); // to remove
      instance.print(); // to remove

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

      //b1.deactivate(6);
      //std::cout << " deactivate 7 : ";
      //b1.print();

      //std::cout << " nb activated bits : " << b1.getNbNonZeroBits() << "\n";*/

      Solution sol(10, 3);
      sol.activateVar(3);
      sol.activateVar(4);
      sol.activateVar(9);
      sol.printVars();



      

      return 0;
   }