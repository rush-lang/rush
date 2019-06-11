#pragma once

#ifndef RUSH_LEXER_ANALYSIS_HPP
#define RUSH_LEXER_ANALYSIS_HPP

#include "rush/lexer/token.hpp"
#include "rush/core/source.hpp"

#include <string>
#include <string_view>
#include <iosfwd>
#include <vector>


namespace rush {

	class lexer;
	class lexer_options;

	class lexical_analysis final {
		friend class lexer;

	public:
		lexical_analysis(lexical_analysis const&) = delete;
		void operator = (lexical_analysis const&) = delete;

		lexical_analysis(lexical_analysis&&);
		void operator = (lexical_analysis&&);

      using reference = typename std::vector<lexical_token>::reference;
      using const_reference = typename std::vector<lexical_token>::const_reference;

		using iterator = typename std::vector<lexical_token>::iterator;
		using const_iterator = typename std::vector<lexical_token>::const_iterator;

      std::string_view id() const noexcept {
         return _id;
      }

		bool empty() const noexcept {
			return _tokens.empty();
		}

		std::size_t size() const noexcept {
			return _tokens.size();
		}

      reference front() noexcept {
         return _tokens.front();
      }

      const_reference front() const noexcept {
         return _tokens.front();
      }

      reference back() noexcept {
         return _tokens.back();
      }

      const_reference back() const noexcept {
         return _tokens.back();
      }

		const_iterator begin() const noexcept {
			return _tokens.begin();
		}

		const_iterator end() const noexcept {
			return _tokens.end();
		}

		const_iterator cbegin() const noexcept {
			return _tokens.cbegin();
		}

		const_iterator cend() const noexcept {
			return _tokens.cend();
		}

	private:
      std::string _id;
		std::vector<lexical_token> _tokens;

      explicit lexical_analysis(
         std::vector<lexical_token> toks)
         : _tokens(std::move(toks)) {}

		explicit lexical_analysis(
         std::string id,
         std::vector<lexical_token> toks)
			: _id { std::move(id) }
         , _tokens(std::move(toks)) {}
	};
} // rush

#endif // RUSH_LEXER_ANALYSIS_HPP
