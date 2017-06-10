CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(PyFSA)

enable_language(CXX)
enable_language(C)

file(GLOB_RECURSE OCTAVE_FULL_FILE_PATH "/usr/include/octave*/octave/mex.h")

string(FIND ${OCTAVE_FULL_FILE_PATH} "mex.h" INDEX)

string(SUBSTRING ${OCTAVE_FULL_FILE_PATH} 0 ${INDEX} OCTAVE_DIRECTORY)

message(STATUS ${OCTAVE_DIRECTORY})

#Find Python
find_package(PythonLibs REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIR}/numpy/
  ${PYTHON_INCLUDE_DIR}/../lib/site-packages/numpy/core/include/numpy 
  ${PYTHON_INCLUDE_DIR}/../Lib/site-packages/numpy/core/include/numpy
  .  
  ../include
  ./include
  ${OCTAVE_DIRECTORY}
  )

link_directories(
  ${PYTHON_INCLUDE_DIR}/../../lib
)

message (STATUS ${PYTHON_INCLUDE_DIR})
message (STATUS ${PYTHON_INCLUDE_DIR})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -s -DNDEBUG")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2 -s -DNDEBUG")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

IF(MSVC)
ADD_DEFINITIONS(-DUSE_MATH_DEFINES -DUSE_SCS -DAB_USE_UNORDERED_MAP	-D_CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)

SET (LIB_TYPE SHARED)

FILE(GLOB HEADER_FILES "include/*.h")
FILE(GLOB FSA_HEADER "include/Fsa.h")
FILE(GLOB SOURCE_FILES "src/*.cpp")

message(STATUS ${FSA_HEADER})

string(FIND ${FSA_HEADER} "Fsa.h" FSAINDEX)
string(SUBSTRING ${FSA_HEADER} 0 ${FSAINDEX} FSADIRECTORY)

message(STATUS ${FSADIRECTORY})

message(STATUS ${HEADER_FILES})
message(STATUS ${SOURCE_FILES})

ADD_LIBRARY(PyFSA ${LIB_TYPE}
	${HEADER_FILES}
	${SOURCE_FILES}
)

IF(WIN32)
SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX .pyd)
ELSEIF(LINUX)
SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX .so)
ENDIF(WIN32)

#SET(CMAKE_LIBRARY_PATH "C:/Python/libs")
#SET_TARGET_PROPERTIES(PyFSA PROPERTIES LINK_FLAGS "/LIBPATH:C:/Python/libs" )
target_link_libraries(PyFSA ${PYTHON_LIBRARIES})

#find_program(
#MEX_COMPILER
#NAMES mex
#PATHS /usr/bin/
#)

execute_process(COMMAND mex -I${FSADIRECTORY} -std=c++11 -lPyFSA -L./ --mex -v ../src/mex_files/trainLinFSA.cpp)
