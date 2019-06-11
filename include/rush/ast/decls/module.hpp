#pragma once

#ifndef RUSH_AST_DECLS_MODULE_HPP
#define RUSH_AST_DECLS_MODULE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/decls/access.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <vector>
#include <string>

namespace rush::ast {
   class module;
   class declaration;

   class module_declaration : public ast::declaration {
   public:
      module_declaration(
         std::unique_ptr<ast::declaration> decl,
         ast::module_access acc)
      : _decl { std::move(decl) }
      , _access { acc } {}

      ast::declaration const& declaration() const noexcept {
         return *_decl;
      }

      virtual std::string_view name() const override {
         return _decl->name();
      }

      virtual ast::type_ref type() const override {
         return _decl->type();
      }

		virtual declaration_kind kind() const override {
         return _decl->kind();
      }

      ast::module_access access() const {
         return _access;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module_decl(*this);
      }

      virtual void attach(ast::node& parent, ast::context& context) override {
         _decl->attach(parent, context);
      }

      virtual void detach(ast::node& parent, ast::context& context) override {
         _decl->detach(parent, context);
      }

   private:
      ast::module_access _access;
      std::unique_ptr<ast::declaration> _decl;
   };
} // rush::ast

#endif // RUSH_AST_DECLS_MODULE_HPP
