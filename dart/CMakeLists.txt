add_subdirectory(external)

# Enable multi-threaded compilation.
# We do this here and not in the root folder since the examples and the
# tutorials do not have enough source files to benefit from this.
if(MSVC AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DART_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DART_BINARY_DIR}/lib")

#===============================================================================
# Source directories
#===============================================================================
# common
# math
# integration
# lcpsolver
# optimizer
#   ipopt
#   nlopt
# dynamics
# collision
#   dart
#   fcl
#   bullet
# constraint
# simulation
# utils
#   sdf
#   urdf
# gui
#   osg
#     render

#===============================================================================
# Targets - {dependency targets}, source directories, [external dependency libs]
#===============================================================================
# dart - common, math, integration, lcpsolver, optimizer, dynamics, collision,
#        collision/dart, collision/fcl, constraint, simulation, [assimp, eigen3,
#        fcl, fmt]
# dart-optimizer-ipopt - {dart}, optimizer/ipopt, [ipopt]
# dart-optimizer-nlopt - {dart}, optimizer/nlopt, [nlopt]
# dart-collision-bullet - {dart}, collision/bullet, [bullet]
# dart-utils - {dart}, utils, utils/sdf, [tinyxml2]
# dart-utils-urdf - {dart-utils}, utils/urdf, [urdfdom]
# dart-gui - {dart}, gui, [opengl, glut]
# dart-gui-osg - {dart-gui}, gui/osg, gui/osg/render, [openscenegraph]

#===============================================================================
# Components - (dependency component), {dependency targets}
#===============================================================================
# dart - {dart}
# optimizer-ipopt - (dart), {dart-optimizer-ipopt}
# optimizer-nlopt - (dart), {dart-optimizer-nlopt}
# collision-bullet - (dart), {dart-collision-bullet}
# utils - (collision-bullet), {dart-utils}
# utils-urdf - (utils), {dart-utils-urdf}
# gui - (utils), {dart-gui}
# gui-osg - (gui), {dart-gui-osg}

# Component. This should be called before adding subdirectories.
add_component(${PROJECT_NAME} dart)

function(dart_add_core_headers)
  dart_property_add(DART_CORE_HEADERS ${ARGN})
endfunction(dart_add_core_headers)

function(dart_add_core_sources)
  dart_property_add(DART_CORE_SOURCES ${ARGN})
endfunction(dart_add_core_sources)

# Add subdirectories
add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(integration)
add_subdirectory(lcpsolver)
add_subdirectory(optimizer)
add_subdirectory(dynamics)
add_subdirectory(collision)
add_subdirectory(constraint)
add_subdirectory(simulation)
add_subdirectory(utils) # tinyxml2, bullet
add_subdirectory(gui) # opengl, glut, bullet

set(DART_CONFIG_HPP_IN ${DART_SOURCE_DIR}/dart/config.hpp.in)
set(DART_CONFIG_HPP_OUT ${DART_BINARY_DIR}/dart/config.hpp)
if(DART_VERBOSE)
  message(STATUS ${DART_CONFIG_HPP_OUT})
endif()
configure_file(${DART_CONFIG_HPP_IN} ${DART_CONFIG_HPP_OUT} @ONLY)
install(FILES ${DART_CONFIG_HPP_OUT} DESTINATION include/dart)

# Print building component
get_property(components GLOBAL PROPERTY ${PROJECT_NAME}_COMPONENTS)
if(DART_VERBOSE)
  message(STATUS "")
  message(STATUS "[ Components ]")
  foreach(component ${components})
    message(STATUS "Adding component: ${component}")
  endforeach()
else()
  list(LENGTH components components_length)
  message(STATUS "Adding ${components_length} components including "
    "the default component 'dart'")
endif()

# Set header and source files
get_filename_component(dart_hpp "dart.hpp" ABSOLUTE)
dart_add_core_headers(${dart_hpp})
get_property(dart_core_headers GLOBAL PROPERTY DART_CORE_HEADERS)
get_property(dart_core_sources GLOBAL PROPERTY DART_CORE_SOURCES)

# Add target
dart_add_library(dart ${dart_core_headers} ${dart_core_sources})
target_include_directories(dart BEFORE
  PUBLIC
    $<BUILD_INTERFACE:${DART_SOURCE_DIR}>
    $<BUILD_INTERFACE:${DART_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(dart
  PUBLIC
    ${CMAKE_DL_LIBS}
    ${PROJECT_NAME}-external-odelcpsolver
    Eigen3::Eigen
    fcl
    assimp
)
if(TARGET fmt::fmt-header-only)
  target_link_libraries(dart PUBLIC fmt::fmt-header-only)
elseif(TARGET fmt::fmt)
  target_link_libraries(dart PUBLIC fmt::fmt)
else()
  message(FATAL_ERROR "Failed to find fmt targets")
endif()

# spdlog settings
if(spdlog_FOUND)
  # Prefer header only target
  if(TARGET spdlog::spdlog_header_only)
    target_link_libraries(dart PUBLIC spdlog::spdlog_header_only)
    target_compile_definitions(dart PUBLIC -DDART_HAVE_spdlog=1)
  elseif(TARGET spdlog::spdlog)
    target_link_libraries(dart PUBLIC spdlog::spdlog)
    target_compile_definitions(dart PUBLIC -DDART_HAVE_spdlog=1)
  else()
    message(WARNING "spdlog found, but no target is found. Expected spdlog::spdlog or target_link_library_public spdlog::spdlog_header_only")
    target_compile_definitions(dart PUBLIC -DDART_HAVE_spdlog=0)
  endif()
else()
  target_compile_definitions(dart PUBLIC -DDART_HAVE_spdlog=0)
endif()

# octomap settings
if(TARGET octomap)
  target_link_libraries(dart PUBLIC octomap)
endif()

# C++ standard settings
target_compile_features(dart PUBLIC cxx_std_17)

# Build DART with all available SIMD instructions
if(DART_ENABLE_SIMD)
  if(MSVC)
    target_compile_options(dart PUBLIC /arch:SSE2 /arch:AVX /arch:AVX2)
    # /arch:SSE2 option is effective only for x86 but not for x64 since it's
    # enabled for x64 by default. The option will be ignored on x64 with a
    # warning message. If anyone knows how to detect the system's architecture
    # in CMake time, then I would gratefully apply these options differently
    # depending on the architectures.
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(dart PUBLIC -march=native)
    if(GCC_VERSION VERSION_GREATER 7.0)
      target_compile_options(dart PUBLIC -faligned-new)
    endif()
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(dart PUBLIC -march=native -faligned-new)
  endif()
else()
  # TODO: Improve this part to detect the system's architecture and apply the
  # appropriate SIMD instructions.
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    # Remove SSE flags if set elsewhere
    string(REPLACE "-msse" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-msse2" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-msse3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "-mssse3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()
endif()

# Active log level
if(DART_ACTIVE_LOG_LEVEL STREQUAL "TRACE")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=0)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "DEBUG")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=1)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "INFO")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=2)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "WARN")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=3)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "ERROR")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=4)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "FATAL")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=5)
elseif(DART_ACTIVE_LOG_LEVEL STREQUAL "OFF")
  target_compile_definitions(dart PUBLIC -DDART_ACTIVE_LOG_LEVEL=6)
else()
  message(FATAL_ERROR "Invalid active log level: ${DART_ACTIVE_LOG_LEVEL}")
endif()

# Default component
add_component_targets(${PROJECT_NAME} dart dart)
add_component_dependencies(${PROJECT_NAME} dart external-odelcpsolver)
add_component_dependency_packages(${PROJECT_NAME} dart
  assimp Eigen3 fcl fmt
)
if(TARGET octomap)
  add_component_dependency_packages(${PROJECT_NAME} dart octomap)
endif()
if(spdlog_FOUND)
  add_component_dependency_packages(${PROJECT_NAME} dart spdlog)
endif()

if(MSVC)
  set_target_properties(
    dart PROPERTIES
    STATIC_LIBRARY_FLAGS_RELEASE "/LTCG"
  )
endif()

if(DART_CODECOV)
  target_link_libraries(dart PUBLIC coverage_config)
endif()

if(DART_BUILD_PROFILE)
  target_link_libraries(dart PUBLIC Tracy::TracyClient)
endif()

install(FILES dart.hpp DESTINATION include/dart/ COMPONENT headers)

dart_format_add(${dart_core_headers} ${dart_core_sources})
