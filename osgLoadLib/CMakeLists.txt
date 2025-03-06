add_definitions(-DOSGBLoadLib_EXPORTS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src osgLoadLib_SRC)
add_library(osgLoadLib SHARED ${osgLoadLib_SRC})
target_include_directories(osgLoadLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(osg_Release_Dir ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
include_directories(${osg_Release_Dir}/include)
set(osg_Debug_Dir ${osg_Release_Dir}/debug)

if(WIN32)
#debug不能和Release的dll混用，混用会导致std::string无法正常传参
target_link_libraries(osgLoadLib PRIVATE
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/osgd.lib>
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/osgDBd.lib>
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/OpenThreadsd.lib>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/osg.lib>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/osgDB.lib>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/OpenThreads.lib>
)
ELSE()
target_link_libraries(osgLoadLib PRIVATE
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/libosgd.so>
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/libosgDBd.so>
        $<$<CONFIG:Debug>:${osg_Debug_Dir}/lib/libOpenThreadsd.so>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libosg.so.3.6.5>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libosgDB.so.3.6.5>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libOpenThreads.so.3.3.1>
)
ENDIF()

install(TARGETS osgLoadLib)
install(FILES include/macro.h include/IOsgbLoad.h DESTINATION include)