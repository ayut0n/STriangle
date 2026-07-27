# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/github_projects/fractals/build/_deps/discord_rpc-src")
  file(MAKE_DIRECTORY "D:/github_projects/fractals/build/_deps/discord_rpc-src")
endif()
file(MAKE_DIRECTORY
  "D:/github_projects/fractals/build/_deps/discord_rpc-build"
  "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix"
  "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/tmp"
  "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/src/discord_rpc-populate-stamp"
  "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/src"
  "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/src/discord_rpc-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/src/discord_rpc-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/github_projects/fractals/build/_deps/discord_rpc-subbuild/discord_rpc-populate-prefix/src/discord_rpc-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
