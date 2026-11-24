cmake_minimum_required(VERSION 3.16)
project(FinalProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(FinalProject main.cpp
        ofApp.cpp
        ofApp.h
        Missile.h
        asteroid.cpp
        asteroid.h
)
