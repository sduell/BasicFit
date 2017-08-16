cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
project (BasicFit)
set(CMAKE_BUILD_TYPE DEBUG)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(include utils)

file(GLOB FITTERSOURCES "src/*.cc")
file(GLOB FITTERUTILS "utils/*.cc")

#Add ROOT libraries to link against
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package (ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net Minuit Physics)

include (${ROOT_USE_FILE})

add_library(BasicFit SHARED ${FITTERSOURCES} ${FITTERUTILS})
target_link_libraries(BasicFit ${ROOT_LIBRARIES})
