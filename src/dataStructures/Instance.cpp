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
 * @file Instance.cpp
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #include "dataStructures/Instance.hpp"

 namespace spp{

    // constructor 
    Instance::Instance(Params &params){

        // opening the file 
        std::ifstream file(params.instance_path);

        if(!file.is_open()){Logger log; log.error("Error in opening the file");}

        else{

            file >> nb_constraints;
            this->constraint_matrix.resize(nb_constraints);

            file >> nb_vars;
            this->profit.resize(nb_vars);

            // profit vector
            for(size_t j = 0; j < nb_vars; j++){file >> this->profit[j];}

           

            // instance statitics 
            int nonzeros_count = 0;
            int max_nonzeros_in_row = 0;


            // constraint matrix 
            for(size_t i = 0; i < nb_constraints; i++){

                int nonzeros_in_row;
                file >> nonzeros_in_row;

                constraint_matrix[i].resize(nonzeros_in_row);
                for(int k = 0; k < nonzeros_in_row; k++){file >> constraint_matrix[i][k];}

                if(nonzeros_in_row > max_nonzeros_in_row){max_nonzeros_in_row = nonzeros_in_row;}
                nonzeros_count += nonzeros_in_row;
            }


            this->stats = {nb_vars,
                           nb_constraints,
                           static_cast<size_t>(nonzeros_count),
                           static_cast<size_t>(max_nonzeros_in_row)};

            

        }
    }

    // getters 
    size_t Instance::getNbVars() const {return nb_vars;}
    size_t Instance::getNbConstraints() const {return nb_constraints;}

    const std::vector<int>& Instance::getProfitVector() const {return profit;}
    const std::vector<std::vector<int>>& Instance::getConstraintMatrix() const {return constraint_matrix;}
    const InstanceStatistics& Instance::getStatistics() const {return stats;}



    void Instance::print() const{

        std::cout << " nb vars : " << nb_vars << "\n"
                  << " nb constraint : " << nb_constraints << " \n \n";

        std::cout << " profit vector : [";
        for(int element : profit){std::cout << element << " ";}
        std::cout << "] \n \n"
                  << "constraint matrix : \n \n";

        for(size_t i = 0; i < nb_constraints; i++){

            for(int element : constraint_matrix[i]){
                std::cout  << element << " ";
            }
            std::cout << "\n";
        }
    }

 }