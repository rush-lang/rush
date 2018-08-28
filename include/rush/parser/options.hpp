#pragma once

#ifndef RUSH_PARSER_OPTIONS_HPP
#define RUSH_PARSER_OPTIONS_HPP

#include "rush/sema/scope.hpp"

namespace rush {
	struct parser_options {
		rush::scope* scope;

		parser_options()
			: scope(&global_scope) {}
	};
}

#endif // RUSH_PARSER_OPTIONS_HPP
