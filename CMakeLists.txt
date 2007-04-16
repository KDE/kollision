find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${LIBRARY_OUTPUT_PATH} ${KDE4_LIB_DIR})
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory(pictures)

set(kollision_SRCS
    animation.cpp
    animator.cpp
    ball.cpp
    main.cpp
    mainarea.cpp
    message.cpp      
    renderer.cpp     
)

kde4_automoc(${kollision_SRCS})
kde4_add_executable(kollision ${kollision_SRCS})

target_link_libraries(kollision ${QT_GUI} ${KDE4_KDEUI_LIBS} kdegames)

install(TARGETS kollision DESTINATION ${BIN_INSTALL_DIR})
