cmake_minimum_required(VERSION 3.15)

project(borin_raspored VERSION 1.0.0 LANGUAGES CXX)

option(BORIN_RASPORED_BUILD_TESTS "Build the test suite" ON)

# Leaving the type blank allows -DBUILD_SHARED_LIBS=ON/OFF
add_library(borin_raspored_lib src/utf8_borin.cpp src/borin_utf8_cyr.cpp src/borin_utf8_lat.cpp)

target_compile_features(borin_raspored_lib PUBLIC cxx_std_20)
set_target_properties(borin_raspored_lib PROPERTIES 
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_include_directories(borin_raspored_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(BORIN_RASPORED_BUILD_TESTS)
    enable_testing()
    
    add_executable(borin_raspored_tests tests/borin_raspored.t.cpp)
    
    target_link_libraries(borin_raspored_tests PRIVATE borin_raspored_lib)
    
    add_test(NAME Basic COMMAND borin_raspored_tests)
endif()
