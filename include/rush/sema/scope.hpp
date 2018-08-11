#pragma once

#ifndef RUSH_SEMA_SCOPE_HPP
#define RUSH_SEMA_SCOPE_HPP

#include "rush/sema/symbol.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace rush {
	class scope;
	extern scope global_scope;

	class scope {
		friend scope init_global_scope();
		friend scope& define_scope(scope& parent);

		scope(scope const&) = delete;
		void operator = (scope const&) = delete;

	public:
		scope(scope&& other)
			: _parent(other._parent)
			, _children(std::move(other._children))
			, _symtable(std::move(other._symtable)) {}

		scope& operator = (scope&& other) {
			_parent = other._parent;
			_children = std::move(other._children);
			_symtable = std::move(other._symtable);
			return *this;
		}

		scope* parent() const noexcept {
			return _parent;
		}

		std::size_t depth() const noexcept {
			return parent() != nullptr
				? parent()->depth() + 1
				: 0;
		}

		void insert(std::string name, symbol);
		symbol const* lookup(std::string name) const;

	private:
		scope(scope* parent)
			: _parent(parent)
			, _children { }
			, _symtable { } {}

		scope* _parent;
		std::vector<scope> _children;
		std::unordered_map<std::string, symbol> _symtable;
	};

	inline scope& define_scope(scope& parent = global_scope) {
		parent._children.push_back(scope { &parent });
		return parent._children.back();
	}
} // rush

#endif // RUSH_SEMA_SCOPE_HPP
