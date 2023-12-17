# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
<<<<<<< HEAD
  "E:/canhom/src"
  "E:/canhom/subprojects/Build/client"
  "E:/canhom/subprojects/Install/client"
  "E:/canhom/subprojects/tmp/client"
  "E:/canhom/subprojects/Stamp/client"
  "E:/canhom/subprojects/Download/client"
  "E:/canhom/subprojects/Stamp/client"
=======
  "C:/Do an mmt/Mang_May_Tinh-master/src"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Build/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Install/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/tmp/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Download/client"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client"
>>>>>>> nguyenquangthinh
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
<<<<<<< HEAD
    file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/client/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/client${cfgdir}") # cfgdir has leading slash
=======
    file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/client${cfgdir}") # cfgdir has leading slash
>>>>>>> nguyenquangthinh
endif()
