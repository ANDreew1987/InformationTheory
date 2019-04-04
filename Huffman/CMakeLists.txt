project(Huffman)

set(SRCS main.cpp)

set(HDRS Node.hpp Tree.hpp)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})
