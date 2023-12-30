cmake_minimum_required(VERSION 3.27.0)
project(MY_UTILS LANGUAGES CXX VERSION 1.1.5)

file(GLOB INCLUDES
    *.hpp
    *.h
)

file(GLOB SOURCES
    *.cpp
    *.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME}) 
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
set(MY_UTILS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/headerOnly )
set(MY_UTILS_INCLUDE ${MY_UTILS_INCLUDE} PARENT_SCOPE)
include_directories(${MY_UTILS_INCLUDE})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PUBLIC ${MY_UTILS_INCLUDE})