project(DCT)

set(SRCS
    main.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories(${DCT_DIR}/DCTLib)

target_link_libraries(${PROJECT_NAME} DCTLib)
