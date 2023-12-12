# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/canhom/src"
  "E:/canhom/subprojects/Build/client"
  "E:/canhom/subprojects/Install/client"
  "E:/canhom/subprojects/tmp/client"
  "E:/canhom/subprojects/Stamp/client"
  "E:/canhom/subprojects/Download/client"
  "E:/canhom/subprojects/Stamp/client"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/client/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/client${cfgdir}") # cfgdir has leading slash
endif()
