project(test-rss-parser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 CONFIG REQUIRED)
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
	Catch2::Catch2
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
	CATCH_CONFIG_MAIN
)
include(Catch)

catch_discover_tests(${PROJECT_NAME})