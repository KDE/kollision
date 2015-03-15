
install(FILES
theme.svgz
# theme.desktop does not need to be installed anymore
DESTINATION ${KDE_INSTALL_DATADIR}/kollision/pictures)

add_subdirectory(ox)

add_subdirectory(hi)
