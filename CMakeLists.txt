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
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if (NOT EMSCRIPTEN)
    if(WIN32)
        set(CompilerFlags
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
            )
        foreach(CompilerFlag ${CompilerFlags})
            string(REPLACE "/MTd" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
            string(REPLACE "/MT" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
            string(REPLACE "/MDd" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
            string(REPLACE "/MD" "/MD" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach()
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/zlibstatic.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/libpng16_static.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/assimp-vc142-mt.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/glfw3.lib")
        # target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_arm64ec_static/msvcprt.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXCharacterKinematic_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXCommon_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXCooking_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXExtensions_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXFoundation_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXPvdSDK_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysXVehicle_static_64.lib")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/win_amd64_static/PhysX_static_64.lib")
    else()
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libzlibstatic.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libpng16.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libassimp.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libz.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXExtensions_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysX_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXPvdSDK_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXVehicle_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXCharacterKinematic_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXCooking_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXCommon_static_64.a")
        target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/linux_amd64_static/libPhysXFoundation_static_64.a")
        target_link_libraries(${PROJECT_NAME} "glfw")
        target_link_libraries(${PROJECT_NAME} "pthread")
    endif()
else()
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/libzlibstatic.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/libpng16.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/libassimp.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXCharacterKinematic_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXCommon_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXCooking_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXExtensions_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXFoundation.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXPvdSDK_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysXVehicle_static.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/em/PhysX_static.a")
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
