#pragma once

#ifndef RUSH_LEXER_LEXER_ITERATOR_HPP
#define RUSH_LEXER_LEXER_ITERATOR_HPP

#include "rush/core/charinfo.hpp"
#include "rush/core/location.hpp"

#include <iterator>

using namespace rush;
using namespace charinfo;

template <typename FwdIter>
class lexer_iterator {
public:
	// restrict_iterator_category_t<FwdIter, std::forward_iterator_tag>,
	using iterator_category = std::forward_iterator_tag;
	using value_type = typename std::iterator_traits<FwdIter>::value_type;
	using pointer = typename std::iterator_traits<FwdIter>::pointer;
	using reference = typename std::iterator_traits<FwdIter>::reference;
	using difference_type = typename std::iterator_traits<FwdIter>::difference_type;

	lexer_iterator()
		: _iter { nullptr }
		, _last { nullptr }
		, _loc {} {}

	explicit lexer_iterator(FwdIter& it, FwdIter const& last, location loc = {})
		: _iter { std::addressof(it) }
		, _last { std::addressof(last) }
		, _loc { loc } {}

	location location() const noexcept {
		return _loc;
	}

	reference operator *() const {
		return **_iter;
	}

	pointer operator ->() const {
		return std::addressof(*_iter);
	}

	lexer_iterator& operator ++() {
		_increment();
		return *this;
	}

	lexer_iterator const operator ++(int) {
		auto temp = *this;
		_increment();
		return temp;
	}

	friend bool operator == (
		lexer_iterator const& lhs,
		lexer_iterator const& rhs) noexcept {
		return *lhs._iter == *rhs._iter;
	}

	friend bool operator != (
		lexer_iterator const& lhs,
		lexer_iterator const& rhs) noexcept {
		return !(lhs == rhs);
	}

private:
	FwdIter* _iter;
	FwdIter const* _last;
	struct location _loc;

	void _increment() {
		assert(*_iter != *_last && "lexer_iterator passed end of range");
		++(*_iter);
		_loc = is_newline(**_iter)
			? _loc.next_line()
			: _loc.next_column();
	}
};

#endif // RUSH_LEXER_LEXER_ITERATOR_HPP
