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
   #include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp" // to remove
   #include "algorithms/geneticAlgorithm/Crossover.hpp" // to remove
   #include "algorithms/geneticAlgorithm/Mutation.hpp" // to remove

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

      size_t nb_individuals_to_generate = params.population_size;

      std::vector<float> individuals_construction_alpha_value(nb_individuals_to_generate);
      std::vector<char> individuals_local_search(nb_individuals_to_generate);

      std::array<size_t, 10> nb_individuals_per_alpha_value;
      std::vector<std::array<size_t, 3>> nb_individuals_per_local_search; 

      const std::array<float, 10> alpha_values_2 = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,0.9f, 1.0f};
      std::vector<std::array<double, 3>> all_proportions = {
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

      /*updateLowLevelLocalSearchProportions(all_proportions,
                                           nb_individuals_per_alpha_value,
                                           nb_individuals_per_local_search,
                                           individuals,
                                           instance);

      setAlphaAndLocalSearch(nb_individuals_to_generate,
                              nb_individuals_per_alpha_value,
                              nb_individuals_per_local_search,  
                              individuals_construction_alpha_value, 
                              individuals_local_search,
                              alpha_values_2,
                              all_proportions);*/

      std::vector<double> population_fitness = computePopulationFitness(individuals,
                                                                        params,
                                                                        instance);

      std::vector<double> cumulative_population_fitness = computeCumulativeQuadraticBiasedFitness(population_fitness);


      std::vector<size_t> crossover_participation = computeIndividualsCrossOverParticipation(cumulative_population_fitness,
                                                                                              params);

      std::cout << " \n\n";

      size_t cumul = 0;

      for(size_t i = 0; i < crossover_participation.size(); i++){

         std::cout << " ind : " << (i+1)
                  << " - fitness : " << population_fitness[i]
                  << " - participation : " << crossover_participation[i]
                  << "\n\n";

         cumul += crossover_participation[i];
      }

      std::cout << " total crossover participation : " << cumul << "\n\n";

      std::vector<size_t> guiding_parents;
      std::vector<size_t> initial_parents;


      setCrossoverCouples(initial_parents,
                          guiding_parents,
                          crossover_participation,
                          population_fitness,
                          params);



      std::vector<Solution> children =  performCrossover(individuals,
                                                         initial_parents,
                                                         guiding_parents,
                                                         params,
                                                         instance);

      std::vector<double> children_fitness = computePopulationFitness(children,
                                                                     params,
                                                                     instance);

      std::cout << " \n\n Children \n\n";
      for(size_t i = 0; i < children_fitness.size(); i++){

         std::cout << " ind : " << (i+1) 
                  << " - fitness : " << children_fitness[i]
                  << "\n\n";
      }


      std::vector<size_t> children_to_mutate_indexes = selectIndexesToMutate(children.size(), params);


      performMutation(children,
                      children_to_mutate_indexes,
                      params,
                      instance);


      std::cout << " \n\n Mutation \n\n";
      for(size_t i = 0; i < children_to_mutate_indexes.size(); i++){

         std::cout << " ind : " << (children_to_mutate_indexes[i]+1) 
                  << " - fitness : " << children_fitness[i]
                  << "\n\n";
      }


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