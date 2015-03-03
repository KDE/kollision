
install(FILES
theme.svgz
# theme.desktop does not need to be installed anymore
DESTINATION ${KDE_INSTALL_DATADIR}/kollision/pictures)

ecm_install_icons(ICONS hi128-apps-kollision.png  hi22-apps-kollision.png  hi48-apps-kollision.png  ox128-apps-kollision.png  ox22-apps-kollision.png  ox48-apps-kollision.png
hi16-apps-kollision.png   hi32-apps-kollision.png  hi64-apps-kollision.png  ox16-apps-kollision.png   ox32-apps-kollision.png  ox64-apps-kollision.png
DESTINATION ${KDE_INSTALL_ICONDIR})
