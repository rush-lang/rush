#pragma once

#ifndef RUSH_BUILTIN_TYPE_HPP
#define RUSH_BUILTIN_TYPE_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/named.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   class builtin_type : public named_type {
   public:
      builtin_type(std::string name)
         : named_type(name) {}

      virtual ast::type_kind kind() const noexcept override{
         return ast::type_kind::builtin;
      };

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_type(*this);
		}
   };

   namespace types {
      // sentinal types
      extern ast::type_ref const error_type;
      extern ast::type_ref const inferred_type;

      // known/built-in types
      extern ast::type_ref const void_type;
      extern ast::type_ref const bool_type;
      extern ast::type_ref const byte_type;
      extern ast::type_ref const sbyte_type;
      extern ast::type_ref const short_type;
      extern ast::type_ref const ushort_type;
      extern ast::type_ref const int_type;
      extern ast::type_ref const uint_type;
      extern ast::type_ref const long_type;
      extern ast::type_ref const ulong_type;
      extern ast::type_ref const float_type;
      extern ast::type_ref const double_type;
      extern ast::type_ref const string_type;
      extern ast::type_ref const char_type;
   }
}

#endif // RUSH_BUILTIN_TYPE_HPP
