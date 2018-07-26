#pragma once

#ifndef RUSH_LEXER_OPTIONS_HPP
#define RUSH_LEXER_OPTIONS_HPP

namespace rush {
	struct lexer_options {
		bool artificial_eof;

		lexer_options()
			: artificial_eof { true } {}
	};
} // rush

#endif // RUSH_LEXER_OPTIONS_HPP
