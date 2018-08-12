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

	// symbol types
	symbol_type symbol::symbol_type() const noexcept {
		return symbol_type::undefined;
	}

	// convenience members
	bool symbol::is_type() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_type::type);
	}

	bool symbol::is_function() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_type::function);
	}

	bool symbol::is_constant() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_type::constant);
	}

	bool symbol::is_variable() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_type::variable);
	}

	bool symbol::is_undefined() const noexcept {
		return has_attributes(_entry->flags(), symbol_type::undefined);
	}

	// access modifiers
	access_modifier symbol::access_modifier() const noexcept {
		return access_modifier::internal;
	}

	// convenience members
	bool symbol::is_internal() const noexcept {
		return has_attributes(_entry->flags(), access_modifier::internal);
	}
	bool symbol::is_exported() const noexcept {
		return has_attributes(_entry->flags(), access_modifier::exported);
	}
	bool symbol::is_public() const noexcept {
		return has_attributes(_entry->flags(), access_modifier::public_);
	}
	bool symbol::is_private() const noexcept {
		return has_attributes(_entry->flags(), access_modifier::private_);
	}
	bool symbol::is_protected() const noexcept {
		return has_attributes(_entry->flags(), access_modifier::protected_);
	}

	// storage class specifiers
	storage_class_specifier symbol::storage_duration() const noexcept {
		return storage_class_specifier::auto_;
	}

	// convenience members
	bool symbol::is_auto_storage_duration() const noexcept {
		return has_attributes(_entry->flags(), storage_class_specifier::auto_);
	}

	bool symbol::is_static_storage_duration() const noexcept {
		return has_attributes(_entry->flags(), storage_class_specifier::static_);
	}

	bool symbol::is_thread_local_storage_duration() const noexcept {
		return has_attributes(_entry->flags(), storage_class_specifier::thread_local_);
	}
} // rush::sema