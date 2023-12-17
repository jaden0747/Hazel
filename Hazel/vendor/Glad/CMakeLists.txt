cmake_minimum_required(VERSION 3.21)

project(Glad)

set(CMAKE_CXX_FLAGS "/MT")

add_library(Glad STATIC 
  src/glad.c 
  include/glad/glad.h 
  include/KHR/khrplatform.h)

target_include_directories(Glad PUBLIC include)