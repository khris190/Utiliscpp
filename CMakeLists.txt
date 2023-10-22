cmake_minimum_required(VERSION 3.27.0)
project(MY_UTILS LANGUAGES CXX CUDA VERSION 1.1.5)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
  set(CMAKE_CUDA_ARCHITECTURES 61)
endif(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)

file(GLOB INCLUDES
    include/**/*.hpp
    include/**/*.h
)

file(GLOB SOURCES
    include/**/*.cpp
    include/**/*.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME}) 
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
set(MY_UTILS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/headerOnly )
set(MY_UTILS_INCLUDE ${MY_UTILS_INCLUDE} PARENT_SCOPE)
include_directories(${MY_UTILS_INCLUDE})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${MY_UTILS_INCLUDE})
