#pragma once

#ifndef RUSH_AST_TYPES_NOMINAL_HPP
#define RUSH_AST_TYPES_NOMINAL_HPP

#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {
   /*! \brief Represents a type with a name that is significant, such that the name
    *         distinguishes it from other nominally typed structures.
    */
   class nominal_type {
   public:

      //! returns the declaration that declares this type.
      ast::declaration* declaration() const noexcept {
         return _decl;
      }

   private:
      ast::declaration* _decl;
   };
}

#endif // RUSH_AST_TYPES_NOMINAL_HPP
