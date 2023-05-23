
include_directories(
    PUBLIC ${PARENT_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library( server
    server.hpp
    server.cpp
    main.cpp
)