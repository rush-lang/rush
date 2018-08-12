#pragma once

#ifndef RUSH_SEMA_SCOPE_HPP
#define RUSH_SEMA_SCOPE_HPP

#include "rush/sema/global_scope.hpp"
#include "rush/core/iterator_range.hpp"
#include "rush/sema/symbol.hpp"
#include "rush/sema/symbol_entry.hpp"
#include "rush/sema/symbol_iterator.hpp"
#include "rush/sema/attributes.hpp"

#include <string>
#include <vector>
#include <unordered_set>

namespace rush {
	class scope final {
		friend scope& ensure_global_scope();

		scope(scope const&) = delete;
		void operator = (scope const&) = delete;

		scope& operator = (scope&& other) {
			_parent = other._parent;
			_children = std::move(other._children);
			_symbols = std::move(other._symbols);
			return *this;
		}

	public:
		using symbol_iterator = detail::basic_symbol_iterator<std::unordered_set<sema::symbol_entry>::iterator>;
		using const_symbol_iterator = detail::basic_symbol_iterator<std::unordered_set<sema::symbol_entry>::const_iterator>;

		// fixme: should not allow move-construction outside of this class.
		// it's public now because the children vector requires it.
		scope(scope&& other)
			: _parent(other._parent)
			, _children(std::move(other._children))
			, _symbols(std::move(other._symbols)) {}

		std::size_t depth() const noexcept {
			return !is_global() ? parent()->depth() + 1 : 0;
		}

		scope* const parent() const noexcept {
			return _parent;
		}

		bool is_global() const noexcept {
			return _parent == nullptr;
		}

		bool is_local_to(scope const& parent) const noexcept {
			return _parent == &parent;
		}

		bool is_descendent_of(scope const& parent) const noexcept;

		scope& push_scope();

		scope& push_block_scope();
		scope& push_function_scope();
		scope& push_class_scope();
		scope& push_module_scope();

		sema::symbol insert(sema::symbol_entry);
		sema::symbol insert_or_assign(sema::symbol_entry);

		// sema::symbol update(sema::symbol_entry);

		sema::symbol lookup(std::string name);
		sema::symbol lookup_local(std::string name);

		iterator_range<const_symbol_iterator> symbols() const;
		iterator_range<const_symbol_iterator> local_symbols() const;

		std::size_t hash_of(sema::symbol_entry const&) const;

		sema::symbol undefined_symbol() const noexcept {
			return _undefined_symbol;
		}

	private:
		sema::symbol _undefined_symbol;

		scope* _parent;
		std::vector<scope> _children;
		std::unordered_set<sema::symbol_entry> _symbols;

		scope(scope* parent)
			: _parent(parent)
			, _children { }
			, _symbols { } { initialize(); }

		void initialize() {
			static const auto undefined_entry_name = "<undefined>";
			_undefined_symbol = this->insert({ undefined_entry_name });
		}

		sema::symbol to_symbol(sema::symbol_entry const& entry) const noexcept {
			return { *this, entry };
		}
	};

	inline sema::symbol lookup(scope& s, std::string name) {
		return s.lookup(std::move(name));
	}

	inline sema::symbol lookup(std::string name) {
		return lookup(ensure_global_scope(), std::move(name));
	}
} // rush

#endif // RUSH_SEMA_SCOPE_HPP
