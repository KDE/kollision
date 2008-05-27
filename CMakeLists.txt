project(kollision)

add_subdirectory(pictures)
add_subdirectory(sounds)

set(kollision_SRCS
    animation.cpp
    animator.cpp
    audioplayer.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    mainwindow.cpp
    message.cpp      
    renderer.cpp
)
kde4_add_kcfg_files(kollision_SRCS kollisionconfig.kcfgc)
kde4_add_ui_files(kollision_SRCS preferences.ui)
kde4_add_app_icon(kollision_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/pictures/hi*-app-kollision.png")
kde4_add_executable(kollision ${kollision_SRCS})

target_link_libraries(kollision ${KDE4_KDEUI_LIBS} kdegames ${KDE4_PHONON_LIBS})

install(TARGETS kollision ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kollision.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kollisionui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kollision)
