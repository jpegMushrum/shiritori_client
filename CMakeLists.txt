cmake_minimum_required(VERSION 3.16)

project(shiritori-client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Network Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Network Core)

set(TS_FILES shiritori-client_en_150.ts)

set(PROJECT_SOURCES
        main.cpp
        
        # Application
        src/application.h
        src/application.cpp
        
        # Base Screen
        src/screens/basescreen.h
        src/screens/basescreen.cpp
        
        # Screens
        src/screens/loginscreen.h
        src/screens/loginscreen.cpp
        src/screens/mainscreen.h
        src/screens/mainscreen.cpp
        src/screens/searchgamescreen.h
        src/screens/searchgamescreen.cpp
        src/screens/gamescreen.h
        src/screens/gamescreen.cpp
        src/screens/wordinfodialog.h
        src/screens/wordinfodialog.cpp
        src/screens/statsscreen.h
        src/screens/statsscreen.cpp
        
        # Models
        src/models/user.h
        src/models/user.cpp
        src/models/game.h
        src/models/game.cpp

        # Services
        src/services/apiservice.h
        src/services/apiservice.cpp
        src/services/serverprotocol.h
        src/services/serverprotocol.cpp
        src/services/gamelogic.h
        src/services/gamelogic.cpp
        src/services/tcpclient.h
        src/services/tcpclient.cpp
        src/services/connectionstatuswidget.h
        src/services/connectionstatuswidget.cpp
        
        # Utils
        src/utils/constants.h
        src/utils/appstate.h
        src/utils/appstate.cpp
        src/utils/screennavigator.h
        src/utils/screennavigator.cpp
        
        # Resources
        resources/resources.qrc
        
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(shiritori-client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET shiritori-client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(shiritori-client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(shiritori-client
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(shiritori-client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.shiritori-client)
endif()
set_target_properties(shiritori-client PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS shiritori-client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(shiritori-client)
endif()
