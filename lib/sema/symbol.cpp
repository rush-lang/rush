#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {
	std::size_t symbol::id() const noexcept {
		return _scope->hash_id_of(*this);
	}
} // rush::sema