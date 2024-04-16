cmake_minimum_required(VERSION 3.28)
project(ldjamgame LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(KAREN_EMPSCRIPTEN OFF CACHE BOOL "enables web build")
set (SRC src/App.cpp src/GameLayer.cpp src/Animation.cpp src/AssetManager.cpp src/RandomHelper.cpp src/ParticleSystem.cpp src/Level.cpp src/Entity.cpp src/SweptAABB.cpp)
add_subdirectory(vendor/Karen/Karen/)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${KAREN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Karen)
if(${KAREN_IS_EMSCRIPTEN})
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html"
    LINK_FLAGS "-sUSE_GLFW=3 -sWASM=0 -sFULL_ES3 --preload-file ../res --bind -DKAREN_EMSCRIPTEN --shell-file template.html"

    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/web"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/web"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/web"
)
else()
 set_target_properties(${PROJECT_NAME} PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)
endif()

