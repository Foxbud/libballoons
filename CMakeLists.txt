cmake_minimum_required(VERSION 3.19)
# Copyright 2020 the libballoons authors
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Update this as you add new source files.
set(SRC
	src/confvars.c
	src/moddef.c
	src/objects.c
	src/pseudoevents.c
	src/sprites.c
	src/obj/balloonbase.c
	src/obj/ballooninflating.c
	src/obj/ballooninflated.c
	src/obj/balloondying.c
	src/obj/ballooncarcass.c)

# Read project metadata from ModInfo.json.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/ModInfo.json" MOD_INFO)
string(JSON MOD_NAME
	GET ${MOD_INFO} name)
string(JSON MOD_VERSION
	GET ${MOD_INFO} version)
string(JSON MOD_DESCRIPTION
	GET ${MOD_INFO} description)
string(JSON MOD_HOMEPAGE
	GET ${MOD_INFO} homepage)

# Define project.
project(${MOD_NAME}
	VERSION ${MOD_VERSION}
	DESCRIPTION ${MOD_DESCRIPTION}
	HOMEPAGE_URL ${MOD_HOMEPAGE}
	LANGUAGES C)

# Include CPack.
set(CPACK_PACKAGE_FILE_NAME "lib${MOD_NAME}-${MOD_VERSION}")
include(CPack)

# Include GenerateExportHeader.
include(GenerateExportHeader)

# Set cmake defaults.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX "lib${MOD_NAME}-${MOD_VERSION}"
		CACHE PATH "..." FORCE)
endif()

# Add mod shared library target.
add_library(${MOD_NAME} SHARED ${SRC})
generate_export_header(${MOD_NAME}
	BASE_NAME MOD
	EXPORT_FILE_NAME include/export.h)
set_target_properties(${MOD_NAME} PROPERTIES
	C_VISIBILITY_PRESET hidden
	CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)
target_include_directories(${MOD_NAME}
	PRIVATE include
	PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
target_compile_options(${MOD_NAME}
	PRIVATE -m32
	PRIVATE -Wall
	PRIVATE -Wextra
	PRIVATE -Werror)
target_link_options(${MOD_NAME}
	PRIVATE -m32
	PRIVATE -rdynamic)

# Add installation target.
install(FILES
	ModInfo.json
	LICENSE.assets.txt
	LICENSE.source.txt
	DESTINATION ".")
install(TARGETS ${MOD_NAME} LIBRARY)
install(DIRECTORY
	ACKNOWLEDGMENTS
	assets
	DESTINATION ".")
