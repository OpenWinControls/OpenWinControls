cmake_minimum_required(VERSION 3.23)

project(OpenWinControls VERSION 2.0 DESCRIPTION "Multiplatform GPD WinControls replacement" LANGUAGES CXX)

set(PROJECT_AUTHOR "kylon")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(src/extern/libOpenWinControls)
add_subdirectory(src/extern/yaml-cpp)

qt_add_resources(QT_RESOURCE
  src/resources/res.qrc
)

set(PROJECT_SRC
  src/pages/Widgets/CharMapWidget.h
  src/pages/Widgets/CharMapWidget.cpp
  src/pages/HomePage.h
  src/pages/HomePage.cpp
  src/pages/FaceButtonsPage.h
  src/pages/FaceButtonsPage.cpp
  src/pages/KeyboardMouseButtonsPage.h
  src/pages/KeyboardMouseButtonsPage.cpp
  src/pages/XinputButtonsPage.h
  src/pages/XinputButtonsPage.cpp
  src/pages/BackButtonsPage.h
  src/pages/BackButtonsPage.cpp
  src/pages/BackButtonsV1Page.h
  src/pages/BackButtonsV1Page.cpp
  src/pages/BackButtonsV2Page.h
  src/pages/BackButtonsV2Page.cpp
  src/pages/LogsPage.h
  src/pages/LogsPage.cpp
  src/pages/SettingsPage.h
  src/pages/SettingsPage.cpp

  src/include/ASCIIHIDMap.h
  src/include/ControllerFeature.h
  src/include/GPDProducts.h
  src/include/CharMapMode.h

  src/main.cpp
  src/mainwindow.cpp
  src/mainwindow.h
  src/mainwindow.ui
)

if (WIN32)
  configure_file(src/resources/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

  list(APPEND PROJECT_SRC
    win.rc
  )
endif ()

configure_file(src/resources/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h)

qt_add_executable(${PROJECT_NAME} WIN32 ${QT_RESOURCE} ${PROJECT_SRC})

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
  message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets lowc::owc yaml-cpp::yaml-cpp)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  BUNDLE  DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
