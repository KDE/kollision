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
    KDEGames6
    KF6::CoreAddons
    KF6::Crash
    KF6::DBusAddons
    KF6::I18n
    KF6::XmlGui
    KF6::IconThemes
)

if(APPLE)
    set_target_properties(kollision PROPERTIES
           MACOSX_BUNDLE_DISPLAY_NAME "Kollision"
        MACOSX_BUNDLE_BUNDLE_NAME "Kollision"
        MACOSX_BUNDLE_LONG_VERSION_STRING "Kollision ${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${RELEASE_SERVICE_VERSION}"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.kollision"
        MACOSX_BUNDLE_COPYRIGHT "2024 The Kollision Developers")
endif()


install(TARGETS kollision ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(PROGRAMS org.kde.kollision.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kollision.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
