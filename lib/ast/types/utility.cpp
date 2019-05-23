#include "rush/ast/types/utility.hpp"
#include "rush/ast/types/builtin.hpp"

#include "reducer.hpp"

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref lhs, ast::type_ref rhs) {
      auto reducer = type_reducer { lhs };
      return reducer(rhs);
   }
}
