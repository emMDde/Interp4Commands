cmake_minimum_required(VERSION 3.0)

set(PROJ_NAME "cmds_interpreter")
project(${PROJ_NAME})

add_subdirectory(plugin)

find_package(XercesC 3.2.0 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/inc)
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/XMLInterp4Config.cpp
    src/LibInterface.cpp
    src/Set4LibInterface.cpp
    src/ProgramInterpreter.cpp
    src/Scene.cpp
    src/Cuboid.cpp
    src/ComChannel.cpp
)
target_link_libraries(${PROJECT_NAME} xerces-c)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,-rpath=./plugin")
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal program:  ${PROJ_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/src/program_dzialan.cmd" # Kopiowanie pliku program_dzialan do przetworzenia dla preprocesora
        "${PROJECT_BINARY_DIR}/program_dzialan.cmd"
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/config config
  )
