cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(dbgsrv)

add_executable(dbgsrv main.cpp)

set_target_properties(dbgsrv PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON)
