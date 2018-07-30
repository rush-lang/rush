#pragma once

#ifndef RUSH_CORE_LOCATION_HPP
#define RUSH_CORE_LOCATION_HPP

#include <cstddef>
#include <sstream>
#include <string>

namespace rush {
	using source_index_t = std::size_t;
	using source_difference_t = std::size_t;

	class location;

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
		location next_line(source_difference_t n = 1) {
			return { _line + n, 1 };
		}

		// \brief Returns a copy of this location with column increment by n.
		location next_column(source_difference_t n = 1) {
			return { _line, _column + n };
		}

	private:
		source_index_t _line;
		source_index_t _column;
	};


	inline std::string to_string(location const& loc) {
		std::ostringstream oss;
		if (loc == location::undefined) {
			oss << "[ln: undefined, col: undefined]";
		}
		else {
			oss << "[ln: " << loc.line()
				<< ", col: " << loc.column() << "]";
		}

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
