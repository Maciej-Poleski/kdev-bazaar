
set(kdevbazaar_PART_SRCS
    bazaarplugin.cpp
)

kde4_add_plugin(kdevbazaar ${kdevbazaar_PART_SRCS})

target_link_libraries(kdevbazaar
    ${KDE4_KDEUI_LIBS}
    ${KDEVPLATFORM_INTERFACES_LIBRARIES}
)

install(TARGETS kdevbazaar DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kdevbazaar.desktop DESTINATION ${SERVICES_INSTALL_DIR})
