#pragma once

#ifndef RUSH_SEMA_SCOPE_HPP
#define RUSH_SEMA_SCOPE_HPP

#include "rush/core/iterator_range.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace rush {
	class scope {

		using scope_container_type = std::vector<scope>;
		using symbol_container_type = std::unordered_map<std::string, int>;

		scope(scope const&) = delete;
		void operator = (scope const&) = delete;

	public:
		scope() = default;

		using scope_iterator_range = rush::iterator_range<
			typename scope_container_type::iterator>;

		using symbol_iterator_range = rush::iterator_range<
			typename symbol_container_type::iterator>;

		scope(scope&& sc)
			: _children(std::move(sc._children))
			, _symbols(std::move(sc._symbols))
			, _parent(sc._parent) {}

		scope& operator = (scope&& sc) {
			_children = std::move(sc._children);
			_symbols = std::move(sc._symbols);
			_parent = sc._parent;
			return *this;
		}

		scope* parent() const noexcept;

		bool is_local() const noexcept;
		bool is_global() const noexcept;

		std::size_t depth() const noexcept;

		void merge(scope&);
		void attach(scope&);
		void detach();

	private:
		std::unordered_map<std::string, int> _symbols;
		std::vector<scope> _children;
		scope* _parent;
	};
} // rush

#endif // RUSH_SEMA_SCOPE_HPP
