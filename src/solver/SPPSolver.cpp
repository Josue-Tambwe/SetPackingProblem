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
   #include "dataStructures/Solution.hpp" // to remove
   #include "dataStructures/Timer.hpp" // to remove
   #include "algorithms/greedy/RunGreedy.hpp"
   #include "algorithms/grasp/RunGRASP.hpp"
   #include "algorithms/tabuSearch/RunTabuSearch.hpp"
   #include "algorithms/simulatedAnnealing/RunSimulatedAnnealing.hpp"
   #include "algorithms/geneticAlgorithm/PopulationGeneration.hpp" // to remove

   #include <iostream> // to remove
   #include <array> // to remove
   #include <cmath> // to remove
   #include <vector> // to remove

   #if USE_BRANCH_AND_BOUND
   #include "algorithms/branchAndBound/RunBranchAndBound.hpp"
   #endif

   #if USE_MILP
   #include "algorithms/milpSolver/RunMilpSolver.hpp"
   #endif

   
   using namespace spp; 

   int main(int argc, char** argv){

      const Params params = parseOptions(argc, argv);

      const Instance instance(params); // to remove

      size_t nb_individuals_to_generate = 123;

      std::vector<float> individuals_construction_alpha_value(nb_individuals_to_generate);
      std::vector<char> individuals_local_search(nb_individuals_to_generate);

      std::array<size_t, 10> nb_individuals_per_alpha_value;
      std::vector<std::array<size_t, 3>> nb_individuals_per_local_search; 

      const std::array<float, 10> alpha_values_2 = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,0.9f, 1.0f};
      const std::vector<std::array<double, 3>> all_proportions = {
         {0.33, 0.33, 0.33},
         {0.2, 0.5, 0.3},
         {0.1, 0.3, 0.6},
         {0.9, 0.05, 0.05},
         {0.7, 0.2, 0.1},
         {0.4, 0.3, 0.3},
         {0.2, 0.6, 0.2},
         {0.45, 0.15, 0.4},
         {0.3, 0.6, 0.1},
         {0.8, 0.1, 0.1}
      };

      Timer timer;
      timer.start();
      setAlphaAndLocalSearch(nb_individuals_to_generate,
                              nb_individuals_per_alpha_value,
                              nb_individuals_per_local_search,  
                              individuals_construction_alpha_value, 
                              individuals_local_search,
                              alpha_values_2,
                              all_proportions);

      std::vector<Solution> individuals = generateIndividuals(individuals_construction_alpha_value,
                                                              individuals_local_search,
                                                              params,
                                                              instance);

      std::cout << " \n\n";
      for(size_t i = 0; i < individuals.size(); i++){

         std::cout << " ind : " << (i+1) 
                  << " - fitness : " << individuals[i].getObjectiveValue(instance)
                  << "\n\n";
      }

      std::cout << " elapsed time : " << timer.getElapsedTime() << " (s) \n\n";


      if(params.algorithm == Algorithm::Greedy){runGreedy(params);}

      else if(params.algorithm == Algorithm::Grasp){runGRASP(params);}

      else if(params.algorithm == Algorithm::TabuSearch){runTabuSearch(params);}

      else if(params.algorithm == Algorithm::SimulatedAnnealing){runSimulatedAnnealing(params);}

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