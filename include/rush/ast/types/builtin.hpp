#pragma once

#ifndef RUSH_BUILTIN_TYPE_HPP
#define RUSH_BUILTIN_TYPE_HPP

#include "rush/ast/types/named.hpp"

namespace rush::ast {
   class builtin_type : public named_type {
   public:
      builtin_type(std::string name)
         : named_type(name) {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			// v.visit_builtin_type(*this);
		}
   };

   namespace types {
      // sentinal types
      extern std::unique_ptr<builtin_type> const error_type;
      extern std::unique_ptr<builtin_type> const inferred_type;

      // known/built-in types
      extern std::unique_ptr<builtin_type> const void_type;
      extern std::unique_ptr<builtin_type> const bool_type;
      extern std::unique_ptr<builtin_type> const byte_type;
      extern std::unique_ptr<builtin_type> const sbyte_type;
      extern std::unique_ptr<builtin_type> const short_type;
      extern std::unique_ptr<builtin_type> const ushort_type;
      extern std::unique_ptr<builtin_type> const int_type;
      extern std::unique_ptr<builtin_type> const uint_type;
      extern std::unique_ptr<builtin_type> const long_type;
      extern std::unique_ptr<builtin_type> const ulong_type;
      extern std::unique_ptr<builtin_type> const float_type;
      extern std::unique_ptr<builtin_type> const double_type;
      extern std::unique_ptr<builtin_type> const string_type;
      extern std::unique_ptr<builtin_type> const char_type;
   }
}

#endif // RUSH_BUILTIN_TYPE_HPP
