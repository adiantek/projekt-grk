cmake_minimum_required(VERSION 3.0)

project(GrafikaKomputerowa)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(EMSCRIPTEN TURE)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "Cmake prefix path: ${CMAKE_PREFIX_PATH}")

include_directories("include" "libraries" "physx_include")
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_DEBUG_POSTFIX _d)
add_definitions(/DPX_PHYSX_STATIC_LIB)

if (NOT EMSCRIPTEN)
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp-vc142-mtd.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw3dll.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlibstatic.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libpng16_static.lib")

        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXCharacterKinematic_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXCommon_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXCooking_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXExtensions_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXFoundation_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXPvdSDK_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysXVehicle_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_debug/PhysX_static_64.lib")

        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXCharacterKinematic_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXCommon_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXCooking_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXExtensions_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXFoundation_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXPvdSDK_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysXVehicle_static_64.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_win_release/PhysX_static_64.lib")

        add_custom_command(
            TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp-vc142-mtd.dll"
                    ${CMAKE_CURRENT_BINARY_DIR}/Release/assimp-vc142-mtd.dll)
        add_custom_command(
            TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp-vc142-mtd.dll"
                    ${CMAKE_CURRENT_BINARY_DIR}/Debug/assimp-vc142-mtd.dll)
        add_custom_command(
            TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw3.dll"
                    ${CMAKE_CURRENT_BINARY_DIR}/Release/glfw3.dll)
        add_custom_command(
            TARGET ${PROJECT_NAME}  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw3.dll"
                    ${CMAKE_CURRENT_BINARY_DIR}/Debug/glfw3.dll)
    else()
        target_link_libraries(${PROJECT_NAME} glfw)
        target_link_libraries(${PROJECT_NAME} assimp)
    endif()
endif()

if (EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libassimp.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libzlibstatic.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/libpng16.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXCharacterKinematic_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXCommon_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXCooking_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXExtensions_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXFoundation.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXPvdSDK_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysXVehicle_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physx_em/PhysX_static.a")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DEMSCRIPTEN=1)
    if (SPLIT_STORAGE)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s EXPORTED_RUNTIME_METHODS=ccall -s FORCE_FILESYSTEM=1")
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -s EXPORTED_RUNTIME_METHODS=ccall --preload-file ../assets")
    endif()
    add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_SOURCE_DIR}/index.html"
                ${CMAKE_CURRENT_BINARY_DIR}/index.html)
endif()

if (CMAKE_DL_LIBS)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})
endif()
