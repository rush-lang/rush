#include "rush/ast/traversal.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/types.hpp"

#include "rush/ast/exprs/array.hpp"
#include "rush/ast/exprs/tuple.hpp"
#include "rush/ast/exprs/lambda.hpp"
#include "rush/ast/decls/function.hpp"

const rush::ast::builtin_error_type infinite_recursion_error_type =
      rush::ast::types::error_type("deducing return type is infinitely recursive.");

rush::ast::type_ref resolve_function_type(
   rush::ast::lambda_expression const& expr,
   rush::ast::node const& resolving,
   rush::ast::context& context);

rush::ast::type_ref resolve_function_type(
   rush::ast::function_declaration const& decl,
   rush::ast::node const& resolving,
   rush::ast::context& context);

class return_type_resolver : public rush::ast::visitor {
public:
   return_type_resolver(rush::ast::node const& node, rush::ast::context& context)
      : _return_type { rush::ast::types::undefined }
      , _node_resolving { &node }
      , _context { context } {}

   rush::ast::type_ref result() const noexcept {
      return _return_type;
   }

   virtual void visit_lambda_expr(rush::ast::lambda_expression const& expr) override {
      _return_type = expr.return_type();
      resolve_return_type(expr.body());
   }

   virtual void visit_function_decl(rush::ast::function_declaration const& decl) override {
      _return_type = decl.return_type();
      resolve_return_type(decl.body());
   }

private:
   rush::ast::context& _context;
   rush::ast::type_ref _return_type;
   rush::ast::node const* _node_resolving;

   void resolve_return_type(rush::ast::node const& node) {
      if (_return_type.kind() == rush::ast::type_kind::error) {
         auto v = function_body_traversal { *this };
         _return_type = rush::visit(node, v).result();
      }
   }

   class function_body_traversal : public rush::ast::traversal {
   public:
      function_body_traversal(
         return_type_resolver& resolver)
         : _resolver { resolver }{}

      rush::ast::type_ref result() const noexcept {
         return _results.empty()
            ? rush::ast::types::void_type
            : _results.size() == 1
            ? _results.front()
            : std::accumulate(
               std::next(_results.begin()),
               _results.end(),
               _results.front(),
               rush::ast::types::reduce);
      }

      virtual void visit_constant_decl(rush::ast::constant_declaration const&) override { /*ignore*/ }
      virtual void visit_variable_decl(rush::ast::variable_declaration const&) override { /*ignore*/ }
      virtual void visit_unary_expr(rush::ast::unary_expression const& expr) override { /*ignore*/ }
      virtual void visit_binary_expr(rush::ast::binary_expression const& expr) override { /*ignore*/ }
      virtual void visit_lambda_expr(rush::ast::lambda_expression const& expr) override { /*ignore !important*/ }
      virtual void visit_literal_expr(rush::ast::array_literal_expression const& expr) override { /*ignore*/ }
      virtual void visit_literal_expr(rush::ast::tuple_literal_expression const& expr) override { /*ignore*/ }

      virtual void visit_conditional_stmt(rush::ast::conditional_statement const& stmt) override {
         // ignore statement condition as it cannot result in a return statement.
         stmt.body().accept(*this);
      }

      virtual void visit_for_stmt(rush::ast::iteration_statement const& stmt) override {
         // ignore parts of the for statement
         // todo: implement when for statement is implemented
      }

      virtual void visit_switch_stmt(rush::ast::switch_statement const& stmt) override {
         // ignore parts of the switch statement
         // todo: implement when switch statement is implemented
      }

      virtual void visit_result_stmt(rush::ast::result_statement const& stmt) override {
         auto type = stmt.result_type();
         if (type.kind() == rush::ast::type_kind::error) {
            type = rush::visit(stmt.expression(), return_statement_traversal { _resolver }).result();
            if (type == infinite_recursion_error_type) _results.clear();
         }

         _results.push_back(type);
      }

   private:
      return_type_resolver& _resolver;
      std::vector<rush::ast::type_ref> _results;

      class return_statement_traversal : public rush::ast::traversal {
      public:
         return_statement_traversal(
            return_type_resolver& resolver)
            : _resolver { resolver } {}

         rush::ast::type_ref result() const noexcept {
            return _results.empty()
               ? rush::ast::types::void_type
               : _results.size() == 1
               ? _results.front()
               : std::accumulate(
                  std::next(_results.begin()),
                  _results.end(),
                  _results.front(),
                  rush::ast::types::reduce);
         }

         virtual void visit_function_decl(rush::ast::function_declaration const& decl) override {
            if (_resolver._node_resolving == &decl) {
               _results.clear();
               _results.push_back(infinite_recursion_error_type);
            } else {
               auto result = resolve_function_type(
                  decl, *_resolver._node_resolving, _resolver._context);
               if (result == infinite_recursion_error_type) _results.clear();
               _results.push_back(result);
            }
         }

         virtual void visit_lambda_expr(rush::ast::lambda_expression const& expr) override {
            if (_resolver._node_resolving == &expr) {
               _results.clear();
               _results.push_back(infinite_recursion_error_type);
            } else {
               auto result = resolve_function_type(
                  expr, *_resolver._node_resolving, _resolver._context);
               if (result == infinite_recursion_error_type) _results.clear();
               _results.push_back(result);
            }
         }

         virtual void visit_identifier_expr(rush::ast::identifier_expression const& expr) override {
            if (!expr.is_unresolved() && expr
                     .declaration().kind() == rush::ast::declaration_kind::function) {
                        rush::visit(expr.declaration(), *this);
                        return;
                     }

            _results.push_back(expr.result_type());
         }

      private:
         return_type_resolver& _resolver;
         std::vector<rush::ast::type_ref> _results;
      };
   };
};


namespace rush::ast {
   ast::type_ref context::bool_type() {
      static const ast::builtin_bool_type bool_type {};
      return { bool_type };
   }

   ast::type_ref context::char_type() { return types::undefined; }

   ast::type_ref context::string_type() {
      static const builtin_string_type string_type {};
      return string_type;
   }

   ast::type_ref context::ieee32_type() { return floating_point_type(floating_point_kind::ieee32); }
   ast::type_ref context::ieee64_type() { return floating_point_type(floating_point_kind::ieee64); }

   ast::type_ref context::int8_type() { return signed_integral_type(integral_kind::byte); }
   ast::type_ref context::int16_type() { return signed_integral_type(integral_kind::word); }
   ast::type_ref context::int32_type() { return signed_integral_type(integral_kind::dword); }
   ast::type_ref context::int64_type() { return signed_integral_type(integral_kind::qword); }

   ast::type_ref context::uint8_type() { return unsigned_integral_type(integral_kind::byte); }
   ast::type_ref context::uint16_type() { return unsigned_integral_type(integral_kind::word); }
   ast::type_ref context::uint32_type() { return unsigned_integral_type(integral_kind::dword); }
   ast::type_ref context::uint64_type() { return unsigned_integral_type(integral_kind::qword); }

   ast::type_ref context::signed_integral_type(integral_kind kind) {
      static const builtin_integral_type int8_type { integral_kind::byte, true };
      static const builtin_integral_type int16_type { integral_kind::word, true };
      static const builtin_integral_type int32_type { integral_kind::dword, true };
      static const builtin_integral_type int64_type { integral_kind::qword, true };

      switch (kind) {
      case integral_kind::byte: return int8_type;
      case integral_kind::word: return int16_type;
      case integral_kind::dword: return int32_type;
      case integral_kind::qword: return int64_type;
      }
   }

   ast::type_ref context::unsigned_integral_type(integral_kind kind) {
      static const builtin_integral_type uint8_type { integral_kind::byte, false };
      static const builtin_integral_type uint16_type { integral_kind::word, false };
      static const builtin_integral_type uint32_type { integral_kind::dword, false };
      static const builtin_integral_type uint64_type { integral_kind::qword, false };

      switch (kind) {
      case integral_kind::byte: return uint8_type;
      case integral_kind::word: return uint16_type;
      case integral_kind::dword: return uint32_type;
      case integral_kind::qword: return uint64_type;
      }
   }

   ast::type_ref context::floating_point_type(floating_point_kind kind) {
      static const builtin_floating_point_type ieee16_type { floating_point_kind::ieee16 };
      static const builtin_floating_point_type ieee32_type { floating_point_kind::ieee32 };
      static const builtin_floating_point_type ieee64_type { floating_point_kind::ieee64 };
      static const builtin_floating_point_type ieee80_type { floating_point_kind::ieee80 };
      static const builtin_floating_point_type ieee128_type { floating_point_kind::ieee128 };
      static const builtin_floating_point_type ppc128_type { floating_point_kind::ppc128 };

      switch (kind) {
      case floating_point_kind::ieee16: return ieee16_type;
      case floating_point_kind::ieee32: return ieee32_type;
      case floating_point_kind::ieee64: return ieee64_type;
      case floating_point_kind::ieee80: return ieee80_type;
      case floating_point_kind::ieee128: return ieee128_type;
      case floating_point_kind::ppc128: return ppc128_type;
      }
   }

   ast::type_ref context::array_type(ast::type_ref type, size_type rank) {
      auto key = detail::array_type_key_t { rank, type };
      auto it = _array_types.find(key);
      if (it == _array_types.end()) {
         auto p = std::make_unique<ast::array_type>(type, std::vector<array_type_dim> {});
         it = _array_types.insert({ key, std::move(p) }).first;
      }
      return *it->second;
   }

   ast::type_ref context::tuple_type(ast::type_ref single) {
      auto types = std::vector<ast::type_ref> { single };
      return tuple_type(std::move(types));
   }

   ast::type_ref context::tuple_type(rush::iterator_range<std::vector<ast::type_ref>::const_iterator> types) {
      auto key = detail::tuple_type_key_t { types };
      auto it = _tuple_types.find(key);
      if (it == _tuple_types.end()) {
         std::vector<ast::type_ref> vtypes { types.begin(), types.end() };
         key.types = vtypes;

         auto p = std::make_unique<ast::tuple_type>(std::move(vtypes));
         it = _tuple_types.insert({ key, std::move(p) }).first;
      }
      return *it->second;
   }

   ast::type_ref context::function_type(ast::type_ref ret, ast::type_ref params) {
      auto key = detail::function_type_key_t { ret, params };
      auto it = _function_types.find(key);
      if (it == _function_types.end()) {
         auto p = std::make_unique<ast::function_type>(ret, params);
         it = _function_types.insert({ key, std::move(p) }).first;
      }
      return *it->second;
   }

   ast::type_ref context::array_type(ast::array_literal_expression& expr) {
      auto types = expr.elements().result_types();
      return types.empty()
           ? array_type(types::undefined)
           : types.size() == 1
           ? array_type(types.front())
           : array_type(std::accumulate(
               std::next(types.begin()),
               types.end(),
               types.front(),
               ast::types::reduce));
   }

   ast::type_ref context::tuple_type(ast::tuple_literal_expression& expr) {
      return tuple_type(expr.arguments().result_types());
   }

   ast::type_ref context::function_type(ast::lambda_expression const& expr) {
      return resolve_function_type(expr, expr, *this);
   }

   ast::type_ref context::function_type(ast::function_declaration const& decl) {
      return resolve_function_type(decl, decl, *this);
   }
}

rush::ast::type_ref resolve_function_type(
   rush::ast::lambda_expression const& expr,
   rush::ast::node const& resolving,
   rush::ast::context& context) {
      auto tutype = context.tuple_type(expr.parameters().types());
      auto fntype = context.function_type(rush::visit(expr, return_type_resolver { resolving, context }).result(), tutype);
      return fntype;
   }

rush::ast::type_ref resolve_function_type(
   rush::ast::function_declaration const& decl,
   rush::ast::node const& resolving,
   rush::ast::context& context) {
      auto tutype = context.tuple_type(decl.parameters().types());
      auto fntype = context.function_type(rush::visit(decl, return_type_resolver { resolving, context }).result(), tutype);
      return fntype;
   }
