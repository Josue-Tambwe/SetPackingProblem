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
 * @file GeneticAlgorithmOutput.cpp
 * @author Josué Tambwe
 * @date 14 August 2026
 */

 
#include "output/GeneticAlgorithmOutput.hpp"

namespace spp{


        void printHeaderGeneticAlgorithm(const Params &params, 
                                         const Instance &instance){

                        const InstanceStatistics& stats = instance.getStatistics();

                        // line 1
                        std::cout << YELLOW;
                        std::cout << "  " <<  "Instance statistics" << std::string(27, ' ') 
                                << "Algorithm" << std::string(14, ' ')
                                << "General Settings" << std::string(22, ' ')
                                << "Operator Settings"
                                << RESET << "\n";

                        // line 2
                        std::cout << "  "  << std::string(19, '-') << std::string(27, ' ') 
                                << std::string(9, '-') << std::string(14, ' ')
                                << std::string(16, '-') << std::string(22, ' ')
                                << std::string(17, '-')
                                << "\n";

                        // line 3
                        std::cout << "  " << std::setw(22) << std::left << "number of variables" 
                                <<  std::setw(2) << std::right << " : " 
                                <<  std::setw(14) <<  std::left << stats.nb_vars << std::setw(3) << std::left << " "

                                << MAGENTA << "Genetic Algorithm" << RESET << std::setw(10) << std::left << " "

                                << std::setw(19) << std::left << "SIMD vectorization" <<  std::setw(2) << std::right << " : ";
                                if(params.use_simd){std::cout << GREEN << std::setw(14) << std::left << "Enabled" << RESET;}
                                else{std::cout << RED << std::setw(14) << std::left << "Disabled" << RESET;}
                                std::cout << std::setw(2) << std::left << " "

                                << std::setw(21) << std::left << "pruning rate" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.pruning_rate
                                << 1 << "\n"; 

                        //line 4
                        std::cout << "  " << std::setw(22) << std::left << "number of constraints"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(16) <<  std::left << stats.nb_constraints << std::setw(28) << std::left << " "

                                
                                << std::setw(19) << std::left << "intensification" <<  std::setw(2) << std::right << " : ";
                                if(params.use_intensification){std::cout << GREEN << std::setw(14) << std::left << "Enabled" << RESET;}
                                else{std::cout << RED  << std::setw(14) << std::left << "Disabled" << RESET;}
                                std::cout << std::setw(2) << std::left << " "

                                << std::setw(21) << std::left << "restart interval" <<  std::setw(2) << std::right << " : "
                                << params.restart_interval 
                                << "\n";

                        // line 5
                        std::cout << std::setw(71) << std::left << " "
                                << std::setw(19) << std::left << "population size" << std::setw(3)  << std::right << " : "
                                <<  std::setw(14) <<  std::left << params.population_size 
                                << std::setw(2) << std::left << " "

                                << std::setw(21) << std::left << "local search time (s)" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.improvement_time 
                                << "\n";

                        // line 6
                        std::cout << "  " << std::setw(22) << std::left << "profit range"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(16) <<  std::left 
                                << (std::to_string(stats.min_profit) + " - " + std::to_string(stats.max_profit))
                                << std::setw(5) << std::left << " "

                                << YELLOW
                                << "Hardware"
                                << RESET
                                << "\n";

                                        

                        // line 7
                        std::cout << "  " << std::setw(22) << std::left << "max nonzeros per row"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(16) <<  std::left << stats.max_nonzeros_per_row
                                << std::setw(5) << std::left << " "

                                << std::string(8, '-')

                                << std::setw(53) << std::left << " "
                                << std::setw(21) << std::left << "tabu tenure" <<  std::setw(2) << std::right << " : "
                                << params.tabu_tenure 
                                <<  "\n";

                        // line 8
                        std::cout << "  " << std::setw(22) << std::left << "matrix density (%)"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(16) <<  std::left << std::fixed << std::setprecision(3) <<  stats.density
                                << std::setw(5) << std::left << " "

                                << std::setw(18) << std::left << "CPU ISA" <<  std::setw(2) << std::right << " : ";
                                if(HAS_X86){std::cout << std::setw(15) << std::left << "x86";} 
                                else{std::cout << std::setw(15) << std::left << "ARM";}
                        std::cout <<  "\n";



                        // line 9
                        std::string simd_value;

                                if(HAS_X86){
                                        if(HAS_AVX2){simd_value = "AVX2 (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                        else{simd_value = "AVX2 (" + std::string(RED) + "Not detected" + RESET + ")";}
                                }

                                else{
                                        if(HAS_NEON){simd_value = "NEON (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                        else{simd_value = "NEON (" + std::string(RED) + "Not detected" + RESET + ")";}
                                }


                        std::cout << std::setw(48) << std::left << " ";
                                if(HAS_X86){
                                        if(HAS_AVX2){simd_value = "AVX2 (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                        else{simd_value = "AVX2 (" + std::string(RED) + "Not detected" + RESET + ")";}
                                }

                                else{
                                        if(HAS_NEON){simd_value = "NEON (" + std::string(GREEN) + "Detected" + RESET + ")";}
                                        else{simd_value = "NEON (" + std::string(RED) + "Not detected" + RESET + ")";}
                                }

                                

                                std::cout << std::setw(18) << std::left  << "SIMD extension" << std::setw(3)  << std::right << " : "
                                << std::setw(30) << std::left  << simd_value 
                                << std::setw(19) << std::left << " "

                                << std::setw(21) << std::left << "initial temperature" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.initial_temperature         
                                <<  "\n";

                        //line 10
                        std::cout << std::setw(48) << std::left << " "
                                << std::setw(15) << std::left << "CPU physical cores" << std::setw(3)  << std::right << " : "
                                <<  std::setw(21) <<  std::left << NUMBER_PHYSICAL_CORES
                                << std::setw(19) << std::left << " "

                                << std::setw(21) << std::left << "final temperature" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.final_temperature     
                                << "\n";



                        // line 11
                        std::cout << std::setw(48) << std::left << " "

                                << std::setw(18) << std::left << "threads used" <<  std::setw(2) << std::right << " : "
                                <<  std::setw(18) << std::left << params.nb_threads 
                                << std::setw(22) << std::left << " "
                                
                                << std::setw(21) << std::left << "cooling interval" <<  std::setw(2) << std::right << " : "
                                << params.cooling_interval        
                                <<  "\n";

                        // line 12
                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "cooling factor" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.cooling_factor          
                                <<  "\n\n";

                        // line 13
                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "crossover rate" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.crossover_rate          
                                <<  "\n";

                        // line 14
                        
                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "mutation rate" <<  std::setw(2) << std::right << " : "
                                << params.mutation_rate          
                                <<  "\n";

                        // line 15
                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "survivor rate" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.survivor_rate        
                                <<  "\n";


                        // line 16
                        if(params.use_max_iterations){

                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "maximum generations" <<  std::setw(2) << std::right << " : "
                                << params.nb_max_iterations << "\n";
                        }


                        // line 17
                        if(params.use_time_limit){

                        std::cout << std::setw(109) << std::left << " "
                                << std::setw(21) << std::left << "time limit (s)" <<  std::setw(2) << std::right << " : "
                                << std::fixed << std::setprecision(2) << params.time_limit << "\n";  
                        
                        }  

                        std::cout << "\n\n";
        }






        void computeProportionVectors(std::vector<double> &VND,
                                      std::vector<double> &TS,
                                      std::vector<double> &SA,
                                      const std::vector<std::array<double, 3>> all_proportions){

                for(size_t i = 0; i < all_proportions.size(); i++){

                        VND[i] = all_proportions[i][0];
                        TS[i] = all_proportions[i][1];
                        SA[i] = all_proportions[i][2];
                }
        }






        void printGeneticAlgorithmIteration(double current_time, 
                                            size_t current_generation,
                                            size_t nb_couples,
                                            size_t nb_children,
                                            size_t nb_mutated_children,
                                            std::int64_t best_individual_objective_value,
                                            double max_population_fitness,
                                            double min_population_fitness,
                                            double average_population_fitness,
                                            double max_children_fitness,
                                            double min_children_fitness,
                                            double average_children_fitness,
                                            double max_mutated_children_fitness,
                                            double min_mutated_children_fitness,
                                            double average_mutated_children_fitness,
                                            const std::array<float, 10> &alpha_values,
                                            const std::vector<std::array<double, 3>> &all_proportions){

                        // line 1
                        std::cout << "  " << std::setw(15) << std::left << "time (s)"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(25) << std::left << std::fixed << std::setprecision(3) 
                                << current_time

                                << std::setw(4) << std::left << " "
                                << std::setw(12) << std::left << "alpha values"
                                << std::setw(2) << std::right << " : ";

                        for(float alpha : alpha_values){

                                std::cout << YELLOW
                                << std::fixed << std::setprecision(1) << alpha
                                << RESET 
                                << "  | ";
                        }

                                
                        std::cout <<  "\n";

                        // line 2
                        std::cout << "  " << std::setw(15) << std::left << "generation"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(20) <<  std::left <<  current_generation

                                << std::setw(24) << std::left << " "
                                << std::string(69, '-')

                                <<  "\n";


                        std::vector<double> VND(all_proportions.size());
                        std::vector<double> TS(all_proportions.size());
                        std::vector<double> SA(all_proportions.size());

                        computeProportionVectors(VND,
                                                 TS,
                                                 SA,
                                                 all_proportions);

                        // line 3
                        std::cout << "  " << std::setw(15) << std::left << "best individual"
                                << std::setw(2) << std::right << " : " 
                                <<  BRIGHT_YELLOW 
                                << std::setw(26) <<  std::left <<  best_individual_objective_value
                                << RESET

                                << std::setw(3) << std::left << " "
                                << std::setw(12) << std::right << "VND"
                                << std::setw(2) << std::right << " : ";
                        
                        for(double proba : VND){std::cout << std::fixed << std::setprecision(2) << proba << " | ";}
                        std::cout << "\n";


                        // line 4
                        std::cout << std::setw(72) << std::left << " "
                                << std::string(69, '-')
                                << "\n";


                        // line 5
                        std::cout << "  " << std::setw(18) << std::left << "Population fitness"
                                << std::setw(29) << std::left << " "

                                << std::setw(12) << std::right << "TS"
                                << std::setw(2) << std::right << " : ";
                        
                        for(double proba : TS){std::cout << std::fixed << std::setprecision(2) << proba << " | ";}

                        std::cout << "\n";


                        // line 6
                        std::cout << "  " << std::string(18, '-')
                                << std::setw(44) << std::left << " "

                                 << std::string(69, '-')
                                 << "\n";


                        // line 7
                        std::cout << "  " << std::setw(10) << std::left << "minimum"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << min_population_fitness

                                << std::setw(8) << std::left << " "
                                << std::setw(12) << std::right << "SA"
                                << std::setw(2) << std::right << " : ";
                        
                        for(double proba : SA){std::cout << std::fixed << std::setprecision(2) << proba << " | ";}

                        std::cout << "\n";



                        // line 8
                        std::cout << "  " << std::setw(10) << std::left << "average"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(2) 
                                << average_population_fitness
                                << "\n";


                        // line 9
                        std::cout << "  " << std::setw(10) << std::left << "maximum"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << max_population_fitness
                                << "\n\n";

                        // line 10
                        std::cout << "  " << std::setw(9) << std::left << "Crossover"
                                << std::setw(40) << std::left << " "

                                << std::setw(8) << std::left << "Mutation"
                                << std::setw(40) << std::left << " "

                                << std::setw(8) << std::left << "Evolutionnary counts"
                                << "\n";


                        // line 11
                        std::cout << "  " << std::string(9, '-') 
                                << std::setw(40) << std::left << " "

                                << std::string(8, '-')
                                << std::setw(40) << std::left << " "

                                << std::string(20, '-')
                                << "\n";


                        // line 12
                        std::cout << "  " << std::setw(15) << std::left << "minimum fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << min_children_fitness
                                << std::setw(5) << std::left << " "

                                << std::setw(15) << std::left << "minimum fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << min_mutated_children_fitness
                                << std::setw(4) << std::left << " "

                                << std::setw(17) << std::left << "mating pairs"
                                << std::setw(2) << std::right << " : " 
                                << nb_couples

                                << "\n";



                        // line 13
                        std::cout << "  " << std::setw(15) << std::left << "average fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(2) 
                                << average_children_fitness
                                << std::setw(5) << std::left << " "

                                << std::setw(15) << std::left << "average fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(2) 
                                << average_mutated_children_fitness
                                << std::setw(4) << std::left << " "

                                << std::setw(17) << std::left << "offspring"
                                << std::setw(2) << std::right << " : " 
                                << nb_children
                                << "\n";


                        // line 14
                        std::cout << "  " << std::setw(15) << std::left << "maximum fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << max_children_fitness
                                << std::setw(5) << std::left << " "

                                << std::setw(15) << std::left << "maximum fitness"
                                << std::setw(2) << std::right << " : " 
                                <<  std::setw(26) << std::left << std::fixed << std::setprecision(0) 
                                << max_mutated_children_fitness<< std::setw(4) << std::left << " "

                                << std::setw(17) << std::left << "mutated offspring"
                                << std::setw(2) << std::right << " : " 
                                << nb_mutated_children
                                << "\n";

                        std::cout <<  "\n\n\n\n\n";

        }







        void printSummaryGeneticAlgorithm(double total_time, 
                                        std::int64_t elite_objective_value,
                                        size_t total_generations,
                                        Status status){

                        std::cout << "\n";

                        std::cout << std::right << std::setw(2)   << ""
                                << std::right  << YELLOW << "Summary" << RESET << "\n"
                                << std::string(2, ' ')
                                << std::string(7, '-')
                                << "\n\n";

                        std::cout << std::string(2, ' ')
                                << std::right << std::setw(2) 
                                << std::left << std::setw(17)  << "elapsed time (s)"
                                << std::right  << " : " 
                                << std::left  << std::setprecision(3) << total_time
                                << "\n\n"

                                << std::string(2, ' ')
                                << std::left << std::setw(17)  << "total generations"
                                << std::right  << " : " 
                                << std::left << total_generations
                                << "\n\n"

                                << std::string(2, ' ')
                                << std::left << std::setw(17)   << "objective value"
                                << std::right  << " : "
                                << std::setprecision(0) << BRIGHT_YELLOW
                                << std::left << elite_objective_value << RESET
                                << "\n\n"

                                << std::string(2, ' ')
                                << std::left << std::setw(17)   << "status"
                                << std::right  << " : "
                                << std::left << status << "\n\n";
        }


}