#pragma once

#ifndef RUSH_LEXER_DUMP_HPP
#define RUSH_LEXER_DUMP_HPP

#include <iosfwd>

namespace rush {
	class lexical_analysis;
	void dump(lexical_analysis const& input);
	void dump(lexical_analysis const& input, std::ostream& out);
} // rush

#endif // RUSH_LEXER_DUMP_HPP
