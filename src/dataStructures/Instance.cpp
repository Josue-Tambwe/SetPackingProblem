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

            file >> this->nb_constraints;
            this->constraint_matrix.resize(nb_constraints);

            file >> this->nb_vars;
            this->profit.resize(nb_vars);

            int max_profit = 0;
            int min_profit = std::numeric_limits<int>::max();

            // profit vector
            for(size_t j = 0; j < nb_vars; j++){

                file >> this->profit[j];

                if(profit[j] > max_profit){max_profit = profit[j];}
                if(profit[j] < min_profit){min_profit = profit[j];}
            }

            // instance statitics 
            int nonzeros_count = 0;
            int max_nonzeros_in_row = 0;

            // constraint matrix 
            for(size_t i = 0; i < nb_constraints; i++){

                int nonzeros_in_row;
                file >> nonzeros_in_row;

                constraint_matrix[i].resize(nonzeros_in_row);
                for(int k = 0; k < nonzeros_in_row; k++){

                    file >> constraint_matrix[i][k];
                    // bringing index to 0-based indexing
                    constraint_matrix[i][k] -= 1;
                }

                if(nonzeros_in_row > max_nonzeros_in_row){max_nonzeros_in_row = nonzeros_in_row;}
                nonzeros_count += nonzeros_in_row;
            }

            float density = (nonzeros_count * 100.0f )/ ((nb_vars * nb_constraints) + epsilon);

            // initialization of the instance statistics member
            this->stats = {nb_vars,
                           nb_constraints,
                           static_cast<size_t>(nonzeros_count),
                           static_cast<size_t>(max_nonzeros_in_row),
                           density,
                           max_profit,
                           min_profit};

            
            computeResourceRequirements();
        }
    }



    void Instance::computeResourceRequirements(){

        this->resource_requirements.resize(nb_vars, BitVector(nb_constraints));

        for(int row = 0; row < static_cast<int>(this->nb_constraints); row++){

            for(int var_index : this->constraint_matrix[row]){

                this->resource_requirements[var_index].activate(row);
            }
        }


    }

    

    // getters 
    size_t Instance::getNbVars() const {return nb_vars;}
    size_t Instance::getNbConstraints() const {return nb_constraints;}

    const std::vector<int>& Instance::getProfitVector() const {return profit;}
    const std::vector<std::vector<int>>& Instance::getConstraintMatrix() const {return constraint_matrix;}
    const InstanceStatistics& Instance::getStatistics() const {return stats;}
    const std::vector<BitVector>& Instance::getResourceRequirements() const {return resource_requirements;}



    void Instance::print() const{
        std::cout << "\n" << std::string(100, '*') << "\n"
                  << std::setw(50) << std::right << "Instance" << "\n"
                  << std::string(100, '*') << "\n"
                  << " number of  variables          : " << nb_vars << "\n"
                  << " number of constraints         : " << nb_constraints << "\n"
                  << " profit range                  : " << stats.min_profit
                  << " - " << stats.max_profit << "\n \n"
                  << " nonzeros count                : " << stats.total_nonzeros << "\n"
                  << " max nonzeros per row          : " << stats.max_nonzeros_per_row << "\n"
                  << " constraint matrix density (%) : " << std::setprecision(3) << stats.density
                  << " \n \n \n";

        std::cout << std::string(100, '*') << "\n"
                  << " Profit vector : \n\n ";
        for(int element : profit){std::cout << element << " ";}
        std::cout << "\n" << std::string(100, '-')  << "\n";


        std::cout << " \n \n" << std::string(100, '*') << "\n" 
                  << " Constraint matrix : \n"
                  << std::string(100, '-') << "\n\n" ; 


        for(size_t i = 0; i < nb_constraints; i++){

            std::cout << " row " << (i+1) << " : ";
            for(int element : constraint_matrix[i]){
                std::cout  << (element + 1) << " ";
            }
            std::cout << "\n\n";
        }
        printResourceRequirements();

    }


    void Instance::printResourceRequirements() const{

        std::cout << std::string(100, '*') << "\n" ; 
        std::cout << " Resource requirements : \n";

        for(size_t var_index = 0; var_index < resource_requirements.size(); var_index++){

            std::cout << std::string(100, '-') << "\n" ; 
            std::cout << " variable " << (var_index + 1) << " = [";

            resource_requirements[var_index].print();
            std::cout << "] \n \n";

            resource_requirements[var_index].printNonZeroIndexes();
        }
        std::cout << std::string(100, '*') << "\n" ; 

    }


 }