#pragma once

#ifndef RUSH_AST_EXPRESSION_HPP
#define RUSH_AST_EXPRESSION_HPP

#include <utility>

namespace rush {

	// Expressions are a tree-like structure with a possibly centralized pool of
	// memory, enabled via move semantics (?? possibly ??)
	class expression {
	public:
		virtual expression clone() const { return *this; };
	protected:
		expression* allocate(expression);
	};

	class binary_expression : public expression {
	public:
		binary_expression(binary_expression const&) = delete;
		binary_expression& operator = (binary_expression const&) = delete;

		binary_expression(expression left, expression right)
			: _left(this->allocate(std::move(left)))
			, _right(this->allocate(std::move(right))) {}

		binary_expression(binary_expression&& other)
			: _left(std::move(other._left))
			, _right(std::move(other._right)) {}

		binary_expression& operator = (binary_expression&& other) {
		}

		expression const& left() const noexcept { return *_left; }
		expression const& right() const noexcept { return *_right; }

		virtual expression clone() const override {
			return binary_expression { _left->clone(), _right->clone() };
		}

	private:
		expression* _left;
		expression* _right;
	};
}

#endif // RUSH_AST_EXPRESSION_HPP
