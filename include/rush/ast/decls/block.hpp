#pragma once

#ifndef RUSH_AST_DECLS_DECLARATION_BLOCK_HPP
#define RUSH_AST_DECLS_DECLARATION_BLOCK_HPP

#include "rush/ast/decls/declaration.hpp"

#include <vector>
#include <algorithm>

namespace rush::ast {
   class declaration_block : public node {
   public:
      declaration_block(std::vector<std::unique_ptr<declaration>> decls)
         : _decls { std::move(decls) } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         std::for_each(_decls.begin(), _decls.end(),
            [&v](auto& decl) { decl->accept(v); });
      }

   private:
      std::vector<std::unique_ptr<ast::declaration>> _decls;
   };

   namespace decls {
      inline std::unique_ptr<declaration_block> block(
         std::vector<std::unique_ptr<ast::declaration>> decls) {
            return std::make_unique<declaration_block>(std::move(decls));
         }
   }
}

#endif // RUSH_AST_DECLS_DECLARATION_BLOCK_HPP
