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

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);

      //if(params.algorithm == Algorithm::Greedy){runGreedy(params);}

      const Instance instance(params);
      Timer timer;
      GurobiBackend lp_gurobi;
      HighsBackend lp_highs;

      timer.start();
      lp_gurobi.buildContinuousModel(instance);
      lp_gurobi.solveContinuousModel();
      timer.stop();

      std::cout << " LP sol gurobi : " << lp_gurobi.getObjectiveValue() << " time : " << timer.getElapsedTime() << " (s) \n\n";

      timer.reset();
      timer.start();
      lp_highs.buildContinuousModel(instance);
      lp_highs.solveContinuousModel();
      timer.stop();

      std::cout << " LP sol highs  : " << lp_highs.getObjectiveValue() << " time : " << timer.getElapsedTime() << " (s) \n\n";

      GurobiBackend ip_gurobi;
      timer.reset();
      timer.start();
      ip_gurobi.buildIntegerModel(instance);
      ip_gurobi.solveIntegerModel(120.0);
      timer.stop();

      std::cout << " IP sol gurobi : " << ip_gurobi.getObjectiveValue() << " time : " << timer.getElapsedTime() << " (s) \n\n";


      HighsBackend ip_highs;
      timer.reset();
      timer.start();
      ip_highs.buildIntegerModel(instance);
      ip_highs.solveIntegerModel(120.0);
      timer.stop();

      std::cout << " IP sol highs  : " << ip_highs.getObjectiveValue() << " time : " << timer.getElapsedTime() << " (s) \n\n";


      HexalyBackend hexaly;
      timer.reset();
      timer.start();
      hexaly.buildIntegerModel(instance);
      hexaly.solveIntegerModel(120.0);
      timer.stop();

      std::cout << " IP sol hexaly  : " << hexaly.getObjectiveValue() << " time : " << timer.getElapsedTime() << " (s) \n\n";


      
      


      

      return 0;
   }