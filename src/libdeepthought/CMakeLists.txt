FILE(GLOB deepthought_srcs *.cpp)
SET(deepthought_libs ${Boost_LIBRARIES} ${LOG4CXX_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SOCI_LIBRARY} ${SOCI_sqlite3_PLUGIN} ${SQLITE3_LIBRARIES})

ADD_LIBRARY(deepthought SHARED ${deepthought_srcs})
TARGET_LINK_LIBRARIES(deepthought ${deepthought_libs})
