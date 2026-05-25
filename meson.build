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

project('SPP-Solver',
        'cpp',
        version : '0.0.1',
        default_options : [
            'cpp_std=c++20',
            'warning_level=3'
        ])



# header files 
inc = include_directories('include')


# getting options 

# CPU physical cores
nb_physical_cores = get_option('NUMBER_PHYSICAL_CORES')


# CPU ISA 
has_x86 = get_option('HAS_X86')
has_arm = get_option('HAS_ARM')

# SIMD extensions 
has_sse2 = get_option('HAS_SSE2')
has_avx2 = get_option('HAS_AVX2')
has_avx512f = get_option('HAS_AVX512F')
has_neon = get_option('HAS_NEON')


if has_x86

    if has_sse2
        add_project_arguments('-DHAS_SSE2=true', language : 'cpp')
    endif

    if has_avx2
        add_project_arguments('-DHAS_AVX2=true', language : 'cpp')
    endif

    if has_avx512f
        add_project_arguments('-DHAS_AVX512F=true', language : 'cpp')
    endif

    if has_sse2 or has_avx2 or has_avx512f
        add_project_arguments('-DUSE_SIMD=true', language : 'cpp')
    endif 

endif


if has_arm

    if has_neon
        add_project_arguments('-DHAS_NEON=true', language : 'cpp')
        add_project_arguments('-DUSE_SIMD=true', language : 'cpp')
    endif

endif


# MILP backends

# Gurobi
has_gurobi = get_option('HAS_GUROBI')
gurobi_home = get_option('GUROBI_HOME')
gurobi_lib = get_option('GUROBI_LIB_NAME')


# Hexaly
has_hexaly = get_option('HAS_HEXALY')
hexaly_home = get_option('HEXALY_HOME')
hexaly_lib = get_option('HEXALY_LIB_NAME')

# Highs
has_highs = get_option('HAS_HIGHS')



# adding options 

add_project_arguments(
    '-DNUMBER_PHYSICAL_CORES=' + nb_physical_cores,
    language : 'cpp'
)


if has_gurobi

    add_project_arguments('-DHAS_GUROBI=true', language : 'cpp')
    gurobi_dep = declare_dependency(
        include_directories : include_directories(gurobi_home + '/include'),
        link_args : [
            '-L' + gurobi_home + '/lib',
            '-l' + gurobi_lib,
            '-lgurobi_c++',
        ],
    )


else

    add_project_arguments('-DHAS_GUROBI=false', language : 'cpp')
    gurobi_dep = dependency('', required : false)

endif






if has_hexaly

    add_project_arguments('-DHAS_HEXALY=true', language : 'cpp')
    hexaly_dep = declare_dependency(
        include_directories : include_directories(hexaly_home + '/include'),
        link_args : [
            '-L' + hexaly_home + '/bin',
            '-l' + hexaly_lib
        ],
        dependencies : [ dependency('threads') ]
    )


else

    add_project_arguments('-DHAS_HEXALY=false', language : 'cpp')
    hexaly_dep = dependency('', required : false)

endif






if has_highs

    add_project_arguments('-DHAS_HIGHS=true', language : 'cpp')
    highs_dep = dependency('highs', required : true, include_type : 'system')


else

    add_project_arguments('-DHAS_HIGHS=false', language : 'cpp')
    highs_dep = dependency('', required : false)

endif




# Enable the usage of the Branch and Bound algorithm

if has_gurobi or has_highs
    add_project_arguments('-DUSE_BRANCH_AND_BOUND=true', language : 'cpp')
endif


# Enable the usage of MILP solvers

if has_gurobi or has_highs or has_hexaly
    add_project_arguments('-DUSE_MILP=true', language : 'cpp')
endif



# source files 
subdir('src')
