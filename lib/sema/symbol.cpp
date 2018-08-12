#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {
	std::size_t symbol::id() const noexcept {
		return _scope->hash_id_of({ this->name(), 0 });
	}

	std::string symbol::name() const noexcept {
		return _entry->name();
	}

} // rush::sema