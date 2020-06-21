/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#pragma once

#ifndef RUSH_BUILTIN_TYPE_HPP
#define RUSH_BUILTIN_TYPE_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"

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

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}
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

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}
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

   protected:
      virtual void attached(ast::scope&, ast::context&) override {}
      virtual void detached(ast::context&) override {}
   };

   namespace types {
      // sentinal types
      extern ast::type_ref const void_type;
      extern ast::type_ref const undefined;
      extern ast::type_ref const undeclared;
      extern ast::type_ref const ambiguous;
      extern ast::type_ref const circular_ref;
      extern ast::type_ref const recursive_ref;
      extern ast::type_ref const inference_fail;
      extern ast::type_ref const irreducible;


      ast::builtin_error_type error_type();
      ast::builtin_error_type error_type(std::string msg);
   }
}

#endif // RUSH_BUILTIN_TYPE_HPP
