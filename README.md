# Introduction

The Set Packing Problem (SPP) is an **NP-hard** combinatorial optimization problem that consists in selecting a subset of **activities** competing for **non-shareable resources** in order to **maximize** the total profit.


# Mathematical Formulation

![](docs/images/mathematical_formulation_spp.png)


# Resolution methods

This solver integrates a set of complementary methods for the Set Packing Problem (SPP), including monotonic and non‑monotonic local search, evolutionary procedures, selection‑based hyper‑heuristics, and exact techniques. Its HPC design relies on compact BitVector representations, loop unrolling, SIMD vectorization, and multithreading to ensure efficient large‑scale optimization.


## 1. Deterministic Greedy Construction

Builds a feasible solution by iteratively selecting the activity with the highest **heuristic score**, then discarding all resource‑conflicting activities to maintain feasibility.

## 2. Neighboorhood Operators

- **0-1 exchange** : activates a currently inactive variable (0 $\rightarrow$ 1) while preserving feasibility.


- **1-1 exchange** : replaces an active variable with an inactive one (1 $\rightarrow$ 0 and 0 $\rightarrow$ 1) while preserving feasibility.


- **1-2 exchange** : deactivates one active variable (1 $\rightarrow$ 0) and activates two inactive variables (0 $\rightarrow$ 1, 0 $\rightarrow$ 1) while preserving feasibility.


- **2-1 exchange** : deactivates two active variables (1 $\rightarrow$ 0, 1 $\rightarrow$ 0) and activates one inactive variable (0 $\rightarrow$ 1) while maintaining feasibility.


## 3. Variable Neighborhood Descent (VND)

Two VND strategies are available:

- **Classical VND** : applies neighborhoods in the order 1‑2 $\rightarrow$ 1‑1 $\rightarrow$ 2‑1 $\rightarrow$ 0‑1.
This variant is extremely fast and evaluates each neighborhood until no further improvement is possible.

- **Intensified VND** : applies neighborhoods in the order 1‑1 $\rightarrow$ 1‑2 $\rightarrow$ 2‑1 $\rightarrow$ 0‑1.
It produces slightly better solutions on most difficult benchmark instances (notably *pb_1000rnd0700.dat* and *pb_2000rnd0700.dat*), but requires significantly more exploration time.


Although the intensified strategy often yields better results on challenging cases, it does not systematically outperform the classical VND — which is the default mode and remains highly competitive thanks to its exceptional speed.



## 9. High‑Performance Computing (HPC) Enhancements

Since both the decision variables and the constraint matrix are purely binary, the solver uses a **compact BitVector representation** to encode solutions and conflicts efficiently.
This enables fast bitwise operations for feasibility checks and neighborhood evaluation.

To further accelerate computation, the solver integrates **SIMD (Single Instruction, Multiple Data)** vectorization, **loop unrolling**, and **CPU multi‑threading**.

These HPC techniques significantly reduce the cost of conflict detection and neighborhood exploration, especially on large and sparse SPP instances.


# Benchmark Instances


All benchmark instances used in this project come from the well‑known **Set Packing Problem (SPP)** library introduced by **Xavier Delorme & Xavier Gandibleux** as part of their work on railway capacity evaluation and combinatorial optimization.

[Benchmark SPP – Delorme & Gandibleux](https://www.emse.fr/%7Edelorme/SetPacking.html#BOSPP)

This dataset is widely used in the literature for evaluating heuristics, metaheuristics, and exact methods for the **Set Packing Problem**.  
It contains **64 mono‑objective instances**, generated with controlled parameters (number of variables, number of constraints, density, and maximum number of non‑zero coefficients per constraint).  
These instances range from **small and easy** (100 variables) to **very large and challenging** (2000 variables and up to 10 000 constraints).

In particular, instances with **low density** and **high Max‑One values**—such as *pb_1000rnd0700* and *pb_2000rnd0700*—are known to be among the most difficult and are commonly used to stress‑test construction heuristics, local search procedures, and exact solvers.






>**Note** : **All instances follow the same standardized SPP input format, and the SPP‑Solver is designed to read this format directly without any preprocessing**.





### Run Greedy algorithm (deterministic construction + VND local search)

```bash
./bin/spp_solver --algorithm=greedy --instance=benchmarks/pb_1000rnd0700.dat --simd --verbose 
```
- **--simd** (optional) : Activates SIMD‑optimized kernels for fast feasibility evaluation in local search neighborhoods.


- **--verbose** (optional) : Prints neighborhood exploration details.

![](docs/images/run_greedy.png)




# References

The theoretical foundations and algorithmic components implemented in this solver rely on established works in combinatorial optimization and metaheuristics. Key references include :


- **Xavier Delorme, Xavier Gandibleux, Joaquin Rodriguez** — *GRASP for Set Packing Problems*.  
  *European Journal of Operational Research*, 153(3), 564–580, 2004.

- **Xavier Delorme** — *Modélisation et résolution de problèmes liés à l’exploitation d’infrastructures ferroviaires*.  
  PhD Thesis, Université de Valenciennes, 2003.

- **Jacques Teghem** — *Recherche Opérationnelle, Tome 1*.  
  Éditions Ellipses, 2012.

- **Edmund K. Burke, Michel Gendreau, Matthew Hyde, Graham Kendall, Gabriela Ochoa, Ender Özcan, Rong Qu** — *Hyper‑heuristics: A Survey of the State of the Art*.  
  *Journal of the Operational Research Society*, 2013.

- **Johann Dréo, Alain Petrowski, Patrick Siarry, Éric Taillard** — *Métaheuristiques pour l’optimisation difficile*.  
  Eyrolles, 2006.
