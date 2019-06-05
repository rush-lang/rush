#pragma once

#ifndef RUSH_LEXER_LEXER_HPP
#define RUSH_LEXER_LEXER_HPP

#include "rush/lexer/lex.hpp"
#include "rush/lexer/analysis.hpp"

#include <string_view>
#include <iosfwd>

namespace rush {
   class source;

	template <typename InIter>
	class lexer_iterator;

	class lexer {
		friend class lexical_iterator;

		lexer(lexer const&) = delete;
		void operator = (lexer const&) = delete;

	public:
		explicit lexer(lexer_options opts = {}) noexcept
         : _opts { std::move(opts) } {}

		lexer(lexer&& other) = default;
		lexer& operator = (lexer&& other) = default;

      lexical_analysis tokenize(std::istream&);
		lexical_analysis tokenize(std::string_view);

   private:
      lexer_options _opts;
	};
};

#endif // RUSH_LEXER_LEXER_HPP
