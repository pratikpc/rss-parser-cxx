project(sample-parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

file(GLOB_RECURSE LIBRARY_CXX_SOURCES src/*.cxx)
file(GLOB_RECURSE LIBRARY_HXX_SOURCES include/*.hxx)

target_sources(${PROJECT_NAME}
PRIVATE
${LIBRARY_CXX_SOURCES}
${LIBRARY_HXX_SOURCES}
)


target_link_libraries(${PROJECT_NAME}
PRIVATE
	pc::rss-parser
)

target_include_directories(${PROJECT_NAME}
	INTERFACE
	$<BUILD_INTERFACE:${RAPIDXML_INCLUDE_DIRS}>)