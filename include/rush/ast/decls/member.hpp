#pragma once

#ifndef RUSH_AST_DECLS_MEMBER_HPP
#define RUSH_AST_DECLS_MEMBER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/decls/access.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <vector>
#include <string>

namespace rush::ast {
   class module;
   class declaration;

   class member_declaration : public ast::declaration {
   public:
      member_declaration(
         ast::declaration const* owner,
         std::unique_ptr<ast::declaration> decl,
         ast::member_access acc)
      : _decl { std::move(decl) }
      , _owner { owner }
      , _access { acc } {}

      ast::declaration const& owner() const noexcept {
         return *_owner;
      }

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

      ast::member_access access() const {
         return _access;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_member_decl(*this);
      }

      virtual void attach(ast::node& parent, ast::context& context) override {
         _decl->attach(parent, context);
      }

      virtual void detach(ast::node& parent, ast::context& context) override {
         _decl->detach(parent, context);
      }

   private:
      ast::member_access _access;
      ast::declaration const* const _owner;
      std::unique_ptr<ast::declaration> _decl;
   };
} // rush::ast

#endif // RUSH_AST_DECLS_MEMBER_HPP
