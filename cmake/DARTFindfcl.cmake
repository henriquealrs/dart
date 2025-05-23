# Copyright (c) 2011-2025, The DART development contributors
# All rights reserved.
#
# The list of contributors can be found at:
#   https://github.com/dartsim/dart/blob/main/LICENSE
#
# This file is provided under the "BSD-style" License

find_package(fcl 0.7.0 CONFIG)
if(NOT FCL_FOUND AND NOT fcl_FOUND)
  find_package(fcl 0.7.0 REQUIRED MODULE)
endif()

# Set target fcl if not set
# Upstream provides the target since 0.5.0 but some package managers don't
# install the config file, which defines the target.
if((FCL_FOUND OR fcl_FOUND) AND NOT TARGET fcl)
  add_library(fcl INTERFACE IMPORTED)
  set_target_properties(fcl PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${FCL_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${FCL_LIBRARIES}"
  )
endif()
