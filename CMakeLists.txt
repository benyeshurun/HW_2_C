cmake_minimum_required(VERSION 3.19)
project(untitled C)

set(CMAKE_C_STANDARD 90)

add_library(untitled kmeans..c kmeans..h)