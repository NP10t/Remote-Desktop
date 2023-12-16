# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Do an mmt/Mang_May_Tinh-master/src"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Build/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Install/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/tmp/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Download/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client${cfgdir}") # cfgdir has leading slash
endif()
