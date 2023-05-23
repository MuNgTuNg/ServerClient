include_directories(
    PUBLIC ${PARENT_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library( client
    client.hpp
    client.cpp
    main.cpp
)