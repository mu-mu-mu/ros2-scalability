cmake_minimum_required(VERSION 3.5)
project(tmpl)


# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rosidl_default_generators REQUIRED)
ament_export_include_directories(include)
ament_export_dependencies(rosidl_default_runtime)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Payload.msg"
 )

add_executable(talker src/pub.cpp)
ament_target_dependencies(talker rclcpp std_msgs)
rosidl_target_interfaces(talker
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

add_executable(listener src/sub.cpp)
ament_target_dependencies(listener rclcpp std_msgs)
rosidl_target_interfaces(listener
  ${PROJECT_NAME} "rosidl_typesupport_cpp")

# add_executable(tl src/pubsub.cpp)
# ament_target_dependencies(tl rclcpp std_msgs)

# EXEC HERE

include_directories(include)

install(TARGETS
  talker
#  tl
# DEP HERE
  listener
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
    launch
    DESTINATION share/${PROJECT_NAME})

ament_package()
