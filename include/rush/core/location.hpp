#pragma once

#ifndef RUSH_CORE_LOCATION_HPP
#define RUSH_CORE_LOCATION_HPP

#include <cstddef>

namespace rush {
	using source_index_t = std::size_t;

	struct location {
		static const location undefined;

		source_index_t line() const noexcept { return _line; }
		source_index_t column() const noexcept { return _column; }

		location()
			: location(0, 0) {}

		location(source_index_t line, source_index_t column)
			: _line(line), _column(column) {}

	private:
		source_index_t _line;
		source_index_t _column;
	};


	inline bool operator < (location const& lhs, location const& rhs) {
		return lhs.line() < rhs.line() || (lhs.line() == rhs.line() && lhs.column() < rhs.column());
	}

	inline bool operator > (location const& lhs, location const& rhs) {
		return (rhs < lhs);
	}

	inline bool operator <= (location const& lhs, location const& rhs) {
		return !(rhs < lhs);
	}

	inline bool operator >= (location const& lhs, location const& rhs) {
		return !(lhs < rhs);
	}


	inline bool operator == (location const& lhs, location const& rhs) {
		return lhs.line() == rhs.line() && lhs.column() == rhs.column();
	}

	inline bool operator != (location const& lhs, location const& rhs) {
		return !(lhs == rhs);
	}
}

#endif // RUSH_CORE_LOCATION_HPP
