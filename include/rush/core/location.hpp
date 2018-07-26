#pragma once

#ifndef RUSH_CORE_LOCATION_HPP
#define RUSH_CORE_LOCATION_HPP

#include <cstddef>

namespace rush {
	using source_index_t = std::size_t;

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
			: location(0, 0) {}

		constexpr location(source_index_t line, source_index_t column) noexcept
			: _line(line), _column(column) {}

		bool is_undefined() const noexcept {
			static const location undef {};
			return *this == undef;
		}

	private:
		source_index_t _line;
		source_index_t _column;
	};


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
