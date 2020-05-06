#include "rush/ast/traversal.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/types.hpp"

#include "rush/ast/exprs/array.hpp"
#include "rush/ast/exprs/tuple.hpp"
#include "rush/ast/exprs/lambda.hpp"
#include "rush/ast/decls/function.hpp"

using namespace rush;

rush::ast::type_ref resolve_function_type(
   rush::ast::lambda_expression const& expr,
   std::vector<rush::ast::node const*> resolving,
   rush::ast::context& context);

rush::ast::type_ref resolve_function_type(
   rush::ast::function_declaration const& decl,
   std::vector<rush::ast::node const*> resolving,
   rush::ast::context& context);

bool is_infinitely_recursive_type(ast::type_ref type) {
   struct infinite_recursion_type_checker : ast::visitor {
      bool result = false;
      void visit_builtin_error_type(ast::builtin_error_type const& errtype) override
      { result = (errtype == ast::types::recursive_ref); }
      void visit_function_type(ast::function_type const& fntype) override
      { fntype.return_type().accept(*this); }
   };

   return rush::visit(type, infinite_recursion_type_checker {}).result;
}

ast::type_ref reduce_expression_types(ast::type_ref lt, ast::type_ref rt) {
   return is_infinitely_recursive_type(lt)
        ? is_infinitely_recursive_type(rt)
        ? lt : rt
        : is_infinitely_recursive_type(rt)
        ? lt : ast::types::reduce(lt, rt);
}

ast::type_ref reduce_return_types(ast::type_ref lt, ast::type_ref rt) {
   return lt.kind() == ast::type_kind::error
        ? rt.kind() == ast::type_kind::error
        ? lt : rt
        : rt.kind() == ast::type_kind::error
        ? lt : reduce_expression_types(lt, rt);
}

class return_type_resolver : public ast::visitor {
public:
   return_type_resolver(ast::context& context, std::vector<ast::node const*> resolving)
      : _resolving { std::move(resolving) }
      , _context { &context }
      , _result { ast::types::undefined } {}

   ast::type_ref result() const noexcept {
       return _result.kind() == ast::type_kind::error
           && _result != ast::types::recursive_ref
            ? ast::types::inference_fail
            : _result;
   }

   void visit_lambda_expr(ast::lambda_expression const& expr) override {
      _result = expr.return_type();
      resolve_return_type(expr.body());
   }

   void visit_function_decl(ast::function_declaration const& decl) override {
      _result = decl.return_type();
      resolve_return_type(decl.body());
   }

private:
   std::vector<ast::node const*> _resolving;
   ast::context* _context;
   ast::type_ref _result;

   bool is_resolving(ast::node const* node) const noexcept {
      return std::find(
         _resolving.begin(),
         _resolving.end(),
         node) != _resolving.end();
   }

   void resolve_return_type(ast::node const& node) {
      if (is_resolving(&node)) {
         _result = ast::types::recursive_ref;
      } else {
         _resolving.push_back(&node);
         if (_result.kind() == rush::ast::type_kind::error)
            _result = rush::visit(node, function_body_traversal { *this }).result();
         _resolving.pop_back();
      }
   }

   class function_body_traversal;
   class result_statement_traversal;

   //! \brief Traverses the body of a function or lambda while collecting
   //         the result types of every result statement. The result of
   //         this traversal is the reduction of all discovered types.
   class function_body_traversal : public ast::traversal {
   public:
      function_body_traversal(return_type_resolver const& resolver)
         : _resolver { &resolver } {}

      ast::type_ref result() const noexcept {
         return _results.empty()
              ? ast::types::void_type
              : std::accumulate(
                 std::next(_results.begin()),
                 _results.end(),
                 _results.front(),
                 reduce_return_types);
      }

      void visit_constant_decl(ast::constant_declaration const& decl) override { /* can safely ignore */ }
      void visit_variable_decl(ast::variable_declaration const& decl) override { /* can safely ignore */ }

      void visit_unary_expr(ast::unary_expression const& expr) override { /* can safely ignore */ }
      void visit_binary_expr(ast::binary_expression const& expr) override { /* can safely ignore */ }
      void visit_lambda_expr(ast::lambda_expression const& expr) override { /* can safely ignore */ }
      void visit_invoke_expr(ast::invoke_expression const& expr) override { /* can safely ignore */ }

      void visit_conditional_stmt(ast::conditional_statement const& stmt) override { stmt.body().accept(*this); }
      void visit_iteration_stmt(ast::iteration_statement const& stmt) override { /*stmt.body().accept(*this);*/ }
      void visit_switch_stmt(ast::switch_statement const& stmt) override { /*stmt.body().accept(*this);*/ }

      void visit_result_stmt(ast::result_statement const& stmt) override {
         auto result_type = stmt.result_type().kind() == ast::type_kind::error
            ? rush::visit(stmt.expression(), result_statement_traversal { stmt.result_type(), *_resolver }).result()
            : stmt.result_type();

         _results.push_back(result_type);
      }

   private:
      std::vector<ast::type_ref> _results;
      return_type_resolver const* _resolver;
   };

   class result_statement_traversal : public ast::traversal {
   public:
      result_statement_traversal(ast::type_ref initial, return_type_resolver const& resolver)
         : _resolver { &resolver }
         , _initial { initial } {}

      ast::type_ref result() const noexcept {
         return !_results.empty()
              ? std::accumulate(
                 std::next(_results.begin()),
                 _results.end(),
                 _results.front(),
                  reduce_expression_types)
              : _initial;
      }

      void visit_identifier_expr(ast::identifier_expression const& expr) override {
         if (!expr.is_unresolved() && expr
                  .declaration().kind() == rush::ast::declaration_kind::function) {
                     rush::visit(expr.declaration(), *this);
                     return;
                  }
         _results.push_back(expr.result_type());
      }

      // traverse the callable and unwrap the result type.
      void visit_invoke_expr(ast::invoke_expression const& expr) override {
         struct unwrap_function_type : ast::visitor {
            ast::type_ref result;
            unwrap_function_type(ast::type_ref start) : result { start } {}
            void visit_function_type(ast::function_type const& type) {
               result = type.return_type();
            }
         };

         auto result = rush::visit(expr.callable(), *this).result();
         result = rush::visit(result, unwrap_function_type { result }).result;

         _results.pop_back();
         _results.push_back(result);
      }

      // treat ternary expressions as if they were two individual return statements.
      void visit_ternary_expr(ast::ternary_expression const& expr) override {
         std::vector<ast::type_ref> results;

         results.push_back(expr.true_expr().result_type().kind() == rush::ast::type_kind::error
                         ? rush::visit(expr.true_expr(), result_statement_traversal {
                           expr.true_expr().result_type(), *_resolver }).result()
                         : expr.true_expr().result_type());

         results.push_back(expr.false_expr().result_type().kind() == rush::ast::type_kind::error
                         ? rush::visit(expr.false_expr(), result_statement_traversal {
                           expr.false_expr().result_type(), *_resolver }).result()
                         : expr.false_expr().result_type());

         _results.push_back(std::accumulate(
                 std::next(results.begin()),
                 results.end(),
                 results.front(),
                 reduce_return_types));
      }

      void visit_lambda_expr(ast::lambda_expression const& expr) override {
         _results.push_back(resolve_function_type(expr, _resolver->_resolving, *_resolver->_context));
      }

      void visit_function_decl(ast::function_declaration const& decl) override {
         _results.push_back(resolve_function_type(decl, _resolver->_resolving, *_resolver->_context));
      }

   private:
      std::vector<ast::type_ref> _results;
      return_type_resolver const* _resolver;
      ast::type_ref _initial;
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
      return resolve_function_type(expr, { &expr }, *this);
   }

   ast::type_ref context::function_type(ast::function_declaration const& decl) {
      return resolve_function_type(decl, { &decl }, *this);
   }
}

rush::ast::type_ref resolve_function_type(
   rush::ast::lambda_expression const& expr,
   std::vector<rush::ast::node const*> resolving,
   rush::ast::context& context) {
      auto tutype = context.tuple_type(expr.parameters().types());
      auto fntype = context.function_type(rush::visit(expr, return_type_resolver { context, std::move(resolving) }).result(), tutype);
      return fntype;
   }

rush::ast::type_ref resolve_function_type(
   rush::ast::function_declaration const& decl,
   std::vector<rush::ast::node const*> resolving,
   rush::ast::context& context) {
      auto tutype = context.tuple_type(decl.parameters().types());
      auto fntype = context.function_type(rush::visit(decl, return_type_resolver { context, std::move(resolving) }).result(), tutype);
      return fntype;
   }
