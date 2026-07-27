#!/usr/bin/env bash 

clear

# detecting the Operating System (Linux or MacOS) and the number of physical cores

KERNEL_NAME=$(uname -s)

case "$KERNEL_NAME" in 

    *Linux*)
        OS_TYPE='Linux'
        NUMBER_PHYSICAL_CORES=$(lscpu -p=core | grep -v '^#' | sort -u | wc -l)
        ARCH_TYPE=$(uname -m)
        ;;

    *Darwin*)
        OS_TYPE='MacOS'
        NUMBER_PHYSICAL_CORES=$(sysctl -n hw.physicalcpu)
        ARCH_TYPE=$(uname -m)
        ;;
    *)
        echo " Unsupported Operating System"
        echo " Operating System found : $KERNEL_NAME"
        exit 1
        ;;
esac



# CPU architecture (logical abstraction)

HAS_X86=false
HAS_ARM=false

case "$ARCH_TYPE" in
    x86_64|amd64)
        HAS_X86=true
        ;;
    arm64|aarch64)
        HAS_ARM=true
        ;;
    *)
        echo " Unsupported CPU architecture: $ARCH_TYPE"
        exit 1
        ;;
esac


# SIMD extensions

HAS_AVX2=false
HAS_NEON=false

if [ "$HAS_X86" = true ]; then
    lscpu | grep -qi avx2      && HAS_AVX2=true

elif [ "$HAS_ARM" = true ]; then
    lscpu | grep -qiE 'asimd|neon' && HAS_NEON=true
fi



# Reject ARMv7 or older

if [ "$HAS_ARM" = true ] && [ "$ARCH_TYPE" != "aarch64" ] && [ "$ARCH_TYPE" != "arm64" ]; then
    echo "ERROR: ARMv7 or older is not supported. ARMv8+ (aarch64 or arm64) required."
    exit 1
fi


# MILP solvers flags

HAS_GUROBI=false
HAS_HEXALY=false
HAS_HIGHS=false

for arg in "$@"; do 
    case "$arg" in 
        HAS_GUROBI)
            HAS_GUROBI=true
            ;;
        HAS_HEXALY)
            HAS_HEXALY=true
            ;;
        HAS_HIGHS)
            HAS_HIGHS=true
            ;;
        
    esac

done


# --------------------- GUROBI ----------------------

# If HAS_GUROBI=true, GUROBI_HOME must exist
if [ "$HAS_GUROBI" = true ]; then

    if [ -z "$GUROBI_HOME" ]; then
        echo "ERROR: HAS_GUROBI=true but GUROBI_HOME is not set in your environment."
        exit 1
    fi

    GUROBI_LIB_NAME=$(ls "$GUROBI_HOME/lib" \
        | grep -E '^libgurobi[0-9]+\.so$' \
        | sed -E 's/^lib(.*)\.so/\1/' \
        | head -n 1)

    if [ -z "$GUROBI_LIB_NAME" ]; then
        echo "ERROR: Could not detect Gurobi shared library in $GUROBI_HOME/lib"
        exit 1
    fi

fi




# -------------------- HEXALY ----------------------

# if HAS_HEXALY=true, HX_HOME must exist
if [ "$HAS_HEXALY" = true ]; then

    if [ -z "$HX_HOME" ]; then
        echo "ERROR: HAS_HEXALY=true but HX_HOME is not set in your environment."
        exit 1
    fi

    HEXALY_LIB_NAME=$(ls "$HX_HOME/bin" \
        | grep -E '^libhexaly[0-9]+\.so$' \
        | sed -E 's/^lib(.*)\.so/\1/' \
        | head -n 1)

    if [ -z "$HEXALY_LIB_NAME" ]; then
        echo "ERROR: Could not detect Hexaly shared library in $HX_HOME/bin"
        exit 1
    fi

fi




# --------------------- HIGHS ----------------------

if [ "$HAS_HIGHS" = true ]; then

    if ! pkg-config --exists highs; then
        echo "ERROR: HAS_HIGHS=true but HiGHS is not found by pkg-config."
        exit 1
    fi

fi




set -e # abort the installation if a command failed


if [ ! -d "build" ]
# case of a NON-existing build folder
then 
    meson setup build --prefix="$PWD" \
                      --buildtype=debug \
                      -DNUMBER_PHYSICAL_CORES="$NUMBER_PHYSICAL_CORES" \
                      -DHAS_GUROBI="$HAS_GUROBI" \
                      -DGUROBI_HOME="$GUROBI_HOME" \
                      -DGUROBI_LIB_NAME="$GUROBI_LIB_NAME" \
                      -DHAS_HEXALY="$HAS_HEXALY" \
                      -DHEXALY_HOME="$HX_HOME" \
                      -DHEXALY_LIB_NAME="$HEXALY_LIB_NAME" \
                      -DHAS_HIGHS="$HAS_HIGHS" \
                      -DHAS_X86="$HAS_X86" \
                      -DHAS_ARM="$HAS_ARM" \
                      -DHAS_AVX2="$HAS_AVX2" \
                      -DHAS_NEON="$HAS_NEON" \
                      || exit 1
else
# when the folder build already exists
    meson setup --reconfigure build \
                --prefix="$PWD" \
                --buildtype=debug \
                -DNUMBER_PHYSICAL_CORES="$NUMBER_PHYSICAL_CORES" \
                -DHAS_GUROBI="$HAS_GUROBI" \
                -DGUROBI_HOME="$GUROBI_HOME" \
                -DGUROBI_LIB_NAME="$GUROBI_LIB_NAME" \
                -DHAS_HEXALY="$HAS_HEXALY" \
                -DHEXALY_HOME="$HX_HOME" \
                -DHEXALY_LIB_NAME="$HEXALY_LIB_NAME" \
                -DHAS_HIGHS="$HAS_HIGHS" \
                -DHAS_X86="$HAS_X86" \
                -DHAS_ARM="$HAS_ARM" \
                -DHAS_AVX2="$HAS_AVX2" \
                -DHAS_NEON="$HAS_NEON" \
                || exit 1
fi



echo " "
echo " --- project configuration..."
echo " "
echo "      CPU architecture         : $ARCH_TYPE"
echo "      Operating System found   : $OS_TYPE"
echo "      Number of physical cores : $NUMBER_PHYSICAL_CORES"
echo " "
echo " --- compiling..."
echo " "
meson compile -C build
echo " "
echo " --- the solver is installed in $PWD/bin/ "
echo " "
meson install -C build
echo " "
echo " --- Installation completed. the SPP-solver is ready!"
echo " "