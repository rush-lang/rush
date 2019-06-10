#pragma once

#ifndef RUSH_AST_DECLS_IMPORT_HPP
#define RUSH_AST_DECLS_IMPORT_HPP

#include "rush/ast/decls/declaration.hpp"
#include "rush/ast/types/builtin.hpp"

#include <memory>

namespace rush::ast {
   class import_declaration : public ast::declaration {
   public:
      import_declaration(std::string name)
         : _name { std::move(name) } {}

      virtual std::string_view name() const override {
         return _name;
      }

      virtual ast::type_ref type() const override {
         return ast::types::undefined;
      }

		virtual declaration_kind kind() const override {
         return ast::declaration_kind::module;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_import_decl(*this);
      }

      virtual void attach(ast::node&, ast::context&) override {}
      virtual void detach(ast::node&, ast::context&) override {}

   private:
      std::string _name;
   };

   namespace decls {
      inline std::unique_ptr<ast::import_declaration> import(std::string name) {
         return std::make_unique<ast::import_declaration>(std::move(name));
      }
   }
}

#endif // RUSH_AST_DECLS_IMPORT_HPP
