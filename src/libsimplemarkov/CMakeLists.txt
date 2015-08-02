FILE(GLOB simplemarkov_srcs *.cpp)
SET(simplemarkov_libs ${Boost_LIBRARIES} ${LOG4CXX_LIBRARIES})

ADD_LIBRARY(simplemarkov SHARED ${simplemarkov_srcs})
TARGET_LINK_LIBRARIES(simplemarkov ${simplemarkov_libs})
