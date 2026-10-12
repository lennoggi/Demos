cmake_minimum_required(VERSION 3.18)

# ***** User-defined variables *****
set(PROJ "AMReX_Hello")
set(SRC Main.cc)
set(EXE ${PROJ})
##set(INC "include")
set(AMREX_INSTALL_DIR "/home/nlennoggi210076/Git_repositories/amrex/install")
# *********************************

# NOTE: include C as language because find_package(AMReX) calls
 #      find_package(MPI LANGUAGES C)
project(${PROJ} VERSION 1.0.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AMReX_ROOT ${AMREX_INSTALL_DIR})
find_package(AMReX REQUIRED COMPONENTS
    MPI
    OMP
    ##HIP
)

add_executable(${EXE} ${SRC})
##target_include_directories(${EXE} PRIVATE ${INC})
target_link_libraries(${EXE} PRIVATE AMReX::amrex)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

install(TARGETS ${EXE}
        RUNTIME DESTINATION bin
)
