#pragma once

#ifndef RUSH_AST_TYPES_FLOATING_HPP
#define RUSH_AST_TYPES_FLOATING_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/builtin.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   enum class floating_point_kind {
      ieee16,
      ieee32,
      ieee64,
      ieee80,
      ieee128,
      ppc128
   };

   class builtin_floating_point_type : public builtin_type {
   public:
      builtin_floating_point_type(floating_point_kind fpk)
         : _fpkind { fpk } {}

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::builtin_floating_point;
      }

      ast::floating_point_kind fpkind() const noexcept {
         return _fpkind;
      }

      std::size_t size() const noexcept {
         switch (_fpkind) {
         case floating_point_kind::ieee16: return 2;
         case floating_point_kind::ieee32: return 4;
         case floating_point_kind::ieee64: return 8;
         case floating_point_kind::ieee80: return 10;
         case floating_point_kind::ieee128:
         case floating_point_kind::ppc128: return 16;
         // default: throw "unreachable";
         }
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_floating_type(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}

   private:
      floating_point_kind _fpkind;
   };
}

#endif // RUSH_AST_TYPES_FLOATING_HPP
