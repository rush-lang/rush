#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {

	const symbol_entry symbol::undefined_entry = make_undefined_entry("<undefined>");

	inline bool has_attributes(symbol_attributes_t) {
		return true;
	}

	inline bool has_attributes(symbol_attributes_t flags, symbol_kind st) {
		auto val = static_cast<symbol_attributes_t>(st);
		return (flags & 0x07) == val;
	}

	inline bool has_attributes(symbol_attributes_t flags, access_modifier am) {
		auto val = static_cast<symbol_attributes_t>(am);
		return ((flags >> 3) & 0x07) == val;
	}

	inline bool has_attributes(symbol_attributes_t flags, storage_class_specifier scs) {
		auto val = static_cast<symbol_attributes_t>(scs);
		return ((flags >> 6) & 0x03) == val;
	}

	template <typename Flag, typename... Args>
	bool has_attributes(symbol_attributes_t flags, Flag first, Args... rest) {
		return has_attributes(flags, first) && has_attributes(flags, rest...);
	}

	std::size_t symbol::id() const noexcept {
		return _scope->hash_of({ this->name() });
	}

	std::string symbol::name() const noexcept {
		return _entry->name();
	}

	symbol symbol::type() const noexcept {
		return _entry->type();
	}

	rush::scope const& symbol::scope() const noexcept {
		return *_scope;
	}

	// symbol types
	symbol_kind symbol::symbol_kind() const noexcept {
		return symbol_kind::undefined;
	}

	// convenience members
	bool symbol::is_type() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_kind::type);
	}

	bool symbol::is_function() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_kind::function);
	}

	bool symbol::is_constant() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_kind::constant);
	}

	bool symbol::is_variable() const noexcept {
		return !is_undefined()
		&& has_attributes(_entry->flags(), symbol_kind::variable);
	}

	bool symbol::is_undefined() const noexcept {
		return has_attributes(_entry->flags(), symbol_kind::undefined);
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