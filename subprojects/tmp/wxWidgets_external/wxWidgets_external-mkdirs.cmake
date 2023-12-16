# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
<<<<<<< HEAD
  "E:/canhom/subprojects/Source/wxWidgets_external"
  "E:/canhom/subprojects/Build/wxWidgets_external"
  "E:/canhom/subprojects/Install/wxWidgets_external"
  "E:/canhom/subprojects/tmp/wxWidgets_external"
  "E:/canhom/subprojects/Stamp/wxWidgets_external"
  "E:/canhom/subprojects/Download/wxWidgets_external"
  "E:/canhom/subprojects/Stamp/wxWidgets_external"
=======
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Source/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Build/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Install/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/tmp/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Download/wxWidgets_external"
  "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/wxWidgets_external"
>>>>>>> nguyenquangthinh
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
<<<<<<< HEAD
    file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/wxWidgets_external/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/canhom/subprojects/Stamp/wxWidgets_external${cfgdir}") # cfgdir has leading slash
=======
    file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/wxWidgets_external/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Do an mmt/Mang_May_Tinh-master/subprojects/Stamp/wxWidgets_external${cfgdir}") # cfgdir has leading slash
>>>>>>> nguyenquangthinh
endif()
