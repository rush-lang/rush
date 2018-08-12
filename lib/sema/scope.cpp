#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

#include <iostream>
using namespace rush::sema;

namespace rush {
	scope& ensure_global_scope() {
		static scope* const gs = new scope(nullptr);
		return *gs;
	}

	scope& global_scope = ensure_global_scope();


	std::size_t scope::depth() const noexcept {
		return !is_global() ? parent()->depth() + 1 : 0;
	}

	bool scope::is_descendent_of(scope const& sc) const noexcept {
		if (this->is_global()) return false;
		if (this->is_local_to(sc)) return true;
		return this->parent()->is_descendent_of(sc);
	}

	sema::symbol scope::insert(symbol_entry s) {
		// inserts or gets the entry s.
		auto result = _symbols.insert(s);
		return to_symbol(*result.first);
	}

	symbol scope::lookup(std::string name) const {
		auto it = _symbols.find({ name });
		if (it != _symbols.end()) return to_symbol(*it);

		return parent() != nullptr
			? parent()->lookup(std::move(name))
			: _undefined_symbol;
	}

	symbol scope::lookup_local(std::string name) const {
		auto it = _symbols.find({ std::move(name) });
		if (it != _symbols.end()) return to_symbol(*it);

		return _undefined_symbol;
	}

	scope& scope::push_block_scope() {
		_children.push_back(scope { this });
		return _children.back();
	}

	scope& scope::push_function_scope() {
		_children.push_back(scope { this });
		return _children.back();
	}

	scope& scope::push_class_scope() {
		_children.push_back(scope { this });
		return _children.back();
	}

	scope& scope::push_module_scope() {
		_children.push_back(scope { this });
		return _children.back();
	}

	std::size_t scope::hash_id_of(sema::symbol_entry const& s) const {
		return _symbols.hash_function()(s);
	}
} // rush