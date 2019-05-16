#include "rush/sema/symbol.hpp"
#include "rush/sema/scope.hpp"

namespace rush::sema {

   const symbol symbol::undefined = {};

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
	inline bool has_attributes(symbol_attributes_t flags, Flag first, Args... rest) {
		return has_attributes(flags, first) && has_attributes(flags, rest...);
	}

	// sema::scope const& symbol::scope() const noexcept {
	// 	return *_scope;
	// }

	// symbol types
	symbol_kind symbol::kind() const noexcept {
      switch(_decl->kind()) {
         case ast::declaration_kind::constant: return symbol_kind::constant;
         case ast::declaration_kind::variable: return symbol_kind::variable;
         case ast::declaration_kind::function: return symbol_kind::function;
         case ast::declaration_kind::struct_: return symbol_kind::struct_;
         case ast::declaration_kind::class_: return symbol_kind::class_;
         case ast::declaration_kind::enum_: return symbol_kind::enum_;
         case ast::declaration_kind::concept: return symbol_kind::concept;
         case ast::declaration_kind::interface: return symbol_kind::interface;
         // case ast::declaration_kind::module: return symbol_kind::module;
         default: return symbol_kind::constant; // todo: fix
      }
	}

	// convenience members
	// bool symbol::is_type() const noexcept {
	// 	return !is_undefined()
	// 	&& has_attributes(_entry->flags(), symbol_kind::type);
	// }

	// bool symbol::is_function() const noexcept {
	// 	return !is_undefined()
	// 	&& has_attributes(_entry->flags(), symbol_kind::function);
	// }

	// bool symbol::is_constant() const noexcept {
	// 	return !is_undefined()
	// 	&& has_attributes(_entry->flags(), symbol_kind::constant);
	// }

	// bool symbol::is_variable() const noexcept {
	// 	return !is_undefined()
	// 	&& has_attributes(_entry->flags(), symbol_kind::variable);
	// }

	// bool symbol::is_undefined() const noexcept {
	// 	return has_attributes(_entry->flags(), symbol_kind::undefined);
	// }

	// access modifiers
	// access_modifier symbol::access_modifier() const noexcept {
	// 	return static_cast<enum access_modifier>((_entry->flags() >> 3) & 0x07);
	// }

	// convenience members
	// bool symbol::is_internal() const noexcept {
	// 	return has_attributes(_entry->flags(), access_modifier::internal);
	// }
	// bool symbol::is_exported() const noexcept {
	// 	return has_attributes(_entry->flags(), access_modifier::exported);
	// }
	// bool symbol::is_public() const noexcept {
	// 	return has_attributes(_entry->flags(), access_modifier::public_);
	// }
	// bool symbol::is_private() const noexcept {
	// 	return has_attributes(_entry->flags(), access_modifier::private_);
	// }
	// bool symbol::is_protected() const noexcept {
	// 	return has_attributes(_entry->flags(), access_modifier::protected_);
	// }

	// storage class specifiers
	// storage_class_specifier symbol::storage_duration() const noexcept {
	// 	return static_cast<enum storage_class_specifier>((_entry->flags() >> 6) & 0x03);
	// }

	// // convenience members
	// bool symbol::is_auto_storage_duration() const noexcept {
	// 	return has_attributes(_entry->flags(), storage_class_specifier::auto_);
	// }

	// bool symbol::is_static_storage_duration() const noexcept {
	// 	return has_attributes(_entry->flags(), storage_class_specifier::static_);
	// }

	// bool symbol::is_thread_local_storage_duration() const noexcept {
	// 	return has_attributes(_entry->flags(), storage_class_specifier::thread_local_);
	// }
} // rush::sema
