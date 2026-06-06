# Introduction

The Set Packing Problem (SPP) is an **NP-hard** combinatorial optimization problem that consists in selecting a subset of **activities** competing for **non-shareable resources** in order to **maximize** the total profit.


# Mathematical Formulation

![](docs/images/mathematical_formulation_spp.png)


# Resolution methods

This solver integrates a set of complementary methods for the Set Packing Problem (SPP), including monotonic and non‑monotonic local search, evolutionary procedures, selection‑based hyper‑heuristics, and exact techniques. Its HPC design relies on compact BitVector representations, loop unrolling, SIMD vectorization, and multithreading to ensure efficient large‑scale optimization.