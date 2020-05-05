#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_matcher.hpp"
#include "rush/ast/types/type_reducer.hpp"
#include "rush/ast/visitor.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

const ast::builtin_error_type irreducible_type =
   ast::types::error_type("could not reduce types.");

class type_reducer {
public:
   type_reducer(ast::type_ref first)
      : _first { first } {}

   ast::type_ref operator ()(ast::type_ref second) const noexcept {
      if (types::match(_first, second))
         return _first;

      return irreducible_type;
   }

private:
   ast::type_ref _first;
};

namespace rush::ast::types {
   ast::type_ref reduce(ast::type_ref lhs, ast::type_ref rhs) {
      auto reducer = type_reducer { lhs };
      return reducer(rhs);
   }
}
