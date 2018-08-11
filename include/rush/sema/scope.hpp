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

	class scope final {
		friend scope init_global_scope();

		scope(scope const&) = delete;
		void operator = (scope const&) = delete;

		scope& operator = (scope&& other) {
			_parent = other._parent;
			_children = std::move(other._children);
			_symtable = std::move(other._symtable);
			return *this;
		}

	public:
		scope(scope&& other)
			: _parent(other._parent)
			, _children(std::move(other._children))
			, _symtable(std::move(other._symtable)) {}

		bool is_global() const noexcept {
			return _parent == nullptr;
		}

		bool is_local_to(scope const& parent) const noexcept {
			return _parent == &parent;
		}

		scope* const parent() const noexcept {
			return _parent;
		}

		bool is_descendent_of(scope const& parent) const noexcept;

		std::size_t depth() const noexcept;

		void insert(symbol);
		symbol const& lookup(std::string name) const;

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
