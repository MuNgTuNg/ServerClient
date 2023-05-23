cmake_minimum_required(VERSION 3.13.4)


project(ShbServer LANGUAGES CXX ) #ShbOGEngine = ${PROJECT_NAME}

add_executable(${PROJECT_NAME} src/main.cpp )

include_directories(
    PUBLIC ${PARENT_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    src
)

add_subdirectory(
    src
)

target_link_libraries(  ${PROJECT_NAME} 
    server
)

