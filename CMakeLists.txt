CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(PyFSA)

enable_language(CXX)
enable_language(C)


##############################################################################################################################

######################################THE SEARCH FOR PYTHON BEGINS####################################
find_package(PythonLibs REQUIRED)
message(STATUS ${PYTHON_INCLUDE_DIR})
file(GLOB_RECURSE PYTHON_NUMPY_INCLUDE "C:/Python*/Lib/site-packages/*/numpy/core/include/numpy/arrayobject.h")
string(FIND ${PYTHON_NUMPY_INCLUDE} '/arrayobject.h' PYTHONINDEX)
string(SUBSTRING ${PYTHON_NUMPY_INCLUDE} 0 ${PYTHONINDEX} PYTHON_NUMPY_INCLUDE_DIR)
message(STATUS ${PYTHON_NUMPY_INCLUDE_DIR})
include_directories(
  ${PYTHON_NUMPY_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIR}/../../lib/python3.6/site-packages/numpy-1.13.0-py3.6-linux-x86_64.egg/numpy/core/include/numpy/
  ${PYTHON_INCLUDE_DIR}/
  ${PYTHON_INCLUDE_DIR}/numpy/
  ${PYTHON_INCLUDE_DIR}/../lib/site-packages/numpy/core/include/numpy 
  ${PYTHON_INCLUDE_DIR}/../Lib/site-packages/numpy/core/include/numpy
  ${PYTHON_INCLUDE_DIR}/include/
  ../include
  ./include
  )
  
  if(WIN32 OR WIN64)
MESSAGE(WARNING "YOUR BINARIES OR THE GENERATED PYD/SO FILE WILL BE IN A FOLDER CALLED DEBUG IN THE DIRECTORY YOU RAN THE CMAKE COMMAND IN")
file(GLOB_RECURSE PYTHON_LIBRARIES "C:/Users/*/AppData/Local/Programs/Python/Python*/libs/*.lib")
file(GLOB_RECURSE PYTHON_NUMPY_INCLUDE "C:/Users/*AppData/Local/Porgrams/Python/Python*/Lib/site-packages/*/numpy/core/include/numpy/arrayobject.h")
endif(WIN32 OR WIN64)

string(COMPARE EQUAL "${PYTHON_LIBRARIES}" "" result)
if(WIN32 OR WIN64)
if(result)
  message(STATUS "TRYING TO FIND PYTHON SOMEWHERE ELSE")
  file(GLOB_RECURSE PYTHON_LIBRARIES "C:/Python*/libs/*.lib")
  string(COMPARE EQUAL "${PYTHON_LIBRARIES}" "" result2)
  if(result2)
  message(FATAL ERROR "PYTHON LIBRARY AND NUMPY NOT FOUND IN DEFAULT LOCATIONS, PLEASE INSTALL IT AND ENSURE IT IS IN C:/Pythonxx OR C:/Users/<your username without anglebrackets>/AppData/Programs/...")
  endif(result2)
else()
  message(STATUS "PYTHON LIBRARY LINKING FROM: ")
  message(STATUS ${PYTHON_LIBRARIES})
endif(result)
endif(WIN32 OR WIN64)

message(STATUS ${PYTHON_LIBRARIES})

message(STATUS ${PYTHON_NUMPY_INCLUDE})

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
