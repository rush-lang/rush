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

#ifndef RUSH_CORE_LOCATION_HPP
#define RUSH_CORE_LOCATION_HPP

#include <cstddef>
#include <sstream>
#include <string>

namespace rush {
	using source_index_t = std::size_t;
	using source_difference_t = std::size_t;

	struct location;

	inline bool operator < (location const& lhs, location const& rhs) noexcept;
	inline bool operator > (location const& lhs, location const& rhs) noexcept;
	inline bool operator <= (location const& lhs, location const& rhs) noexcept;
	inline bool operator >= (location const& lhs, location const& rhs) noexcept;
	inline bool operator == (location const& lhs, location const& rhs) noexcept;
	inline bool operator != (location const& lhs, location const& rhs) noexcept;

	struct location {
		static const location undefined;

		source_index_t line() const noexcept { return _line; }
		source_index_t column() const noexcept { return _column; }

		constexpr location() noexcept
			: location(1, 1) {}

		constexpr location(source_index_t line, source_index_t column) noexcept
			: _line(line), _column(column) {}

		bool is_undefined() const noexcept {
			static const location undef { 0, 0 };
			return *this == undef;
		}

		// \brief Returns a copy of this location with line incremented by n, and the column reset to 1
		location next_line(source_difference_t n = 1) const noexcept {
			return { _line + n, 1 };
		}

		// \brief Returns a copy of this location with column increment by n.
		location next_column(source_difference_t n = 1) const noexcept {
			return { _line, _column + n };
		}

	private:
		source_index_t _line;
		source_index_t _column;
	};

   class location_range {
   public:
      location_range(
         rush::location start,
         rush::location end)
         : _start { start }
         , _end { end } {}

      bool is_undefined() const noexcept {
         return _start.is_undefined()
             || _end.is_undefined();
      }

      rush::location const& start() const noexcept {
         return _start;
      }

      rush::location const& end() const noexcept {
         return _end;
      }

   private:
      rush::location _start;
      rush::location _end;
   };

   template <typename CharT, typename TraitsT>
   std::basic_ostream<CharT, TraitsT>& operator << (std::basic_ostream<CharT, TraitsT>& os, location loc) {
      if (loc == location::undefined) { os << "[line: undefined, col: undefined]"; }
		else { os << "[line: " << loc.line() << ", col: " << loc.column() << "]"; }
      return os;
   }

	inline std::string to_string(location const& loc) {
		std::ostringstream oss;
      oss << loc;
		return oss.str();
	}

	inline bool operator == (location const& lhs, location const& rhs) noexcept {
		return lhs.line() == rhs.line() && lhs.column() == rhs.column();
	}

	inline bool operator != (location const& lhs, location const& rhs) noexcept {
		return !(lhs == rhs);
	}


	inline bool operator < (location const& lhs, location const& rhs) noexcept {
		return lhs.line() < rhs.line() || (lhs.line() == rhs.line() && lhs.column() < rhs.column());
	}

	inline bool operator > (location const& lhs, location const& rhs) noexcept {
		return (rhs < lhs);
	}

	inline bool operator <= (location const& lhs, location const& rhs) noexcept {
		return !(rhs < lhs);
	}

	inline bool operator >= (location const& lhs, location const& rhs) noexcept {
		return !(lhs < rhs);
	}
}

#endif // RUSH_CORE_LOCATION_HPP
