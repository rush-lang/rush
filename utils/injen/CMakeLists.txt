cmake_minimum_required(VERSION 3.11)
project(injen VERSION ${RUSH_VERSION} LANGUAGES CXX)

add_executable(injen
	src/main.cpp
)

target_include_directories(injen PRIVATE include)
target_link_libraries(injen PRIVATE inja)


include(CMakeParseArguments)

function(rush_render_templates)
	set(oneValueOptions
		JSON
		OUTDIR)
	set(multiValueOptions
		DEPENDENTS)

	cmake_parse_arguments(ARG "" "${oneValueOptions}" "${multiValueOptions}" ${ARGN})

	# For some reason inja::Environment::load_json, and by extension
	# nlohmann json throws with absolute paths. So we build a relative one here.
	file(RELATIVE_PATH ARG_JSON ${CMAKE_CURRENT_BINARY_DIR} ${ARG_JSON})
	file(RELATIVE_PATH ARG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR} ${ARG_OUTDIR})

	foreach(tpl_header ${ARG_UNPARSED_ARGUMENTS})
		string(REGEX REPLACE "^(.*\.hpp)\.j2$" "\\1" gen_header ${tpl_header})
		file(RELATIVE_PATH rel_tpl_header ${CMAKE_CURRENT_BINARY_DIR} ${tpl_header})
		file(RELATIVE_PATH rel_gen_header ${CMAKE_SOURCE_DIR} ${gen_header})
		list(APPEND rel_template_headers ${rel_tpl_header})
		list(APPEND template_headers ${tpl_header})
		list(APPEND generated_headers ${gen_header})

		add_custom_command(
			COMMAND mkdir --parents ${ARG_OUTDIR}
			COMMAND injen ${rel_tpl_header} --json ${ARG_JSON}
			COMMAND mv ${gen_header} --target-directory=${ARG_OUTDIR}
			DEPENDS injen ${tpl_header}
			OUTPUT ${gen_header}
			COMMENT "Generating source file ${rel_gen_header}")
	endforeach()

	foreach(dep ${ARG_DEPENDENTS})
		add_custom_target(${dep}-generated DEPENDS ${generated_headers})
		add_dependencies(${dep} ${dep}-generated)
	endforeach()

endfunction()
