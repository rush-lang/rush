#pragma once

#ifndef RUSH_AST_TYPES_INTEGRAL_HPP
#define RUSH_AST_TYPES_INTEGRAL_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   enum class integral_kind {
      byte,
      word,
      dword,
      qword
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

      std::size_t bit_width() const noexcept {
         return size() * 8;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_integral_type(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}

   private:
      integral_kind _kind;
      bool _signed;
   };
}

#endif // RUSH_AST_TYPES_INTEGRAL_HPP
