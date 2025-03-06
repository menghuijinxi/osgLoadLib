vcpkg_download_distfile(ARCHIVE
    URLS "http://10.1.1.58:8418/SunXuJing/osgLoadLib/archive/0.0.1.tar.gz"
    FILENAME "osgLoadLib-0.0.1.zip"
	SHA512 f8cac521b5ba2f0f7d6998870ef946d6552bba5a67942616911a5a19888134516f25904c65686c44dec64d0ee8e5dbf29cbe636d10e1f3e1ec878390b65b2bb0
)

vcpkg_extract_source_archive_ex(
    OUT_SOURCE_PATH SOURCE_PATH
    ARCHIVE ${ARCHIVE}
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_cmake_install()