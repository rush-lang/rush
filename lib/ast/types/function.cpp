#include "rush/ast/types/function.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/types/builtin.hpp"

#include "rush/parser/dump.hpp"

#include <optional>
#include <iterator>

using namespace rush;

class function_return_type_resolver : public ast::traversal {
public:
   virtual void visit_constant_decl(ast::constant_declaration const&) override { /*ignore*/ }
	virtual void visit_variable_decl(ast::variable_declaration const&) override { /*ignore*/ }

   virtual void visit_unary_expr(ast::unary_expression const& expr) override { /*ignore*/ }
   virtual void visit_binary_expr(ast::binary_expression const& expr) override { /*ignore*/ }

   virtual void visit_if_stmt(ast::if_statement const& stmt) override {
      // ignore statement condition as it cannot result in a return statement.
      stmt.then().accept(*this);
      if (stmt.else_()) stmt.else_()->accept(*this);
   }

   virtual void visit_while_stmt(ast::while_statement const& stmt) override {
      // ignore statement condition as it cannot result in a return statement.
      stmt.body().accept(*this);
   }

   virtual void visit_for_stmt(ast::for_statement const& stmt) override {
      // ignore parts of the for statement
      // todo: implement when for statement is implemented
   }

   virtual void visit_switch_stmt(ast::switch_statement const& stmt) override {
      // ignore parts of the switch statement
      // todo: implement when switch statement is implemented
   }

   virtual void visit_return_stmt(ast::result_statement const& stmt) override {
      _results.push_back(stmt.result_type());
   }

   ast::type_ref result() const noexcept {
      // todo: type reduce the resulting types to the common most type and return that.
      return !_results.empty()
         ? _results.front()
         : ast::types::void_type;
   }

private:
   std::vector<ast::type_ref> _results;
   ast::visitor* _parent;
};


namespace rush::ast {
   void function_type::resolve_return_type(ast::statement const& stmt) const {
      if (_return_type == types::undefined || _return_type == types::error_type) {
         auto v = function_return_type_resolver {};
         stmt.accept(v);
         _return_type = v.result();
      }
   }
}
