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
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libosg.so>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libosgDB.so>
        $<$<CONFIG:Release>:${osg_Release_Dir}/lib/libOpenThreads.so>
)
ENDIF()

IF(WIN32)
    set(osgDll_Debug_Dir ${osg_Debug_Dir}/bin)
    set(osgDll_Release_Dir ${osg_Release_Dir}/bin)
    
    #拷贝jpeg62.dll，读取osgb中的贴图时用得着
add_custom_command(
        TARGET osgLoadLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/jpeg62.dll>$<$<CONFIG:Release>:${osgDll_Release_Dir}/jpeg62.dll>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/zlibd1.dll>$<$<CONFIG:Release>:${osgDll_Release_Dir}/zlib1.dll>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/osgUtild.dll>$<$<CONFIG:Release>:${osgDll_Release_Dir}/osgUtil.dll>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/osgDBd.dll>$<$<CONFIG:Release>:${osgDll_Release_Dir}/osgDB.dll>"
        $<TARGET_FILE_DIR:osgLoadLib>
)

#拷贝osg，jpeg,serializers_osg这三个插件，读取osgb文件时用得着
add_custom_command(
        TARGET osgLoadLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:osgLoadLib>/osgPlugins-3.6.5
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_osgd.dll>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_osg.dll>"
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_jpegd.dll>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_jpeg.dll>"
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_serializers_osgd.dll>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_serializers_osg.dll>"
        $<TARGET_FILE_DIR:osgLoadLib>/osgPlugins-3.6.5
)
ELSE()
    set(osgDll_Debug_Dir ${osg_Debug_Dir}/lib)
    set(osgDll_Release_Dir ${osg_Release_Dir}/lib)
    
    #拷贝jpeg62.dll，读取osgb中的贴图时用得着
add_custom_command(
        TARGET osgLoadLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libjpeg.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libjpeg.so>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libz.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libz.so>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libosgUtild.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libosgUtil.so>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libosgDBd.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libosgDB.so>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libosgd.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libosg.so>"
        "$<$<CONFIG:Debug>:${osgDll_Debug_Dir}/libOpenThreadsd.so>$<$<CONFIG:Release>:${osgDll_Release_Dir}/libOpenThreads.so>"
        $<TARGET_FILE_DIR:osgLoadLib>
)

#拷贝osg，jpeg,serializers_osg这三个插件，读取osgb文件时用得着
add_custom_command(
        TARGET osgLoadLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:osgLoadLib>/osgPlugins-3.6.5
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_osgd.so>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_osg.so>"
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_jpegd.so>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_jpeg.so>"
        "$<$<CONFIG:Debug>:${osg_Debug_Dir}/plugins/osgPlugins-3.6.5/osgdb_serializers_osgd.so>$<$<CONFIG:Release>:${osg_Release_Dir}/plugins/osgPlugins-3.6.5/osgdb_serializers_osg.so>"
        $<TARGET_FILE_DIR:osgLoadLib>/osgPlugins-3.6.5
)
ENDIF()