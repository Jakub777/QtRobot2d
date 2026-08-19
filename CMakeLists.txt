cmake_minimum_required(VERSION 3.16)

project(HelloQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

qt_standard_project_setup()

qt_add_executable(HelloQt
  main.cpp
  point.cpp
  robot_view_data.h
  robot_tolerances.h
  robot_algorithm.h
  point_to_point_algorithm.cpp
  point_to_point_algorithm.h
  my_button.cpp
  canvas.cpp
  r_joint.cpp
  r_link.cpp
  r_robot.cpp
  r_segment.cpp
  gui_labeled_double_spinbox.cpp
  gui_segment_widget.cpp
  robot_manager.cpp
  robot_controller.cpp
)

target_link_libraries(HelloQt PRIVATE Qt6::Widgets Qt6::Gui)

install(TARGETS HelloQt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
