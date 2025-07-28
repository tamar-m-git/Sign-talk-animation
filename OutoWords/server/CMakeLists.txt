cmake_minimum_required(VERSION 3.10)
project(OutoWordsServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# אם תרצי טרגוט Windows ספציפי
# add_compile_definitions(_WIN32_WINNT=0x0601)
add_definitions(-D_WIN32_WINNT=0x0601)

find_package(crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(server
    src/main.cpp
    src/Outomation.cpp
    src/OutoGraph.cpp
    src/Vertex.cpp
    src/MovingVertex.cpp
)

target_link_libraries(server
    PRIVATE
        Crow::Crow
        nlohmann_json::nlohmann_json
)
