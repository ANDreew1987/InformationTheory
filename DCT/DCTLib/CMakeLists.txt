project(DCTLib)

set(SRCS
    Matrix.cpp)

set(HDRS
    Matrix.hpp)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HDRS})
