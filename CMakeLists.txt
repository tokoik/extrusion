cmake_minimum_required(VERSION 3.21)

# Allow configuring legacy subprojects (freeglut) with newer CMake versions.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(extrusion LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXTRUSION_SOURCES
  main.cpp
  extrusion.cpp
  quaternion.cpp
)

set(EXTRUSION_HEADERS
  extrusion.h
  quaternion.h
)

if(APPLE)
  add_executable(extrusion MACOSX_BUNDLE
    ${EXTRUSION_SOURCES}
    ${EXTRUSION_HEADERS}
  )

  find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
  find_library(GLUT_FRAMEWORK GLUT REQUIRED)

  target_link_libraries(extrusion PRIVATE
    ${OPENGL_FRAMEWORK}
    ${GLUT_FRAMEWORK}
  )

  set_target_properties(extrusion PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME extrusion
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.extrusion"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
    MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2026 Kohe Tokoi. All rights reserved."
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.yourcompany.extrusion"
    XCODE_ATTRIBUTE_PRODUCT_NAME extrusion
    XCODE_GENERATE_SCHEME ON
    XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  )

  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
    set_target_properties(extrusion PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
  endif()

  set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY XCODE_GENERATE_SCHEME ON)

elseif(WIN32)
  include(FetchContent)

  set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
  set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  set(INSTALL_PDB OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(
    freeglut
    GIT_REPOSITORY https://github.com/freeglut/freeglut.git
    GIT_TAG v3.6.0
  )
  FetchContent_MakeAvailable(freeglut)

  add_executable(extrusion
    ${EXTRUSION_SOURCES}
    ${EXTRUSION_HEADERS}
  )

  target_link_libraries(extrusion PRIVATE
    freeglut
    opengl32
    glu32
    winmm
  )

else()
  add_executable(extrusion
    ${EXTRUSION_SOURCES}
    ${EXTRUSION_HEADERS}
  )

  find_package(OpenGL REQUIRED)
  find_package(GLUT REQUIRED)

  target_link_libraries(extrusion PRIVATE
    OpenGL::GL
    OpenGL::GLU
    GLUT::GLUT
    m
  )
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT extrusion)
