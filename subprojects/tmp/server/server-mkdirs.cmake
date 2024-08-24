# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/canhom/src"
  "E:/canhom/subprojects/Build/server"
  "E:/canhom/subprojects/Install/server"
  "E:/canhom/subprojects/tmp/server"
  "E:/canhom/subprojects/Stamp/server"
  "E:/canhom/subprojects/Download/server"
  "E:/canhom/subprojects/Stamp/server"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/server/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/server${cfgdir}") # cfgdir has leading slash
endif()
