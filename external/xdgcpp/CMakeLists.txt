cmake_minimum_required(VERSION 3.6)
project(xdgcpp VERSION 0.1.0)

include(GNUInstallDirs)

set(XDG_BUILD_TESTS OFF CACHE BOOL "Toggles building of the xdg test suite")

set(XDGCPP_VERSION ${PROJECT_VERSION})

add_library(xdgcpp STATIC src/xdg.cpp)
set_property(TARGET xdgcpp PROPERTY CXX_STANDARD 17)
set_target_properties(xdgcpp PROPERTIES PUBLIC_HEADER "include/xdg.h")
target_include_directories(xdgcpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/xdgcpp>
  )
add_executable(xdgcpp-info src/xdg_info.cpp)
set_property(TARGET xdgcpp-info PROPERTY CXX_STANDARD 17)
target_link_libraries(xdgcpp-info xdgcpp)

if(XDG_BUILD_TESTS)
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)

  enable_testing()
  add_executable(xdgcpp_test src/xdg_test.cpp)
  set_property(TARGET xdgcpp_test PROPERTY CXX_STANDARD 17)
  target_compile_definitions(xdgcpp_test PRIVATE
    -DBOOST_TEST_DYN_LINK
    -DBOOST_TEST_MAIN
    -DBOOST_TEST_MODULE=xdgcpp
    )
  target_link_libraries(xdgcpp_test
    xdgcpp
    Boost::unit_test_framework
    )

  add_test(xdgcpp_test xdgcpp_test)
endif()

install(TARGETS xdgcpp xdgcpp-info
  EXPORT xdgcpp-targets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# CMake integration
configure_file(xdgcpp-config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/xdgcpp-config.cmake" @ONLY)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/xdgcpp-config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/xdgcpp/")
export(TARGETS xdgcpp
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/xdgcpp-targets.cmake")
export(PACKAGE xdgcpp)
install(EXPORT xdgcpp-targets
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/xdgcpp/")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/xdgcpp.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/xdgcpp.pc"
  @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/xdgcpp.pc"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# EOF #
