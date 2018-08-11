#pragma once

#ifndef RUSH_SEMA_SCOPE_HPP
#define RUSH_SEMA_SCOPE_HPP

#include "rush/core/iterator_range.hpp"
#include "rush/sema/symbol.hpp"
#include "rush/sema/symbol_iterator.hpp"
#include "rush/sema/attributes.hpp"

#include <string>
#include <vector>
#include <unordered_set>

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
		using symbol_iterator = detail::basic_symbol_iterator<std::unordered_set<sema::symbol>::iterator>;
		using const_symbol_iterator = detail::basic_symbol_iterator<std::unordered_set<sema::symbol>::const_iterator>;

		// fixme: should not allow move-construction outside of this class.
		// it's public now because the children vector requires it.
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

		scope& push_block_scope();
		scope& push_function_scope();
		scope& push_class_scope();
		scope& push_module_scope();

		void insert(sema::symbol);
		void insert_or_assign(sema::symbol);

		sema::symbol const& lookup(std::string name) const;
		sema::symbol const& lookup_local(std::string name) const;

		iterator_range<const_symbol_iterator> symbols() const;
		iterator_range<const_symbol_iterator> local_symbols() const;

	private:
		scope(scope* parent)
			: _parent(parent)
			, _children { }
			, _symtable { } {}

		scope* _parent;
		std::vector<scope> _children;
		std::unordered_set<sema::symbol> _symtable;
	};
} // rush

#endif // RUSH_SEMA_SCOPE_HPP
