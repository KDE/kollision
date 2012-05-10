project(kollision)

if(NOT COMPILING_KDEGAMES)
	find_package(KDE4 REQUIRED)
	include(KDE4Defaults)
	include(MacroLibrary)
	find_package(KDEGames REQUIRED)

	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
	include_directories(${QDBUS_INCLUDE_DIRS} ${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIRS})
endif(NOT COMPILING_KDEGAMES)

add_subdirectory(pictures)
add_subdirectory(sounds)

set(kollision_SRCS
    animation.cpp
    animator.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    mainwindow.cpp
    message.cpp      
)
kde4_add_kcfg_files(kollision_SRCS kollisionconfig.kcfgc)
kde4_add_ui_files(kollision_SRCS preferences.ui)
kde4_add_app_icon(kollision_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/pictures/hi*-app-kollision.png")
kde4_add_executable(kollision ${kollision_SRCS})

target_link_libraries(kollision ${KDE4_KDEUI_LIBS} kdegames ${KDE4_PHONON_LIBS})

install(TARGETS kollision ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS kollision.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kollisionui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kollision)
