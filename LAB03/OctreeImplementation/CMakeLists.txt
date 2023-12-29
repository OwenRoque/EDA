cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(VTK_DIR "C:/Users/Owen/Documents/VTK/bin" CACHE PATH "Path to VTK build directory")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OctreeImplementation)

find_package(VTK COMPONENTS 
    CommonColor
    CommonCore
    FiltersSources
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "OctreeImplementation: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(OctreeImplementation "OctreeImplementation.cpp" "Octree.hpp" "Octree.cpp" "Point.hpp")
  target_link_libraries(OctreeImplementation PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS OctreeImplementation
  MODULES ${VTK_LIBRARIES}
)