#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <string>

namespace rush::ast {
   class parameter;
   class storage_declaration;
   class function_declaration;

   class identifier {
      friend class parameter;
      friend class storage_declaration;
      friend class function_declaration;

   public:
      std::string name() const noexcept {
         return _decl.name();
      }

      ast::type_ref type() const noexcept {
         return _decl.type();
      }

      ast::declaration const& declaration() const noexcept {
         return _decl;
      }

   private:
      ast::declaration const& _decl;

      identifier(ast::declaration const& decl)
         : _decl { decl } {}
   };
}

#endif // RUSH_AST_IDENTIFIER_HPP
