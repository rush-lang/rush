#include "rush/ast/types/utility.hpp"
#include "rush/ast/types/builtin.hpp"

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref, ast::type_ref) {
      return types::error_type;
   }
}
