#include "rush/ast/types/builtin.hpp"
#include <memory>
#include <array>

namespace rush::ast::types {
   ast::builtin_error_type error_type() {
      return error_type("");
   }

   ast::builtin_error_type error_type(std::string msg) {
      return ast::builtin_error_type { std::move(msg) };
   }

   ast::builtin_error_type const undefined_inst = error_type("undefined");
   ast::builtin_void_type const void_type_inst = {};

   // sentinals
	ast::type_ref const undefined = { undefined_inst };
	ast::type_ref const void_type = { void_type_inst };
}
