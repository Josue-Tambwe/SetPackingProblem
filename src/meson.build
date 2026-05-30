#------------------------------------------------------------------------------
# SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
#              and efficient BitVector techniques.
#------------------------------------------------------------------------------
# Copyright (c) 2026 Josué Tambwe
#               Nantes Université, France
#------------------------------------------------------------------------------
# This software is released under the MIT License.
# See the LICENSE file for more details.
#------------------------------------------------------------------------------

# subdirectories

subdir('dataStructures')
subdir('solver')
subdir('output')
subdir('input')
subdir('algorithms')
subdir('hpc')





# combining all source files

all_sources = data_struc_sources + solver_sources + output_sources + input_sources + algorithm_sources + hpc_sources



# final binary file 

executable('spp_solver',
            all_sources,
            include_directories : inc,
            dependencies : [gurobi_dep, hexaly_dep, highs_dep],
            install : true,
            install_dir : 'bin')