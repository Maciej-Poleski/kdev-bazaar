cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(bazaar)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)
include (KDE4Defaults)

include_directories(
    ${KDE4_INCLUDES}
    ${KDEVPLATFORM_INCLUDE_DIR}
)

add_definitions(-DKDE_DEFAULT_DEBUG_AREA=9525)

add_subdirectory(src)
