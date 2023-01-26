ecm_setup_version(${KOLLISION_VERSION}
    VARIABLE_PREFIX KOLLISION
    VERSION_HEADER kollision_version.h
)

add_executable(kollision)

target_sources(kollision PRIVATE
    animation.cpp
    animation.h
    animator.cpp
    animator.h
    ball.cpp
    ball.h
    mainarea.cpp
    mainarea.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    message.cpp
    message.h
    spritefwd.h
    sprite.h

    kollision.qrc
)

kconfig_add_kcfg_files(kollision kollisionconfig.kcfgc)

ki18n_wrap_ui(kollision preferences.ui)

file(GLOB ICONS_SRCS "${CMAKE_SOURCE_DIR}/pictures/hi/*-apps-kollision.png")
ecm_add_app_icon(kollision ICONS ${ICONS_SRCS})

target_link_libraries(kollision
    KF5KDEGames
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::DBusAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::XmlGui
)

install(TARGETS kollision ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.kollision.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kollision.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
