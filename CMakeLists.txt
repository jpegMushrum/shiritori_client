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
        src/screens/gameendscreen.h
        src/screens/gameendscreen.cpp
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


        src/services/tcpclient.h
        src/services/tcpclient.cpp
        src/services/connectionstatuswidget.h
        src/services/connectionstatuswidget.cpp
        src/services/servicethread.h
        src/services/servicethread.cpp
        src/services/notificationmanager.h 
        src/services/notificationmanager.cpp
        
        # Utils

        src/utils/appstate.h
        src/utils/appstate.cpp
        src/utils/screennavigator.h
        src/utils/screennavigator.cpp
        src/utils/toast.h
        src/utils/toast.cpp
        src/utils/toastwidget.h
        src/utils/toastwidget.cpp
        
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

# ==================== TESTS ====================
enable_testing()

# Find Qt Test module
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

# Test sources - Models
set(TEST_MODELS_SOURCES
    tests/test_models.h
    tests/test_models.cpp
    src/models/user.h
    src/models/user.cpp
    src/models/game.h
    src/models/game.cpp
)

# Test sources - Utils
set(TEST_UTILS_SOURCES
    tests/test_utils.h
    tests/test_utils.cpp
    src/utils/appstate.h
    src/utils/appstate.cpp
    src/utils/screennavigator.h
    src/utils/screennavigator.cpp
)

# Test sources - Services
set(TEST_SERVICES_SOURCES
    tests/test_services.h
    tests/test_services.cpp
    src/utils/appstate.h
    src/utils/appstate.cpp
    src/services/serverprotocol.h
    src/services/serverprotocol.cpp
    src/services/apiservice.h
    src/services/apiservice.cpp
    src/services/tcpclient.h
    src/services/tcpclient.cpp
)

# Test sources - Navigation
set(TEST_NAVIGATION_SOURCES
    tests/test_navigation.h
    tests/test_navigation.cpp
    src/utils/screennavigator.h
    src/utils/screennavigator.cpp
    src/utils/toastwidget.h
    src/utils/toastwidget.cpp
    src/utils/toast.h
    src/utils/toast.cpp
)

# Test sources - Network
set(TEST_NETWORK_SOURCES
    tests/test_network.h
    tests/test_network.cpp
    src/services/tcpclient.h
    src/services/tcpclient.cpp
    src/services/apiservice.h
    src/services/apiservice.cpp
    src/services/serverprotocol.h
    src/services/serverprotocol.cpp
    src/utils/appstate.h
    src/utils/appstate.cpp
)

# Create test executables
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Test Models
    qt_add_executable(test_models
        ${TEST_MODELS_SOURCES}
    )
    target_link_libraries(test_models PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME TestModels COMMAND test_models)

    # Test Utils
    qt_add_executable(test_utils
        ${TEST_UTILS_SOURCES}
    )
    target_link_libraries(test_utils PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
    add_test(NAME TestUtils COMMAND test_utils)

    # Test Services
    qt_add_executable(test_services
        ${TEST_SERVICES_SOURCES}
    )
    target_link_libraries(test_services PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)
    add_test(NAME TestServices COMMAND test_services)

    # Test Navigation
    qt_add_executable(test_navigation
        ${TEST_NAVIGATION_SOURCES}
    )
    target_link_libraries(test_navigation PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
    add_test(NAME TestNavigation COMMAND test_navigation)

    # Test Network
    qt_add_executable(test_network
        ${TEST_NETWORK_SOURCES}
    )
    target_link_libraries(test_network PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)
    add_test(NAME TestNetwork COMMAND test_network)

    # Test Integration
    qt_add_executable(test_integration
        ${TEST_INTEGRATION_SOURCES}
    )
    target_link_libraries(test_integration PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME TestIntegration COMMAND test_integration)

else()
    # Qt5 tests
    add_executable(test_models ${TEST_MODELS_SOURCES})
    target_link_libraries(test_models PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME TestModels COMMAND test_models)

    add_executable(test_utils ${TEST_UTILS_SOURCES})
    target_link_libraries(test_utils PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
    add_test(NAME TestUtils COMMAND test_utils)

    add_executable(test_services ${TEST_SERVICES_SOURCES})
    target_link_libraries(test_services PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)
    add_test(NAME TestServices COMMAND test_services)

    add_executable(test_navigation ${TEST_NAVIGATION_SOURCES})
    target_link_libraries(test_navigation PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
    add_test(NAME TestNavigation COMMAND test_navigation)

    add_executable(test_network ${TEST_NETWORK_SOURCES})
    target_link_libraries(test_network PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)
    add_test(NAME TestNetwork COMMAND test_network)

    add_executable(test_integration ${TEST_INTEGRATION_SOURCES})
    target_link_libraries(test_integration PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME TestIntegration COMMAND test_integration)
endif()

