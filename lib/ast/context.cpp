#include "rush/ast/context.hpp"
#include "rush/ast/types.hpp"

namespace rush::ast {
   ast::type_ref context::bool_type() {
      static const ast::builtin_bool_type bool_type {};
      return { bool_type };
   }

   ast::type_ref context::char_type() { return types::undefined; }

   ast::type_ref context::string_type() {
      static const builtin_string_type string_type {};
      return string_type;
   }

   ast::type_ref context::ieee32_type() { return floating_point_type(floating_point_kind::ieee32); }
   ast::type_ref context::ieee64_type() { return floating_point_type(floating_point_kind::ieee64); }

   ast::type_ref context::int8_type() { return signed_integral_type(integral_kind::byte); }
   ast::type_ref context::int16_type() { return signed_integral_type(integral_kind::word); }
   ast::type_ref context::int32_type() { return signed_integral_type(integral_kind::dword); }
   ast::type_ref context::int64_type() { return signed_integral_type(integral_kind::qword); }

   ast::type_ref context::uint8_type() { return unsigned_integral_type(integral_kind::byte); }
   ast::type_ref context::uint16_type() { return unsigned_integral_type(integral_kind::word); }
   ast::type_ref context::uint32_type() { return unsigned_integral_type(integral_kind::dword); }
   ast::type_ref context::uint64_type() { return unsigned_integral_type(integral_kind::qword); }

   ast::type_ref context::signed_integral_type(integral_kind kind) {
      static const builtin_integral_type int8_type { integral_kind::byte, true };
      static const builtin_integral_type int16_type { integral_kind::word, true };
      static const builtin_integral_type int32_type { integral_kind::dword, true };
      static const builtin_integral_type int64_type { integral_kind::qword, true };

      switch (kind) {
      case integral_kind::byte: return int8_type;
      case integral_kind::word: return int16_type;
      case integral_kind::dword: return int32_type;
      case integral_kind::qword: return int64_type;
      }
   }

   ast::type_ref context::unsigned_integral_type(integral_kind kind) {
      static const builtin_integral_type uint8_type { integral_kind::byte, false };
      static const builtin_integral_type uint16_type { integral_kind::word, false };
      static const builtin_integral_type uint32_type { integral_kind::dword, false };
      static const builtin_integral_type uint64_type { integral_kind::qword, false };

      switch (kind) {
      case integral_kind::byte: return uint8_type;
      case integral_kind::word: return uint16_type;
      case integral_kind::dword: return uint32_type;
      case integral_kind::qword: return uint64_type;
      }
   }

   ast::type_ref context::floating_point_type(floating_point_kind kind) {
      static const builtin_floating_point_type ieee16_type { floating_point_kind::ieee16 };
      static const builtin_floating_point_type ieee32_type { floating_point_kind::ieee32 };
      static const builtin_floating_point_type ieee64_type { floating_point_kind::ieee64 };
      static const builtin_floating_point_type ieee80_type { floating_point_kind::ieee80 };
      static const builtin_floating_point_type ieee128_type { floating_point_kind::ieee128 };
      static const builtin_floating_point_type ppc128_type { floating_point_kind::ppc128 };

      switch (kind) {
      case floating_point_kind::ieee16: return ieee16_type;
      case floating_point_kind::ieee32: return ieee32_type;
      case floating_point_kind::ieee64: return ieee64_type;
      case floating_point_kind::ieee80: return ieee80_type;
      case floating_point_kind::ieee128: return ieee128_type;
      case floating_point_kind::ppc128: return ppc128_type;
      }
   }

   ast::type_ref context::array_type(ast::type_ref, size_type) {
      return types::undefined;
   }

   ast::type_ref context::tuple_type(iterator_range<ast::type_ref>) {
      return types::undefined;
   }

   ast::type_ref context::function_type(ast::type_ref ret, iterator_range<ast::type_ref> params) {
      return types::undefined;
   }
}
