FILE(GLOB tetrinet_srcs *.cpp)
SET(tetrinet_libs ${Boost_LIBRARIES} ${LOG4CXX_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

ADD_LIBRARY(tetrinet SHARED ${tetrinet_srcs})
TARGET_LINK_LIBRARIES(tetrinet ${tetrinet_libs})
