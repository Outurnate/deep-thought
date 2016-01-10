FILE(GLOB project_tests *.cpp)

set(CMAKE_BUILD_TYPE Debug)

FOREACH(testFile ${project_tests})
    INCLUDE_DIRECTORIES(${MAINFOLDER}/src)
    GET_FILENAME_COMPONENT(testExecutable ${testFile} NAME_WE)
    ADD_EXECUTABLE(${testExecutable} ${testFile})
    TARGET_LINK_LIBRARIES(${testExecutable} deepthought)
    TARGET_LINK_LIBRARIES(${testExecutable} tetrinet)
    ADD_TEST(${testExecutable} ${EXECUTABLE_OUTPUT_PATH}/${testExecutable})
ENDFOREACH(testFile)
