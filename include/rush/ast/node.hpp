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

#ifndef RUSH_AST_NODE_HPP
#define RUSH_AST_NODE_HPP

namespace rush::ast {
   class visitor;
   class context;

   class node {
   public:
      node() = default;
      virtual ~node() = default;

      ast::node const* parent() const noexcept {
         return _parent;
      }

      ast::context* context() const noexcept {
         return _context;
      }

      void attach(ast::context& context);
      virtual void accept(ast::visitor&) const = 0;
      virtual void accept(ast::visitor&& v) const { accept(v); }

   protected:
      void attach(ast::node& child, ast::node* parent = nullptr);
      void detach(ast::node& child);

      virtual void attached(ast::node*, ast::context&) = 0;
      virtual void detached(ast::node*, ast::context&) = 0;

   private:
      ast::node* _parent;
      ast::context* _context;
   };
} // rush

#endif // RUSH_AST_NODE_HPP
