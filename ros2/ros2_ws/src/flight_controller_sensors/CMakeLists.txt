cmake_minimum_required(VERSION 3.8)
project(flight_controller_sensors)

include(ExternalProject)

#Flight Controller Lib
if(NOT DEFINED NAVIGATOR_VERSION)
    set(ZIP_URL "https://github.com/bluerobotics/navigator-lib/releases/latest/download/cpp.zip")
    message(STATUS "Using Navigator lib version: latest")
else()
    set(ZIP_URL "https://github.com/bluerobotics/navigator-lib/releases/download/${NAVIGATOR_VERSION}/cpp.zip")
    message(STATUS "Using Navigator lib version: ${NAVIGATOR_VERSION}")
endif()

set(ZIP_UNPACK_DIR "${CMAKE_BINARY_DIR}/navigator_zip_contents")
message(STATUS "Identified system processor: ${CMAKE_SYSTEM_PROCESSOR}")

# Download released navigator lib files
ExternalProject_Add(
    navigator_zip
    URL ${ZIP_URL}
    DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/navigator_zip_download"
    SOURCE_DIR ${ZIP_UNPACK_DIR}
    # Since this is not a cmake project but just files, we need to cleanup the commands
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
)

# Adjust library path based on the system processor
if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv7")
    set(LIB_PATH_SUBDIR "armv7-unknown-linux-gnueabihf")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv8" OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    set(LIB_PATH_SUBDIR "aarch64-unknown-linux-gnu")
else()
    message(FATAL_ERROR "Unsupported architecture")
endif()

set(LIBRARY_PATH "${ZIP_UNPACK_DIR}/${LIB_PATH_SUBDIR}/libbluerobotics_navigator.so")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(sensor_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(flight_controller_sensors_publisher src/flight_controller_sensors_publisher.cpp)
add_dependencies(flight_controller_sensors_publisher navigator_zip)
ament_target_dependencies(flight_controller_sensors_publisher rclcpp example_interfaces)
ament_target_dependencies(flight_controller_sensors_publisher rclcpp sensor_msgs)

target_link_libraries(flight_controller_sensors_publisher ${LIBRARY_PATH})
set_target_properties(flight_controller_sensors_publisher PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
target_include_directories(flight_controller_sensors_publisher PRIVATE "${ZIP_UNPACK_DIR}")

install(PROGRAMS ${LIBRARY_PATH}
  DESTINATION lib
)

install(TARGETS
  flight_controller_sensors_publisher
  DESTINATION lib/${PROJECT_NAME})

ament_package()
