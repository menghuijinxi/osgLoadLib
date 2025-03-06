set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)

#v143代表vs2022
set(VCPKG_PLATFORM_TOOLSET v143)
#设置msvc的小版本，mscv版本一定要和toolset中的版本号完全一致，这样可以避免vcpkg编译的有些库无法链接，或者调用时产生莫名其妙的崩溃。
#c++库经常发生这种情况，如果需要升级mscv版本请保证toolset和这个参数的版本号完全一致
set(VCPKG_PLATFORM_TOOLSET_VERSION 14.36.32532)