#include "rush/ast/types/function.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/types/builtin.hpp"

#include "rush/parser/dump.hpp"

#include <optional>
#include <numeric>

using namespace rush;

class function_return_type_resolver : public ast::traversal {
   friend class rush::ast::function_type;

public:
   function_return_type_resolver()
      : _last_result { ast::types::void_type } {}

   virtual void visit_constant_decl(ast::constant_declaration const&) override { /*ignore*/ }
	virtual void visit_variable_decl(ast::variable_declaration const&) override { /*ignore*/ }

   virtual void visit_unary_expr(ast::unary_expression const& expr) override { /*ignore*/ }
   virtual void visit_binary_expr(ast::binary_expression const& expr) override { /*ignore*/ }

   virtual void visit_conditional_stmt(ast::conditional_statement const& stmt) override {
      // ignore statement condition as it cannot result in a return statement.
      stmt.body().accept(*this);
   }

   virtual void visit_for_stmt(ast::iteration_statement const& stmt) override {
      // ignore parts of the for statement
      // todo: implement when for statement is implemented
   }

   virtual void visit_switch_stmt(ast::switch_statement const& stmt) override {
      // ignore parts of the switch statement
      // todo: implement when switch statement is implemented
   }

   virtual void visit_return_stmt(ast::result_statement const& stmt) override {
      _last_result = stmt.result_type();
      if (_last_result != infinite_recursion_error_type)
         _results.push_back(stmt.result_type());
   }

   ast::type_ref result() const noexcept {
      if (_results.empty())
         return _last_result;

      return std::accumulate(
         std::next(_results.begin()),
         _results.end(),
         _results.front(),
         ast::types::reduce);
   }

private:
   static const ast::builtin_error_type infinite_recursion_error_type;
   std::vector<ast::type_ref> _results;
   ast::type_ref _last_result;
};

const ast::builtin_error_type function_return_type_resolver::infinite_recursion_error_type =
      ast::types::error_type("deducing return type is infinitely recursive.");

namespace rush::ast {
   void function_type::resolve_return_type(ast::statement const& stmt) const {
      /// check if the expression resulted in infinite recursion
      /// via counting the number times this function has been called
      /// without resolving.
      if (_resolve_iter > 0) _return_type = function_return_type_resolver::infinite_recursion_error_type;
      else if (_return_type == types::undefined || _return_type.kind() == type_kind::builtin_error) {
         ++_resolve_iter;
         auto v = function_return_type_resolver {};
         stmt.accept(v);
         _return_type = v.result();
         --_resolve_iter;
      }
   }
}
