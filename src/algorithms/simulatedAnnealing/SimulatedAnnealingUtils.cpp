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
 * @file SimulatedAnnealingUtils.cpp
 * @author Josué Tambwe
 * @date 2 August 2026
 */

#include "algorithms/simulatedAnnealing/SimulatedAnnealingUtils.hpp"

namespace spp{


    double polynomialBoltzmannApproximation(double x){

        return (1.0 / (1.0 + x + (x * x)));

    }




    bool acceptanceCriterion(double degradation, double current_temperature){

        // initialization of the thread random number generator
        auto& rng = getThreadLocalRng();

        // initialization of the uniform real distribution in [0,1]
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        double random_number = dist(rng);

        // computation of the threshold
        double threshold = polynomialBoltzmannApproximation(degradation / (current_temperature + 1e-9));

        return (random_number <= threshold);

    }





    bool stoppingCriteriaSimulatedAnnealing(double current_time, 
                                            size_t current_iteration,
                                            double current_temperature,
                                            const Params &params){

        if(params.use_time_limit && !params.use_max_iterations){return current_time >= params.time_limit;}

        if(!params.use_time_limit && params.use_max_iterations){return current_iteration >= params.nb_max_iterations;}

        else{
            return (current_iteration >= params.nb_max_iterations) || 
                   (current_time >= params.time_limit) ||
                   (current_temperature <= params.final_temperature);
        }


    }


}