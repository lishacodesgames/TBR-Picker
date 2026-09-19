cmake_minimum_required(VERSION 3.20)
project(TBR LANGUAGES C CXX)

file(GLOB_RECURSE MY_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
   ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(${PROJECT_NAME} ${MY_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pch/Precompiled.h)

# (MSVC has different warning flags)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   # turn on all warnings for better debugging
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

   # disable harmless warnings
   target_compile_options(${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers -Wno-unused-result) 
endif()

# copy assets/ into bin/ to remove dependencies and weird paths in code
add_custom_target(copy_assets ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
   COMMENT "Copying assets to output directory"
)
add_dependencies(${PROJECT_NAME} copy_assets)
