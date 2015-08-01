#FILE(GLOB project_programs *.cpp)
#
#FOREACH(programFile ${project_programs})
#    INCLUDE_DIRECTORIES(${MAINFOLDER}/src)
#    GET_FILENAME_COMPONENT(testExecutable ${programFile} NAME_WE)
#    ADD_EXECUTABLE(${programExecutable} ${programFile})
#    TARGET_LINK_LIBRARIES(${programExecutable} libdeepthought)
#ENDFOREACH(programFile)

SET(project_BIN ${PROJECT_NAME})

INCLUDE_DIRECTORIES(${MAINFOLDER}/src)
ADD_EXECUTABLE(${project_BIN} DeepThought.cpp)
TARGET_LINK_LIBRARIES(${project_BIN} deepthought)
SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
