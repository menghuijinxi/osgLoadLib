add_executable(test main.cpp)
target_link_libraries(test PRIVATE osgLoadLib)
add_custom_command(
        TARGET test
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:osgLoadLib> $<TARGET_FILE_DIR:test>
)

IF(WIN32)
        target_compile_options(test PRIVATE /GR-)
ELSE()
        target_compile_options(test PRIVATE -fno-rtti)
ENDIF()
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DELAYLOAD:osgd.dll /DELAYLOAD:osgDBd.dll")
