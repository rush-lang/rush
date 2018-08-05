cmake_minimum_required(VERSION 3.11)
project(injen VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(injen
	src/main.cpp
)

target_compile_features(injen PUBLIC cxx_std_17)

target_include_directories(injen PRIVATE include)
target_link_libraries(injen PRIVATE inja cxxopts)


include(CMakeParseArguments)


function(rush_render_templates)
	set(oneValueOptions JSON)
	set(multiValueOptions DEPENDENTS)

	cmake_parse_arguments(ARG "" "${oneValueOptions}" "${multiValueOptions}" ${ARGN})

	# For some reason inja::Environment::load_json, and by extension
	# nlohmann json throws with absolute paths. So we build a relative one here.
	file(RELATIVE_PATH ARG_JSON ${CMAKE_CURRENT_BINARY_DIR} ${ARG_JSON})

	foreach(tpl_header ${ARG_UNPARSED_ARGUMENTS})
		file(RELATIVE_PATH rel_tpl_header ${CMAKE_CURRENT_BINARY_DIR} ${tpl_header})

		# Build the generated header path.
		string(REGEX REPLACE "^(.*)\.j2$" "\\1" gen_header ${tpl_header})
		file(RELATIVE_PATH rel_gen_header ${CMAKE_SOURCE_DIR} ${gen_header})
		string(CONCAT gen_header ${CMAKE_BINARY_DIR}/ ${rel_gen_header})
		list(APPEND generated_headers ${gen_header})

		# Build custom command to generate source file from template.
		get_filename_component(outdir ${gen_header} DIRECTORY)
		add_custom_command(
			COMMAND ${CMAKE_COMMAND} -E make_directory ${outdir}
			COMMAND injen ${rel_tpl_header} --json ${ARG_JSON}
			COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_SOURCE_DIR}/${rel_gen_header} ${gen_header}
			DEPENDS injen ${tpl_header}
			OUTPUT ${gen_header}
			COMMENT "Generating source file ${rel_gen_header}")
	endforeach()

	foreach(dep ${ARG_DEPENDENTS})
		add_custom_target(${dep}-generated DEPENDS ${generated_headers})
		add_dependencies(${dep} ${dep}-generated)
	endforeach()

endfunction()
