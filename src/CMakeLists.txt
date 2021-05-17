ecm_setup_version(${KOLLISION_VERSION}
    VARIABLE_PREFIX KOLLISION
    VERSION_HEADER kollision_version.h
)

add_executable(kollision)

target_sources(kollision PRIVATE
    animation.cpp
    animator.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    mainwindow.cpp
    message.cpp

    kollision.qrc
)

kconfig_add_kcfg_files(kollision kollisionconfig.kcfgc)

ki18n_wrap_ui(kollision preferences.ui)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/pictures/hi/*-apps-kollision.png")
ecm_add_app_icon(kollision ICONS ${ICONS_SRCS})

target_link_libraries(kollision
    KF5KDEGames
    KF5::Crash
    KF5::DBusAddons
    KF5::I18n
    KF5::XmlGui
)

install(TARGETS kollision ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.kollision.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kollision.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
