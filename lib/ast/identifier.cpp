#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/builtin.hpp"

#include <cassert>

using namespace rush;

struct identifier_name_visitor {
   std::string operator ()(ast::declaration const* decl) { return decl->name(); }
   std::string operator ()(ast::identifier::resolver* res) { return res->name(); }
};

struct identifier_type_visitor {
   ast::type_ref operator ()(ast::declaration const* decl) { return decl->type(); }
   ast::type_ref operator ()(ast::identifier::resolver* res) { return ast::types::undefined; }
};

namespace rush::ast {

   identifier::identifier(resolver& res) : _val { &res } {
      res.listen(this);
   }

   identifier::identifier(identifier&& other) : _val { std::move(other._val) } {
      if (auto res = std::get_if<ast::identifier::resolver*>(&_val)) {
         (*res)->replace_listener(&other, this);
      }
   }

   void identifier::operator = (identifier&& other) {
      _val = std::move(other._val);
      if (auto res = std::get_if<ast::identifier::resolver*>(&_val)) {
         (*res)->replace_listener(&other, this);
      }
   }

   bool identifier::is_unresolved() const noexcept {
      return !std::holds_alternative<ast::declaration const*>(_val);
   }

   std::string identifier::name() const noexcept {
      return std::visit(identifier_name_visitor {}, _val);
   }

   ast::type_ref identifier::type() const noexcept {
      return std::visit(identifier_type_visitor {}, _val);
   }

   ast::declaration const& identifier::declaration() const noexcept {
      assert("attempting to access the declaration of an unresolved identifier.");
      return *std::get<ast::declaration const*>(_val);
   }
} // rush::ast
