include(ExternalProject)

set(CMAKE_ARGS
  -DCMAKE_POLICY_DEFAULT_CMP0091:STRING=NEW 
  -DCMAKE_MSVC_RUNTIME_LIBRARY:STRING=MultiThreaded$<$<CONFIG:Debug>:Debug>
  -DBUILD_SHARED_LIBS=OFF
  )

ExternalProject_Add(
  tinyxml2_project
  GIT_REPOSITORY    https://github.com/leethomason/tinyxml2.git
  GIT_TAG           master
  GIT_SHALLOW       1
  PREFIX            ${PROJECT_SOURCE_DIR}/extern/tinyxml2
  DOWNLOAD_DIR      ${PROJECT_SOURCE_DIR}/extern/tinyxml2
  SOURCE_DIR        ${PROJECT_SOURCE_DIR}/extern/tinyxml2/source
  BINARY_DIR        ${PROJECT_SOURCE_DIR}/extern/tinyxml2/build
  CMAKE_ARGS        ${CMAKE_ARGS}
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)

ExternalProject_Get_Property(tinyxml2_project source_dir)
ExternalProject_Get_Property(tinyxml2_project binary_dir)
set(TINYXML2_HEADERS ${source_dir})
set(TINYXML2_LIBS ${binary_dir}/Release)
