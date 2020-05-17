#pragma once

#ifndef RUSH_AST_DECLS_NOMINAL_HPP
#define RUSH_AST_DECLS_NOMINAL_HPP

#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {
   class nominal_declaration : public declaration {
   public:
      virtual std::string_view name() const = 0;

      ast::identifier identifier() const { return { this }; };
   protected:
      nominal_declaration() = default;
   };
}

#endif // RUSH_AST_DECLS_NOMINAL_HPP
