CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(PyFSA)

enable_language(CXX)
enable_language(C)

#####################################FIND OCTAVE OR MATLAB TO PRODUCE MEX SAMPLE CODE################################################
if(UNIX)
  file(GLOB_RECURSE OCTAVE_OR_MATLAB_FULL_FILE_PATH "/usr/include/octave*/octave/mex.h")
elseif(WIN32 OR WIN64)
  file(GLOB_RECURSE OCTAVE_OR_MATLAB_FULL_FILE_PATH 
  "C:/Program Files*/MATLAB/*/extern/include/mex.h")
  if(NOT OCTAVE_OR_MATLAB_FULL_FILE_PATH)
  file(GLOB_RECURSE OCTAVE_OR_MATLAB_FULL_FILE_PATH
  "C:/Octave*/Octave*/include/octave*/octave/mex.h")
  endif(NOT OCTAVE_OR_MATLAB_FULL_FILE_PATH)
  ##################################IF WE DON'T FIND OCTAVE OR MATLAB FATAL ERROR#####################################################
  if(NOT OCTAVE_OR_MATLAB_FULL_FILE_PATH)
    message(FATAL_ERROR "ERROR!!! ERROR!!! OCTAVE OR MATLAB NOT FOUND PLEASE DOWNLOAD AND PLACE THESE IN DEFAULT LOCATIONS THEN RERUN THIS CMAKELIST")
  endif(NOT OCTAVE_OR_MATLAB_FULL_FILE_PATH)
endif(UNIX)

######################################PRINTS PATH OF OCTAVE############################################################
message(STATUS ${OCTAVE_OR_MATLAB_FULL_FILE_PATH})

######################################THIS REMOVES MEX.H FROM PATH SO THAT WE CAN INCLUDE THE DIRECTORY##########################
string(FIND ${OCTAVE_OR_MATLAB_FULL_FILE_PATH} "mex.h" INDEX)

string(SUBSTRING ${OCTAVE_OR_MATLAB_FULL_FILE_PATH} 0 ${INDEX} OCTAVE_DIRECTORY)

message(STATUS ${OCTAVE_DIRECTORY})
##############################################################################################################################

######################################THE SEARCH FOR PYTHON BEGINS####################################
find_package(PythonLibs REQUIRED)
message(STATUS ${PYTHON_INCLUDE_DIR})
include_directories(
  ${PYTHON_INCLUDE_DIR}/../../lib/python3.6/site-packages/numpy-1.13.0-py3.6-linux-x86_64.egg/numpy/core/include/numpy/
  ${PYTHON_INCLUDE_DIR}/
  ${PYTHON_INCLUDE_DIR}/numpy/
  ${PYTHON_INCLUDE_DIR}/../lib/site-packages/numpy/core/include/numpy 
  ${PYTHON_INCLUDE_DIR}/../Lib/site-packages/numpy/core/include/numpy
  ${PYTHON_INCLUDE_DIR}/include/
  ../include
  ./include
  ${OCTAVE_DIRECTORY}
  )

if(WIN32 OR WIN64)
MESSAGE(WARNING "YOUR BINARIES OR THE GENERATED PYD/SO FILE WILL BE IN A FOLDER CALLED DEBUG IN THE DIRECTORY YOU RAN THE CMAKE COMMAND IN")
file(GLOB_RECURSE PYTHON_LIBRARIES "C:/Users/*/AppData/Local/Programs/Python/Python*/libs/*.lib")
endif(WIN32 OR WIN64)

string(COMPARE EQUAL "${PYTHON_LIBRARIES}" "" result)
if(WIN32 OR WIN64)
if(result)
  message(STATUS "TRYING TO FIND PYTHON SOMEWHERE ELSE")
  file(GLOB_RECURSE PYTHON_LIBRARIES "C:/Python*/libs/*.lib")
  string(COMPARE EQUAL "${PYTHON_LIBRARIES}" "" result2)
  if(result2)
  message(FATAL ERROR "PYTHON LIBRARY NOT FOUND IN DEFAULT LOCATIONS, PLEASE INSTALL IT AND ENSURE IT IS IN C:/Pythonxx OR C:/Users/<your username without anglebrackets>/AppData/Programs/...")
  endif(result2)
else()
  message(STATUS "PYTHON LIBRARY LINKING FROM: ")
  message(STATUS ${PYTHON_LIBRARIES})
endif(result)
endif(WIN32 OR WIN64)

message(STATUS ${PYTHON_LIBRARIES})

######################################CHECKS FOR C++11 SUPPORT, IF SUPPORT IS NOT PRESENT THIS CODE WILL STOP FURTHER PROCESSING############################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -O2 -s -DNDEBUG")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -O2 -fPIC -s -DNDEBUG")
else()
        message(FATAL ERROR "THE COMPILER ${CMAKE_CXX_COMPILER} HAS NO C++11 SUPPORT!!!!! PLEASE USE A NEWER COMPILER TO USE THIS PROJECT!! THANK YOU")
endif()
#############################################################################################################################################################
IF(MSVC)
ADD_DEFINITIONS(-DUSE_MATH_DEFINES -DUSE_SCS -DAB_USE_UNORDERED_MAP	-D_CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)

FILE(GLOB HEADER_FILES "include/*.h")
FILE(GLOB SOURCE_FILES "src/*.cpp")

##########################################CREATE SHARED LIBRARY CONSTRUCTED FROM OUR SOURCE CODE AND HEADER FILES#########################
SET (LIB_TYPE SHARED)

ADD_LIBRARY(PyFSA ${LIB_TYPE}
	${HEADER_FILES}
	${SOURCE_FILES}
)

##########################################SET SUFFIX OF SHARED LIBRARY BASED ON PLATFORM GENERATING IT####################################
IF(WIN32)
SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX 32.pyd)
ELSEIF(LINUX)
SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX .so)
ELSEIF(WIN64)
SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX 64.pyd)
ENDIF(WIN32)
##########################################LINK PyFSA WITH PYTHON LIBRARY LIBRARY FILE#########################################
target_link_libraries(PyFSA ${PYTHON_LIBRARIES})
