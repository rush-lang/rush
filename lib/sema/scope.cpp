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

	const symbol_entry undefined_symbol_entry { "<undefined>", symbol_flag_t {} };
	const symbol_entry error_type_symbol_entry { "<error-type>", symbol_flag_t {} };

	std::size_t scope::depth() const noexcept {
		return !is_global() ? parent()->depth() + 1 : 0;
	}

	bool scope::is_descendent_of(scope const& sc) const noexcept {
		if (this->is_global()) return false;
		if (this->is_local_to(sc)) return true;
		return this->parent()->is_descendent_of(sc);
	}

	void scope::insert(symbol_entry s) {
		_symbols.insert(s);
	}

	symbol scope::lookup(std::string name) const {
		auto it = _symbols.find({ name });
		if (it != _symbols.end()) return to_symbol(*it);

		return parent() != nullptr
			? parent()->lookup(std::move(name))
			: to_symbol(undefined_symbol_entry);
	}

	symbol scope::lookup_local(std::string name) const {
		auto it = _symbols.find({ std::move(name) });
		if (it != _symbols.end()) return to_symbol(*it);

		return to_symbol(undefined_symbol_entry);
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