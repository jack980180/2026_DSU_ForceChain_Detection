cmake_minimum_required(VERSION 4.0)
project(ForceChainScanner)

set(CMAKE_CXX_STANDARD 20)

add_executable(ForceChainScanner
        src/DisjointSet.h
        src/DisjointSet.cpp
        src/vector.cpp
        src/vector.h
        test/test_vector.cpp
        test/test_disjoint_set.cpp
        src/Particle.cpp
        src/Particle.h
        src/SimulationBox.cpp
        src/SimulationBox.h
        src/main.cpp
        src/SimulationDriver.cpp
        src/SimulationDriver.h
        plot/plotter.cpp)
