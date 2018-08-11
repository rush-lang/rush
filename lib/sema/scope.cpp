#include "rush/sema/scope.hpp"
#include "rush/sema/symbol.hpp"

using namespace rush::sema;

namespace rush {
	symbol const undefined_symbol { "<undefined>" };

	scope init_global_scope();
	scope global_scope = init_global_scope();

	scope init_global_scope() {
		auto gs = scope { nullptr };
		gs.insert(undefined_symbol);
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

	void scope::insert(symbol s) {
		_symtable.emplace(s);
	}

	symbol const& scope::lookup(std::string name) const {
		auto it = _symtable.find(symbol { name });
		if (it != _symtable.end())
			return *it;

		return parent() != nullptr
			? parent()->lookup(name)
			: undefined_symbol;
	}

	symbol const& scope::lookup_local(std::string name) const {
		auto it = _symtable.find(symbol { std::move(name) });
		if (it != _symtable.end())
			return *it;

		return undefined_symbol;
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
} // rush