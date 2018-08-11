#include "rush/sema/scope.hpp"
#include "rush/sema/symbol.hpp"

using namespace rush::sema;

namespace rush {
	scope init_global_scope();
	scope global_scope = init_global_scope();

	scope init_global_scope() {
		auto gs = scope { nullptr };
		gs.insert({ "<undefined>", 0 });
		// gs.insert(symbol { "int", builtin_symbol | type_symbol });
		// gs.insert(symbol { "uint", builtin_symbol | type_symbol });
		// gs.insert(symbol { "long", builtin_symbol | type_symbol });
		// gs.insert(symbol { "ulong", builtin_symbol | type_symbol });
		return std::move(gs);
	}

	std::size_t scope::depth() const noexcept {
		return !is_global() ? parent()->depth() + 1 : 0;
	}

	bool scope::is_descendent_of(scope const& sc) const noexcept {
		if (this->is_global()) return false;
		if (this->is_local_to(sc)) return true;
		return this->parent()->is_descendent_of(sc);
	}

	void scope::insert(symbol_entry s) {
		_symtable.emplace(symbol { *this, s.name(), s.flags() });
	}

	symbol const& scope::lookup(std::string name) const {
		auto it = _symtable.find(symbol { *this, name, 0 });
		if (it != _symtable.end())
			return *it;

		return parent() != nullptr
			? parent()->lookup(name)
			: get_undefined_symbol();
	}

	symbol const& scope::lookup_local(std::string name) const {
		auto it = _symtable.find(symbol { *this, std::move(name), 0 });
		if (it != _symtable.end())
			return *it;

		return get_undefined_symbol();
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

	std::size_t scope::hash_id_of(sema::symbol const& s) const {
		return _symtable.hash_function()(s);
	}
} // rush