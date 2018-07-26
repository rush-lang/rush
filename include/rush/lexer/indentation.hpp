#pragma once

#ifndef RUSH_LEXER_INDENTATION_HPP
#define RUSH_LEXER_INDENTATION_HPP


#include <cstddef>
#include <cstdint>

namespace rush {

	enum class indentation_style : std::uint8_t {
		unknown,
		spaces,
		tabs
	};

	class indentation final {
		indentation(std::size_t depth, indentation_style style)
			: _depth { depth }
			, _style { style } {}

	public:
		indentation()
			: _depth { 0 }
			, _style { indentation_style::unknown } {}

		std::size_t depth() const noexcept {
			return _depth;
		}

		indentation_style style() const noexcept {
			return _style;
		}

		std::size_t increment() noexcept {
			return ++_depth;
		}

		std::size_t decrement() noexcept {
			return --_depth;
		}

		static inline indentation spaces(std::size_t depth) noexcept {
			return { depth, indentation_style::spaces };
		}

		static inline indentation tabs(std::size_t depth) noexcept {
			return { depth, indentation_style::tabs };
		}

	private:
		std::size_t _depth;
		indentation_style _style;
	};
} // rush






#endif // RUSH_LEXER_INDENTATION_HPP
