cmake_minimum_required(VERSION 4.0)
project(FinalProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(FinalProject main.cpp
        ofApp.cpp
        ofApp.h
        Missile.h
        asteroid.cpp
        asteroid.h
)
