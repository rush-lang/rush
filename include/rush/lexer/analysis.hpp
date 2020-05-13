/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
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

      rush::source const& source() const noexcept {
         return _tokens.front().source();
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
