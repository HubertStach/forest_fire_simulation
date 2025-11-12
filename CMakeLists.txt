cmake_minimum_required(VERSION 3.24)
project(cw5 LANGUAGES CXX) # ##### RENAME ME! #####

# Workaround for Clang + raylib compound literals
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===== Dependencies already in your project =====
include(FetchContent)
include(cmake/raylib.cmake)
include(cmake/imgui.cmake)
include(cmake/rlimgui.cmake)

# ===== OpenCV =====
# Jeśli CMake nie znajdzie OpenCV na Windows, podaj:
#   -DOpenCV_DIR="D:/path/to/opencv/build/x64/vc16/lib"
# lub użyj vcpkg toolchaina.
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

# ===== Sources =====
set(SOURCE_FILES
    Main.cpp
    automat.cpp
    image.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Linkowanie istniejących bibliotek (rlimgui łączy raylib + imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE rlimgui)

# Dodajemy OpenCV: include’y + biblioteki
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

# Dodatkowe logi przy konfiguracji (pomocne w debugowaniu)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")

# macOS frameworki (zostawiam bez zmian)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Kopiowanie/symlink katalogu z danymi
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/Data/
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Data/)
