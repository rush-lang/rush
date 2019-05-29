#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {
   std::string identifier::name() const noexcept {
      return _decl->name();
   }

   ast::type_ref identifier::type() const noexcept {
      return _decl->type();
   }

   ast::declaration const& identifier::declaration() const noexcept {
      return *_decl;
   }
} // rush::ast
