project ("DCTCompare")

set(CMAKE_INCLUDE_CURRENTUI_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

file(GLOB SRCS 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB HDRS 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

file(GLOB FORMS 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

include_directories(
	include_directories(${DCT_DIR}/DCTLib)
)

add_executable(${PROJECT_NAME} WIN32
	${SRCS}
	${HDRS}
	${FORMS}
)

set(LIBRARIES
	DCTLib
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
)

target_link_libraries (${PROJECT_NAME}
	PRIVATE
	${LIBRARIES}
)
