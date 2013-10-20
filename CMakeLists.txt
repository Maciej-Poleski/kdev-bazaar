project(bazaar)

find_package(KDE4 REQUIRED)
find_package(KDevPlatform REQUIRED)
include (KDE4Defaults)

include_directories(
    ${KDE4_INCLUDES}
    ${KDEVPLATFORM_INCLUDE_DIR}
)

add_subdirectory(src)
