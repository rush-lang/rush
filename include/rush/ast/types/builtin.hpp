#pragma once

#ifndef RUSH_BUILTIN_TYPE_HPP
#define RUSH_BUILTIN_TYPE_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"

#include <string>
#include <memory>

namespace rush::ast {
   class builtin_type : public type {
   public:
   };

   class builtin_error_type : public builtin_type {
   public:
      builtin_error_type() = default;
      builtin_error_type(std::string msg)
         : _msg { std::move(msg) } {}

      std::string const& message() const noexcept {
         return _msg;
      }

      virtual ast::type_kind kind() const noexcept override {
         return ast::type_kind::error;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
			v.visit_builtin_error_type(*this);
		}

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}
   private:
      std::string _msg;
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

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}
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

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}
   };

   namespace types {
      // sentinal types
      extern ast::type_ref const undefined;
      extern ast::type_ref const void_type;

      ast::builtin_error_type error_type();
      ast::builtin_error_type error_type(std::string msg);
   }
}

#endif // RUSH_BUILTIN_TYPE_HPP
