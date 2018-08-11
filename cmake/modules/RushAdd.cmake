
include(CMakeParseArguments)

function(rush_add_library target)
	add_library(${target} ${ARGN})
	set_target_properties(${target}
		PROPERTIES
			PREFIX "librush-")
endfunction()

function(rush_add_executable target)
endfunction()