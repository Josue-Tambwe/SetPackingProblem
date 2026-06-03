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
 * @file Solution.cpp
 * @author Josué Tambwe
 * @date 25 May 2026
 */

 #include "dataStructures/Solution.hpp"

 namespace spp{

    // constructor 
    Solution::Solution(const Instance &instance):
      solution_vector(instance.getNbVars()),
      consumed_resources(instance.getNbConstraints()){

      this->objective_value = 0;
      this->status = Status::UNKNOWN;
    }

        
    // getters 

    size_t Solution::getNbVars() const {return solution_vector.getNbLogicalBits();}
    size_t Solution::getNbNonZeroVars() const {return solution_vector.getNbNonZeroBits();}
    size_t Solution::getNbVarWords() const {return solution_vector.getNbWords();}

    std::vector<int> Solution::getNonZeroVarsIndexes() const {return solution_vector.getNonZeroBitIndexes();}
    std::vector<int> Solution::getZeroVarsIndexes() const {return solution_vector.getZeroBitIndexes();}

    size_t Solution::getNbConstraints() const{return consumed_resources.getNbLogicalBits();}
    size_t Solution::getNbConsumedResources() const {return consumed_resources.getNbNonZeroBits();}
    size_t Solution::getNbConsumedResourceWords() const {return consumed_resources.getNbWords();}

    std::vector<int> Solution::getNonZeroConsumedResourcesIndexes() const {return consumed_resources.getNonZeroBitIndexes();}
    std::vector<int> Solution::getZeroConsumedResourcesIndexes() const {return consumed_resources.getZeroBitIndexes();}
    std::vector<std::uint64_t>& Solution::getConsumedResourcesData(){return this->consumed_resources.getData();}
    std::uint64_t* Solution::getConsumedResourcesPointerToData(){return this->consumed_resources.getPointerToData();}


    Status Solution::getStatus() const {return status;}


    bool Solution::isFeasible(const Instance &instance){

      std::vector<int> nonzeros_vars = this->getNonZeroVarsIndexes();
      size_t nb_words = this->getNbConsumedResourceWords();
      std::vector<std::uint64_t> resources_used(nb_words, 0ULL);
      const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

      for(int var : nonzeros_vars){

        const std::vector<std::uint64_t>& resources_required = resource_requirements[var].getData();
        
        // checks conflicts
        for(size_t word = 0; word < nb_words; word++){

          if((resources_used[word] & resources_required[word]) != 0ULL) return false;
        }

        // accumulates resources used
        for(size_t word = 0; word < nb_words; word++){

          resources_used[word] |= resources_required[word];
        }

      }

      return true;
    }


    std::int64_t Solution::getObjectiveValue(const Instance &instance){
      
      computeObjectiveValue(instance);
      return this->objective_value;
    }


    

    // setters

    void Solution::activateVar(int var_index, const Instance &instance){

      // variable activation
      solution_vector.activate(var_index);

      const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

      // update of the consumed resources 
      std::vector<int> resources_required = resource_requirements[var_index].getNonZeroBitIndexes();

      for(int index : resources_required){this->consumed_resources.activate(index);}

    }



    void Solution::deactivateVar(int var_index, const Instance &instance){

      // variable deactivation
      solution_vector.deactivate(var_index);

      const std::vector<BitVector>& resource_requirements = instance.getResourceRequirements();

      // update (free) the consumed resources
      std::vector<int> resources_required = resource_requirements[var_index].getNonZeroBitIndexes();

      for(int index : resources_required){this->consumed_resources.deactivate(index);}

    }



    void Solution::setStatus(Status status){this->status = status;}

    

    void Solution::computeObjectiveValue(const Instance &instance){

      const std::vector<int>& profit_vector = instance.getProfitVector();
      std::vector<int> nonzeros_indexes = this->getNonZeroVarsIndexes();

      this->objective_value = 0;
      for(int index : nonzeros_indexes){this->objective_value += profit_vector[index];}
    }



    void Solution::printVars() const {

      std::cout << " variables = [";
      solution_vector.print();
      std::cout << "] \n \n";

      solution_vector.printNonZeroIndexes();
      std::cout << "\n \n";         

    }


    void Solution::printConsumedResources() const {

      std::cout << " consumed resources = [";
      consumed_resources.print();
      std::cout << "] \n \n";

      consumed_resources.printNonZeroIndexes();
      std::cout << "\n \n"; 

    }



    void Solution::print(const Instance &instance) {

      std::cout << "\n" << std::string(100, '*') << "\n"
                << std::setw(50) << std::right << "Solution" << "\n"
                << std::string(100, '*') << "\n"
                << " number of ressources used   : " << this->getNbConsumedResources() << "\n"
                << " number of nonzero variables : " << this->getNbNonZeroVars() << " \n\n"
                << " objective value             : " << this->getObjectiveValue(instance) << "\n\n";

      this->solution_vector.printNonZeroIndexes();
      std::cout << std::string(100, '*') << "\n" ; 
                

    }
 }