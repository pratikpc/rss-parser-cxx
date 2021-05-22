cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rss-parser
	VERSION 1.0.0
	DESCRIPTION
	"A header only c++ RSS Parser library. Supports C++20 Generators"
	HOMEPAGE_URL  "https://github.com/pratikpc/rss-parser-cxx")

find_package(pugixml CONFIG REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)
add_library(pc::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Adding the install interface generator expression makes sure that the include
# files are installed to the proper location (provided by GNUInstallDirs)
target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
			$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_link_libraries(${PROJECT_NAME}
	INTERFACE 
	pugixml::pugixml)

if(DEFINED rss_parser_sample)
	add_subdirectory(sample)
endif()

# locations are provided by GNUInstallDirs
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}_Targets
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
								 VERSION ${PROJECT_VERSION}
								 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION
  ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
			  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/pc DESTINATION include)
