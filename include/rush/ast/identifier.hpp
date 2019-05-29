#pragma once

#ifndef RUSH_AST_IDENTIFIER_HPP
#define RUSH_AST_IDENTIFIER_HPP

#include "rush/ast/types/type.hpp"

#include <string>

namespace rush::ast {
   class declaration;

   class identifier {
      friend class declaration;

   public:
      std::string name() const noexcept;
      ast::type_ref type() const noexcept;
      ast::declaration const& declaration() const noexcept;

   private:
      ast::declaration const* _decl;

      identifier(ast::declaration const* decl)
         : _decl { decl } {}
   };
}

#endif // RUSH_AST_IDENTIFIER_HPP
