# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitclone-lastrun.txt" AND EXISTS "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitinfo.txt" AND
  "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitclone-lastrun.txt" IS_NEWER_THAN "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "E:/canhom/subprojects/Source/wxWidgets_external"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'E:/canhom/subprojects/Source/wxWidgets_external'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/wxWidgets/wxWidgets.git" "wxWidgets_external"
    WORKING_DIRECTORY "E:/canhom/subprojects/Source"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/wxWidgets/wxWidgets.git'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe" 
          checkout "v3.1.4" --
  WORKING_DIRECTORY "E:/canhom/subprojects/Source/wxWidgets_external"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v3.1.4'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "E:/canhom/subprojects/Source/wxWidgets_external"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'E:/canhom/subprojects/Source/wxWidgets_external'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitinfo.txt" "E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'E:/canhom/subprojects/Stamp/wxWidgets_external/wxWidgets_external-gitclone-lastrun.txt'")
endif()
