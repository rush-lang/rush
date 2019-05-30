#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/utility.hpp"
#include "rush/ast/visitor.hpp"

using namespace rush;

class type_reducer {
public:
   type_reducer(ast::type_ref first)
      : _first { first } {}

   ast::type_ref operator ()(ast::type_ref second) const noexcept {
      if (_first == second)
         return _first;

      return irreducible_type;
   }

private:
   ast::type_ref _first;
   static const ast::builtin_error_type irreducible_type;
};

const ast::builtin_error_type type_reducer::irreducible_type =
   ast::types::error_type("could not reduce types.");

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref lhs, ast::type_ref rhs) {
      auto reducer = type_reducer { lhs };
      return reducer(rhs);
   }
}
