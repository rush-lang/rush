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
#if defined(_MSC_VER) ||                                            \
   (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
   (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#ifndef RUSH_AST_NODE_HPP
#define RUSH_AST_NODE_HPP

#include "rush/core/location.hpp"

namespace rush::ast {
   class visitor;
   class context;
   class scope;

   class node {
   public:
      node()
      : _parent { nullptr }
      , _context { nullptr } {}

      virtual ~node() = default;

      ast::context* context() const noexcept {
         return _context;
      }

      ast::node* parent() noexcept {
         return _parent;
      }

      ast::node const* parent() const noexcept {
         return _parent;
      }

      virtual rush::location_range location() const noexcept {
         return {
            rush::location::undefined ,
            rush::location::undefined };
      }

      void attach(ast::context& context);
      virtual void accept(ast::visitor&) const = 0;
      virtual void accept(ast::visitor&& v) const { accept(v); }

   protected:
      void adopt(ast::node& child, ast::node* parent = nullptr);
      void orphan(ast::node& child);

      void attach(ast::scope&, ast::node& child);
      void detach(ast::node& child);

      virtual void adopted(ast::node& parent) {}
      virtual void orphaned(ast::node& parent) {}

      virtual void attached(ast::scope&, ast::context&) = 0;
      virtual void detached(ast::context&) = 0;

   private:
      ast::node* _parent;
      ast::context* _context;
   };
} // rush::ast

#endif // RUSH_AST_NODE_HPP
