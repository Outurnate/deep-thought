FILE(GLOB project_tests *.cpp)

FOREACH(testFile ${project_tests})
    GET_FILENAME_COMPONENT(testExecutable ${testFile} NAME_WE)
    ADD_EXECUTABLE(${testExecutable} ${testFile})
    TARGET_LINK_LIBRARIES(${testExecutable} ${project_LIBS})
    ADD_TEST(${testExecutable} ${EXECUTABLE_OUTPUT_PATH}/${testExecutable})
ENDFOREACH(testFile)
