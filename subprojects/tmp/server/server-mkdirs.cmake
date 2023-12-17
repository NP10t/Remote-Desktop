# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
<<<<<<< HEAD
  "E:/canhom/src"
  "E:/canhom/subprojects/Build/server"
  "E:/canhom/subprojects/Install/server"
  "E:/canhom/subprojects/tmp/server"
  "E:/canhom/subprojects/Stamp/server"
  "E:/canhom/subprojects/Download/server"
  "E:/canhom/subprojects/Stamp/server"
=======
  "C:/Do an mmt/Mang_May_Tinh-master/src"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Build/server"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Install/server"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/tmp/server"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/server"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Download/server"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/server"
>>>>>>> nguyenquangthinh
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
<<<<<<< HEAD
    file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/server/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/server${cfgdir}") # cfgdir has leading slash
=======
    file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/server/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/server${cfgdir}") # cfgdir has leading slash
>>>>>>> nguyenquangthinh
endif()
