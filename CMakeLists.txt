cmake_minimum_required(VERSION 3.16)

#初始化vcpkg
IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg)
    message("vcpkg已经存在，不需要再次下载")
ELSE()
    message("下载vcpkg中...")
    execute_process(COMMAND git clone https://github.com/microsoft/vcpkg.git ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg)
    IF(WIN32)
    	execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg/bootstrap-vcpkg.bat)
    ELSE()
	    execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg/bootstrap-vcpkg.sh)
    ENDIF()
ENDIF()
IF(WIN32)
    IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg)
        file(COPY x64-windows-custom.cmake DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg/triplets)
        set(VCPKG_TARGET_TRIPLET x64-windows-custom)
    ENDIF()
    #设置msvc具体的小版本，指定具体mscv版本是为了避免制作mod时使用了不同版本mscv导致mod会发生各种异常崩溃。当然也能避免项目年久失修无法编译
    set(CMAKE_GENERATOR_TOOLSET host=x64,version=14.36.32532)
    # 必须在add_library ， add_executable 前设置，否则无效。让脚本的字符串使用utf8格式编译
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
    #关闭RTTI
    #add_compile_options(/GR-)
ENDIF()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/out/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_STANDARD 20)

project(osgbLoadLib)

add_subdirectory(test)
add_subdirectory(osgLoadLib)