/*------------------------------------------------------------------------------
 * SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
 *              and efficient BitVector techniques.
 *------------------------------------------------------------------------------
 * Copyright (c) 2025 Josué Tambwe
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
   #include "dataStructures/Instance.hpp"
   #include <iostream> // to remove 
   
   using namespace spp; 

   int main(int argc, char** argv){

      Params params;
      params.instance_path = argv[1];
      Instance instance(params);
      instance.print();

      

      return 0;
   }