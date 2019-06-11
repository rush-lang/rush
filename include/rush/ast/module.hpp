#pragma once

#ifndef RUSH_AST_MODULE_HPP
#define RUSH_AST_MODULE_HPP

#include "rush/ast/decls/import.hpp"
#include "rush/ast/decls/module.hpp"

namespace rush::ast {
   class module : public ast::node {
   public:
      module()
         : module("<unnamed>") {}

      module(std::string name)
         : _name { std::move(name) } {}

      std::string const& name() const noexcept {
         return _name;
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

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module(*this);
      }

      virtual void attach(ast::node&, ast::context&) override {

      }

      virtual void detach(ast::node&, ast::context&) override {

      }

   private:
      std::vector<std::unique_ptr<import_declaration>> _imports;
      std::vector<std::unique_ptr<module_declaration>> _decls;
      std::string _name;
   };
} // rush::ast

#endif // RUSH_AST_MODULE_HPP
