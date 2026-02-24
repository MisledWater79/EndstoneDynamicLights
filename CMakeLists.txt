cmake_minimum_required(VERSION 3.25)
project(dynamic-lights)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
        src/BinStream.cpp
        src/DynamicLights.cpp
        src/Entry.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++ -fPIC")
    link_libraries(-static-libgcc -static-libstdc++ libc++.a libc++abi.a)
endif ()

include(FetchContent)
FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG v0.11.1
)
FetchContent_MakeAvailable(endstone)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

endstone_add_plugin(${PROJECT_NAME} ${SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
