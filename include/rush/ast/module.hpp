#pragma once

#ifndef RUSH_AST_MODULE_HPP
#define RUSH_AST_MODULE_HPP

#include "rush/ast/decls/import.hpp"
#include "rush/ast/decls/module.hpp"
#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/decls/undeclared.hpp"

#include <vector>
#include <algorithm>

namespace rush::ast {
   class module : public ast::node {
   public:
      module(std::string id)
         : _id { std::move(id) } {}

      std::string const& id() const noexcept {
         return _id;
      }

      void push_back(std::unique_ptr<ast::import_declaration> imp) {
         _imports.push_back(std::move(imp));
      }

      void push_back(std::unique_ptr<ast::declaration> decl, ast::module_access access) {
         _decls.push_back(std::make_unique<ast::module_declaration>(std::move(decl), access));
      }

      std::vector<std::unique_ptr<import_declaration>> const& imports() const noexcept {
         return _imports;
      }

      std::vector<std::unique_ptr<module_declaration>> const& declarations() const noexcept {
         return _decls;
      }

      ast::declaration const* undeclared_identifier(std::string name) {
         push_back(std::make_unique<ast::undeclared_identifier>(std::move(name)), ast::module_access::internal);
         return _decls.back().get();
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module(*this);
      }

      virtual void attach(ast::node&, ast::context& ctx) override {
         std::for_each(_imports.begin(), _imports.end(), [this, &ctx](auto& imp) { imp->attach(*this, ctx); });
         std::for_each(_decls.begin(), _decls.end(), [this, &ctx](auto& decl) { decl->attach(*this, ctx); });
      }

      virtual void detach(ast::node&, ast::context& ctx) override {
         std::for_each(_imports.begin(), _imports.end(), [this, &ctx](auto& imp) { imp->detach(*this, ctx); });
         std::for_each(_decls.begin(), _decls.end(), [this, &ctx](auto& decl) { decl->detach(*this, ctx); });
      }

   private:
      std::vector<std::unique_ptr<import_declaration>> _imports;
      std::vector<std::unique_ptr<module_declaration>> _decls;
      std::string _id;
   };
} // rush::ast

#endif // RUSH_AST_MODULE_HPP
