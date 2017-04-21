project(gvfspanel)

set(HEADERS
    src/gvfsdlg.h
    src/GvfsService.h
    src/GvfsServiceException.h
    src/KeyBarTitlesHelper.h
    src/LngStringIDs.h
    src/MountPoint.h
    src/MountPointStorage.h
    src/Plugin.h
)

set(SOURCES
    src/gvfsdlg.cpp
    src/GvfsService.cpp
    src/KeyBarTitlesHelper.cpp
    src/MountPoint.cpp
    src/MountPointStorage.cpp
    src/Plugin.cpp
    src/PluginMain.cpp
)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(UUID uuid)

add_library (${PROJECT_NAME} MODULE ${HEADERS} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME}
    utils
    ${GTKMM_LIBRARIES}
    ${UUID_LIBRARIES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DWINPORT_DIRECT
    -DUNICODE
    -DFAR_DONT_USE_INTERNALS
    -DPLUGIN_NAME="${PROJECT_NAME}"
)

target_include_directories(${PROJECT_NAME} PRIVATE ./inc)
target_include_directories(${PROJECT_NAME} PRIVATE ../far2l)
target_include_directories(${PROJECT_NAME} PRIVATE ../far2l/Include)
target_include_directories(${PROJECT_NAME} PRIVATE ../WinPort)
target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${UUID_INCLUDE_DIRS})

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${INSTALL_DIR}/Plugins/${PROJECT_NAME}/plug"
    PREFIX ""
    SUFFIX ".far-plug-wide")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/configs
   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configs "${INSTALL_DIR}/Plugins/${PROJECT_NAME}"
)