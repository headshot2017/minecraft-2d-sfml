cmake_minimum_required(VERSION 3.1)

project(minecrap)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "sfml/build") # git clone https://github.com/sfml/sfml


file(GLOB SOURCES
	*.h
	*.cpp
	gui/*.h
	gui/*.cpp
	submenus/*.h
	submenus/*.cpp
	game/*.h
	game/*.cpp
	game/entities/*.h
	game/entities/*.cpp
	game/entities/particles/*.h
	game/entities/particles/*.cpp
)


find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(minecrap ${SOURCES})
target_link_libraries(minecrap sfml-graphics sfml-system sfml-window bass)
