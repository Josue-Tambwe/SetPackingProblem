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
 * @file Mutation.cpp
 * @author Josué Tambwe
 * @date 11 August 2026
 */

#include "algorithms/geneticAlgorithm/Mutation.hpp"


namespace spp{


    size_t getRandomIndex(const std::unordered_set<int> &non_zero_vars){

        auto& rng = getThreadLocalRng();
        std::uniform_int_distribution<size_t> dist(0, (non_zero_vars.size() - 1));

        size_t index = dist(rng);

        std::unordered_set<int>::const_iterator iterator = non_zero_vars.begin();
        std::advance(iterator, index);

        return static_cast<size_t>(*iterator);
    }





    void computeDisturbedSolution(Solution &solution,
                                  const Instance &instance){

        // getting non-zero variables within the current solution
        std::unordered_set<int> non_zero_vars = computeInitialSolutionNonZeroVarsIndexes(solution);

        size_t nb_vars_to_deactivate = static_cast<size_t>(std::floor(non_zero_vars.size() * 0.2));

        for(size_t i = 0; i < nb_vars_to_deactivate; i++){

            // getting a random variable index
            int index = getRandomIndex(non_zero_vars);

            // deactivating the non-zero variable
            solution.deactivateVar(index, instance);

            // removing the deactivated variable from the set of non-zero variables
            non_zero_vars.erase(index);

        }
    }





    void performIteratedLocalSearchMutation(Solution &child,
                                            const Params &params,
                                            const Instance &instance){

        // disturbance of the child
        computeDisturbedSolution(child,
                                 instance);

        // VND local search on the disturbed child
        variableNeighborhoodDescent(params.use_intensification,
                                    params, 
                                    child, 
                                    instance);

    }





    std::vector<size_t> selectIndexesToMutate(size_t nb_children, const Params &params){

        // initialization of all children indexes

        std::unordered_set<size_t> all_children_indexes;

        for(size_t i = 0; i < nb_children; i++){all_children_indexes.insert(i);}

        size_t nb_children_to_mutate = static_cast<size_t>(std::floor(nb_children * params.mutation_rate));

        std::vector<size_t> children_to_mutate(nb_children_to_mutate);

        for(size_t i = 0; i < nb_children_to_mutate; i++){

            // getting a random child index
            size_t index = getRandomIndex(all_children_indexes);

            children_to_mutate[i] = index;

            // removing the child index from the set of non-mutated children
            all_children_indexes.erase(index);
        }

        return children_to_mutate;
    }





    void performMutationSingleThread(int start,
                                     int end,
                                     std::vector<Solution> &children,
                                     const std::vector<size_t> &children_to_mutate_indexes,
                                     const Params &params,
                                     const Instance &instance){

        for(int i = start; i <= end; i++){

            performIteratedLocalSearchMutation(children[children_to_mutate_indexes[i]],
                                               params,
                                               instance);

        }

    }





    void performMutation(std::vector<Solution> &children,
                         const std::vector<size_t> &children_to_mutate_indexes,
                         const Params &params,
                         const Instance &instance){


        int work_size = static_cast<int>(children_to_mutate_indexes.size());

        int nb_threads_used = std::min(params.nb_threads, work_size);

        std::vector<std::thread> workers(nb_threads_used);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            workers[id] = std::thread(performMutationSingleThread,
                                      start,
                                      end,
                                      std::ref(children),
                                      std::ref(children_to_mutate_indexes),
                                      std::ref(params),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

    }


}