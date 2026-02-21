cmake_minimum_required(VERSION 3.15)
project(PaperEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SDL2
find_package(SDL2 REQUIRED)

# PaperEngine library
file(GLOB_RECURSE PAPER_SOURCES "src/*.cpp")
file(GLOB_RECURSE PAPER_HEADERS "include/*.h")

# Explicitly add new features
set(PAPER_SOURCES 
    ${PAPER_SOURCES}
    src/PaperEngine.cpp
    src/NewFeatures.cpp
)

add_library(PaperEngine STATIC ${PAPER_SOURCES} ${PAPER_HEADERS})

target_include_directories(PaperEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(PaperEngine PUBLIC
    ${SDL2_LIBRARIES}
)

# Compiler warnings
if(MSVC)
    target_compile_options(PaperEngine PRIVATE /W4)
else()
    target_compile_options(PaperEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Example executable
add_executable(PaperExample examples/example_game.cpp)
target_link_libraries(PaperExample PRIVATE PaperEngine)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Installation
install(TARGETS PaperEngine
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/PaperEngine
)
