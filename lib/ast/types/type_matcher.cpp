#include "rush/ast/types/type_matcher.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/visitor.hpp"

namespace ast = rush::ast;
namespace types = rush::ast::types;

class type_matcher : public ast::visitor {
public:
   type_matcher(ast::type_ref type)
      : _type { std::move(type) }
      , _result { false } {}

   bool result() const noexcept {
      return _result;
   }

   virtual void visit_builtin_integral_type(ast::builtin_integral_type const& second) override {
      auto first = _type.as<ast::builtin_integral_type>();
      if (!first) return;
      _result =
         first->unit() == second.unit() &&
         first->size() == second.size() &&
         first->bit_width() == second.bit_width() &&
         first->is_signed() == second.is_signed();
   }

private:
   ast::type_ref _type;
   bool _result;
};

namespace rush::ast::types {
   bool match(ast::type_ref lhs, ast::type_ref rhs) {
      return (lhs == rhs) || rush::visit(lhs, type_matcher { rhs }).result();
   }
}
