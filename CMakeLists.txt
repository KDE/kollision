project(kollision)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.3.0")
set (KF5_MIN_VERSION "5.15.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    DBusAddons
    DocTools
    I18n
    KIO
    NewStuff
    NotifyConfig
    TextWidgets
    WidgetsAddons
    XmlGui
)

find_package(KF5KDEGames 4.9.0 REQUIRED)
find_package(Phonon4Qt5 CONFIG REQUIRED)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory(pictures)
add_subdirectory(sounds)
add_subdirectory(doc)

set(kollision_SRCS
    animation.cpp
    animator.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    mainwindow.cpp
    message.cpp
)
kconfig_add_kcfg_files(kollision_SRCS kollisionconfig.kcfgc)
ki18n_wrap_ui(kollision_SRCS preferences.ui)
file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/pictures/hi/*-apps-kollision.png")
ecm_add_app_icon(kollision_SRCS ICONS ${ICONS_SRCS})
add_executable(kollision ${kollision_SRCS})

target_link_libraries(kollision
    KF5::Crash
    KF5::DBusAddons
    KF5::I18n
    KF5::XmlGui
    Phonon::phonon4qt5
    KF5KDEGames
)

install(TARGETS kollision ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kollision.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kollision.appdata.xml DESTINATION ${KDE_INSTALL_DATAROOTDIR}/appdata/)
install(FILES kollisionui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kollision)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
