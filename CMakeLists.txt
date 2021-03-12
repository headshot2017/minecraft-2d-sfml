cmake_minimum_required(VERSION 3.1)
project(minecrap)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(OS_LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	set(OS_MAC 1)
endif()


if(NOT OS_MAC) # mac .app bundles have Frameworks, no need for static link
	set(SFML_STATIC_LIBRARIES TRUE)
endif()

set(SFML_DIR "sfml/build") # git clone https://github.com/sfml/sfml


file(GLOB SOURCES
	icon.rc
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

if(OS_WINDOWS)
	link_directories(${CMAKE_SOURCE_DIR}/lib/windows)
	file(COPY ${CMAKE_SOURCE_DIR}/lib/windows/bass.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif(OS_LINUX)
	link_directories(${CMAKE_SOURCE_DIR}/lib/linux)
	file(COPY ${CMAKE_SOURCE_DIR}/lib/linux/libbass.so DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
elseif(OS_MAC)
	link_directories(${CMAKE_SOURCE_DIR}/lib/mac)
	file(COPY ${CMAKE_SOURCE_DIR}/lib/mac/libbass.dylib DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(minecrap ${SOURCES})
if(OS_WINDOWS)
	target_link_libraries(minecrap -static -static-libgcc -static-libstdc++ sfml-graphics sfml-system sfml-window bass)
else()
	target_link_libraries(minecrap -Wl,-rpath,. sfml-graphics sfml-system sfml-window bass)
	if(OS_MAC)
		target_link_libraries(minecrap -Wl,-rpath,../sfml/extlibs/libs-osx/Frameworks "-framework OpenGL" -stdlib=libc++)
		target_compile_options(minecrap PRIVATE -stdlib=libc++)
		set_target_properties(minecrap PROPERTIES
			BUNDLE True
			MACOSX_BUNDLE_GUI_IDENTIFIER world.tee.headbot.minecraft2d
			MACOSX_BUNDLE_BUNDLE_NAME "Minecraft 2D"
			MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
			MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
			#MACOSX_BUNDLE_INFO_PLIST
		)
	endif()
endif()
