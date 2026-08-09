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
 * @file Crossover.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/Crossover.hpp"


namespace spp{


    double findMinimumFitness(const std::vector<double> &population_fitness){

        double minimum_fitness = std::numeric_limits<double>::max();

        for(double fitness : population_fitness){

            if(fitness < minimum_fitness){minimum_fitness = fitness;}
        }

        return minimum_fitness;
    }




    std::vector<double> computeQuadraticBiasedFitness(const std::vector<double> &population_fitness){

        double minimum_fitness = findMinimumFitness(population_fitness);

        std::vector<double> biased_population_fitness(population_fitness.size());

        for(size_t i = 0; i < population_fitness.size(); i++){

            biased_population_fitness[i] = (population_fitness[i] + 1e-6) - minimum_fitness;

            biased_population_fitness[i] *= biased_population_fitness[i];

        }

        return biased_population_fitness;
    }





    void normalizeBiasedFitness(std::vector<double> &biased_population_fitness){

        // computation of the cumulative fitness
        
        double cumulative_fitness = 0.0;

        for(double fitness : biased_population_fitness){cumulative_fitness += fitness;}

        // computation of the inverse cumulative fitness
        double inverse_cumulative_fitness = 1.0 / (cumulative_fitness + 1e-9);

        for(size_t i = 0; i < biased_population_fitness.size(); i++){

            biased_population_fitness[i] = biased_population_fitness[i] * inverse_cumulative_fitness;
        }

    }





    std::vector<double> computeCumulativeQuadraticBiasedFitness(const std::vector<double> &population_fitness){

        // computation of quadratic biased fitness
        std::vector<double> biased_population_fitness = computeQuadraticBiasedFitness(population_fitness);

        // fitness normalization
        normalizeBiasedFitness(biased_population_fitness);

        double cumulative_fitness = 0.0;

        std::vector<double> cumulative_population_fitness(population_fitness.size());

        for(size_t i = 0; i < biased_population_fitness.size(); i++){

            cumulative_fitness += biased_population_fitness[i];

            cumulative_population_fitness[i] = cumulative_fitness;

        }

        return cumulative_population_fitness;
    }





    std::vector<size_t> computeIndividualsCrossOverParticipation(const std::vector<double> &cumulative_population_fitness,
                                                                 const Params &params){

        size_t total_crossover_participation = static_cast<size_t>(std::floor((cumulative_population_fitness.size() * params.crossover_rate) / 2.0) * 2.0);

        double inverse = 1.0 / (total_crossover_participation + 1e-9);

        // initialization of the thread random number generator
        auto& rng = getThreadLocalRng();

        // initialization of the uniform real distribution
        std::uniform_real_distribution<double> dist(0.0, inverse);

        double random_number = dist(rng);

        std::vector<size_t> crossover_participation(cumulative_population_fitness.size());

        size_t i = 0;

        for(size_t j = 0; j < total_crossover_participation; j++){

            double pointer = random_number + (j * inverse);

            while((i < cumulative_population_fitness.size()) && 
                  (cumulative_population_fitness[i] < pointer )){

                    i += 1;
            }

            crossover_participation[i] += 1;
        }

        return crossover_participation;
    }

    
}