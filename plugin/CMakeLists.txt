set(CURRENT_TARGET "krunner_kiten")
set(CURRENT_TARGET_NAME "krunner_kiten")
set(CURRENT_TARGET_INTERFACE "${CURRENT_TARGET}_interface")

message(STATUS "Configuring ${CURRENT_TARGET}...")

# Qt
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

# KDE
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH}")

include(KDEInstallDirs)
#include(KDECMakeSettings)
#include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
find_package(KF5Runner REQUIRED)
find_package(KF5I18n REQUIRED)

# Qt automoc
message(STATUS "Enabling Qt automoc...")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# create target after qt setup
CreateTarget(${CURRENT_TARGET} SHARED ${CURRENT_TARGET_NAME} 17)

# Qt deprecated warnings
target_compile_definitions(${CURRENT_TARGET} PRIVATE -DQT_DEPRECATED_WARNINGS)
target_compile_definitions(${CURRENT_TARGET} PRIVATE -DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# disable Qt foreach macro
target_compile_definitions(${CURRENT_TARGET} PRIVATE -DQT_NO_FOREACH)

target_link_libraries(${CURRENT_TARGET}
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        KF5::Runner
        KF5::I18n
        -lkiten
)

kcoreaddons_desktop_to_json(${CURRENT_TARGET} plasma-runner-kiten.desktop)

install(TARGETS "${CURRENT_TARGET}" DESTINATION "${KDE_INSTALL_PLUGINDIR}/kf5/krunner")

message(STATUS "Configured ${CURRENT_TARGET}.")
