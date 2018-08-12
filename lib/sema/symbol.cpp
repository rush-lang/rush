#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {
	std::size_t symbol::id() const noexcept {
		return _scope->hash_of({ this->name() });
	}

	symbol symbol::type() const noexcept {
		auto symbol = _entry->type();
		return symbol._entry == nullptr
			? _scope->lookup("")
			: symbol;
	}

	std::string symbol::name() const noexcept {
		return _entry->name();
	}

	rush::scope const& symbol::scope() const noexcept {
		return *_scope;
	}

	bool symbol::has_access_modifier(access_modifier am) const noexcept {
		return has_attributes(_entry->flags(), am);
	}

	bool symbol::is_type() const noexcept {
		return is_defined()
		&& has_attributes(_entry->flags(), symbol_type::type);
	}

	bool symbol::is_function() const noexcept {
		return is_defined()
		&& has_attributes(_entry->flags(), symbol_type::function);
	}

	bool symbol::is_constant() const noexcept {
		return is_defined()
		&& has_attributes(_entry->flags(), symbol_type::constant);
	}

	bool symbol::is_variable() const noexcept {
		return is_defined()
		&& has_attributes(_entry->flags(), symbol_type::variable);
	}
} // rush::sema