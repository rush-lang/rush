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

   ast::builtin_void_type const void_type_inst = {};
   ast::builtin_error_type const undefined_inst = error_type("undefined");
   ast::builtin_error_type const undeclared_inst = error_type("undeclared identifier");
   ast::builtin_error_type const circular_ref_inst = error_type("circular reference detected");

   // sentinels
	ast::type_ref const void_type = { void_type_inst };
	ast::type_ref const undefined = { undefined_inst };
   ast::type_ref const undeclared = { undeclared_inst };
   ast::type_ref const circular_ref = { circular_ref_inst };
}
