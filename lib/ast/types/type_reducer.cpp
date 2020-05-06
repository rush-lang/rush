#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_matcher.hpp"
#include "rush/ast/types/type_reducer.hpp"
#include "rush/ast/visitor.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

class type_reducer {
public:
   type_reducer(ast::type_ref first)
      : _first { first } {}

   ast::type_ref operator ()(ast::type_ref second) const noexcept {
      if (_first.kind() == ast::type_kind::error
       || second.kind() == ast::type_kind::error)
         return ast::types::irreducible;

      if (types::match(_first, second))
         return _first;

      return ast::types::irreducible;
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
