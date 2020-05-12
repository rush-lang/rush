#pragma once

#ifndef RUSH_AST_DECLS_CLASS_HPP
#define RUSH_AST_DECLS_CLASS_HPP

#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/member.hpp"
#include "rush/ast/context.hpp"

#include <string>
#include <string_view>

namespace rush::ast {

   class class_declaration : public ast::declaration {
   public:
      class_declaration(std::string name)
         : _name { std::move(name) }
         , _type { types::undefined } {}

      virtual std::string_view name() const noexcept override {
         return _name;
      }

      virtual ast::type_ref type() const noexcept override {
         return types::void_type;
      }

		virtual declaration_kind kind() const noexcept override {
         return declaration_kind::class_;
      }

      std::vector<std::unique_ptr<ast::member_declaration>> const& members() const noexcept {
         return _members;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_class_decl(*this);
      };

      virtual void attach(ast::node&, ast::context& context) override {
      };

      virtual void detach(ast::node&, ast::context&) override {
      };

   private:
      std::string _name;
      ast::type_ref _type;

      std::vector<std::unique_ptr<ast::member_declaration>> _members;
   };
} // rush::ast

namespace rush::ast::decls {
   inline std::unique_ptr<class_declaration> class_(std::string name) {
      return std::make_unique<class_declaration>(
         std::move(name));
   }
}


#endif // RUSH_AST_DECLS_CLASS_HPP
