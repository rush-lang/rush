#pragma once

#ifndef RUSH_BUILTIN_TYPE_HPP
#define RUSH_BUILTIN_TYPE_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   enum class integral_kind {
      byte,
      word,
      dword,
      qword
   };

   enum class floating_kind {
      ieee16,
      ieee32,
      ieee64,
      ieee80,
      ieee128,
      ppc128
   };

   class builtin_type : public type {
   public:
   };

   class builtin_void_type : public builtin_type {
   public:
      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::builtin_void;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_void_type(*this);
		}
   };

   class builtin_bool_type : public builtin_type {
   public:
      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::builtin_bool;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_bool_type(*this);
		}
   };

   class builtin_integral_type : public builtin_type {
   public:
      builtin_integral_type(integral_kind kind, bool sign)
         : _kind { kind }
         , _signed { sign } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::builtin_integral;
      }

      bool is_signed() const noexcept {
         return _signed;
      }

      integral_kind unit() const noexcept {
         return _kind;
      }

      std::size_t size() const noexcept {
         switch (_kind) {
         case integral_kind::byte: return 1;
         case integral_kind::word: return 2;
         case integral_kind::dword: return 4;
         case integral_kind::qword: return 8;
         // default: throw "unreachable"; // assert("unreachable");
         }
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_integral_type(*this);
		}

   private:
      integral_kind _kind;
      bool _signed;
   };

   class builtin_floating_type : public builtin_type {
   public:
      builtin_floating_type(floating_kind fpk)
         : _fpkind { fpk } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::builtin_floating;
      }

      ast::floating_kind fpkind() const noexcept {
         return _fpkind;
      }

      std::size_t size() const noexcept {
         switch (_fpkind) {
         case floating_kind::ieee16: return 2;
         case floating_kind::ieee32: return 4;
         case floating_kind::ieee64: return 8;
         case floating_kind::ieee80: return 10;
         case floating_kind::ieee128:
         case floating_kind::ppc128: return 16;
         // default: throw "unreachable";
         }
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_floating_type(*this);
		}

   private:
      floating_kind _fpkind;
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
