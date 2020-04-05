enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)

include(${PROJECT_SOURCE_DIR}/extern/cmake/tinyxml2.cmake)

set(SOURCES dllmain.cpp libcryxml.cpp types.cpp)

add_library(libcryxml SHARED ${SOURCES})

target_compile_definitions(libcryxml PUBLIC -DBUILD_DLL)

set_property(TARGET libcryxml PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_dependencies(libcryxml tinyxml2_project)

target_include_directories(libcryxml PUBLIC
                           "${TINYXML2_HEADERS}"
)

target_link_directories(libcryxml PUBLIC
                        "${TINYXML2_LIBS}"
)

target_link_libraries(libcryxml tinyxml2)

install(TARGETS libcryxml
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist
        ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/dist
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/dist
)

