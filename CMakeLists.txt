cmake_minimum_required (VERSION 3.15)

project (oop2_project_shaked_v_noy_t_rina_s_salomon_d)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline)
endif ()

add_executable (oop2_project_shaked_v_noy_t_rina_s_salomon_d WIN32 "") 

add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (resources)
add_subdirectory (external/box2d/src)


set (SFML_STATIC_LIBRARIES TRUE)
set (SFML_DIR "C:/SFML/SFML-2.5.1/lib/cmake/SFML")

find_package (SFML 2.5 COMPONENTS graphics audio REQUIRED)

set_target_properties (sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

target_link_libraries (oop2_project_shaked_v_noy_t_rina_s_salomon_d sfml-graphics sfml-main sfml-audio)

configure_file (C:/SFML/SFML-2.5.1/bin/openal32.dll ${CMAKE_BINARY_DIR} COPYONLY)

target_link_libraries (oop2_project_shaked_v_noy_t_rina_s_salomon_d box2d)