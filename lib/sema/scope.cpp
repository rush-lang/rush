#include "rush/sema/scope.hpp"

namespace rush {
	scope init_global_scope() {
		auto gs = scope { nullptr };
		// ...
		return std::move(gs);
	}

	scope global_scope = init_global_scope();

	symbol const* scope::lookup(std::string name) const {
		auto it = _symtable.find(name);
		if (it != _symtable.end())
			return &it->second;

		return parent() != nullptr
			? parent()->lookup(name)
			: nullptr;
	}
} // rush