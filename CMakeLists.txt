cmake_minimum_required(VERSION 2.8.11)

project(FTutor1D)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

set(CMAKE_BUILD_TYPE DEBUG)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set the compilation way
set(CMAKE_VERBOSE_MAKEFILE ON CACHE STRING "Tracing the compilation process" FORCE)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)


include_directories(${Qt5Widgets_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -ggdb3")

file(GLOB SRC_LIST
	"src/*.h"
	"src/*.cpp"
	"src/qcustomplot/*"
)


add_executable(${PROJECT_NAME} ${SRC_LIST})



# Use the Widgets module from Qt 5.
target_link_libraries(FTutor1D ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} Qt5::Xml Qt5::XmlPatterns ${OPENGL_LIBRARIES})
