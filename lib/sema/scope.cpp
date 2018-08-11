#include "rush/sema/scope.hpp"
#include "rush/sema/symbol.hpp"

namespace rush {
	symbol const undefined_symbol;

	scope init_global_scope();
	scope global_scope = init_global_scope();

	scope init_global_scope() {
		auto gs = scope { nullptr };
		// ...
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
		_symtable.emplace(s.name(), s);
	}

	symbol const& scope::lookup(std::string name) const {
		auto it = _symtable.find(name);
		if (it != _symtable.end())
			return it->second;

		return parent() != nullptr
			? parent()->lookup(name)
			: undefined_symbol;
	}
} // rush