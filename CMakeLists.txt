project(kollision)

add_subdirectory(pictures)

set(kollision_SRCS
    animation.cpp
    animator.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    mainwindow.cpp
    message.cpp      
    renderer.cpp
)
kde4_add_kcfg_files(kollision_SRCS kollisionconfig.kcfgc)
kde4_add_ui_files(kollision_SRCS preferences.ui)
kde4_add_executable(kollision ${kollision_SRCS})

target_link_libraries(kollision ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kollision DESTINATION ${BIN_INSTALL_DIR})
install(FILES kollision.desktop DESTINATION ${XDG_APPS_DIR})
install(FILES kollisionui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kollision)
