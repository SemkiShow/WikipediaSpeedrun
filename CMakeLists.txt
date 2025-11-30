cmake_minimum_required(VERSION 3.14)
project(WikipediaSpeedrun)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

file(GLOB SOURCES src/*.cpp include/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include/)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

if(WIN32 AND CMAKE_CROSSCOMPILING)
    set(QT_DLLS
        "Qt6Core.dll"
        "Qt6Gui.dll"
        "Qt6Widgets.dll"

        "libgcc_s_seh-1.dll"
        "libwinpthread-1.dll"
        "libstdc++-6.dll"
        "icui18n76.dll"
        "icuuc76.dll"
        "libpcre2-16-0.dll"
        "zlib1.dll"
        "icudata76.dll"
        "libexpat-1.dll"
        "libbz2-1.dll"
        "libfreetype-6.dll"
        "libfontconfig-1.dll"
        "libglib-2.0-0.dll"
        "libharfbuzz-0.dll"
        "libintl-8.dll"
        "iconv.dll"
        "libpng16-16.dll"
        "libpcre2-8-0.dll"
    )

    foreach(dll ${QT_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "/usr/x86_64-w64-mingw32/sys-root/mingw/bin/${dll}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach()
endif()
