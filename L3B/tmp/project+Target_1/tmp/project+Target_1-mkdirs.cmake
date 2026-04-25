# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1")
  file(MAKE_DIRECTORY "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1")
endif()
file(MAKE_DIRECTORY
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/1"
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1"
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/tmp"
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/src/project+Target_1-stamp"
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/src"
  "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/src/project+Target_1-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/src/project+Target_1-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/nnicholls/VSCODE Projects/Template_L3/L3B/tmp/project+Target_1/src/project+Target_1-stamp${cfgdir}") # cfgdir has leading slash
endif()
