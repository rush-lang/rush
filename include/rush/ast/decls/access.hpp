#pragma once

#ifndef RUSH_AST_DECLS_ACCESS_HPP
#define RUSH_AST_DECLS_ACCESS_HPP

#include <cstdint>

namespace rush::ast {
   enum class module_access : std::uint8_t {
      internal,
      exported
   };

   enum class type_access : std::uint8_t {
      public_,
      private_,
      protected_,
      internal
   };
}

#endif // RUSH_AST_DECLS_ACCESS_HPP
