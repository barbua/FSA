CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(PyFSA C CXX)

#Find Python
find_package(PythonLibs REQUIRED)
include_directories(
  ${PYTHON_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIR}/../Lib/site-packages/numpy/core/include/numpy
  .  
  ../src
)
link_directories(
  ${PYTHON_INCLUDE_DIR}/../libs
)


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

ADD_LIBRARY(PyFSA ${LIB_TYPE}
	 ../src/DataMemory.h
	 ../src/DataSource.h
	 ../src/Fsa.h
	 ../src/FsaSchedule.h
	 ../src/LossFunctions.h
	 ../src/MomPars.h
	 ../src/VectorUtil.h
	 PyFSA.h
	 PyFSA.cpp
	 )

SET_TARGET_PROPERTIES(PyFSA PROPERTIES SUFFIX .pyd)

#SET(CMAKE_LIBRARY_PATH "C:/Python/libs")
#SET_TARGET_PROPERTIES(PyFSA PROPERTIES LINK_FLAGS "/LIBPATH:C:/Python/libs" )
target_link_libraries(PyFSA ${PYTHON_LIBRARIES})

