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

#ifndef RUSH_AST_SCOPE_HPP
#define RUSH_AST_SCOPE_HPP

#include "rush/ast/decls/nominal.hpp"
#include "rush/extra/iterator_range.hpp"
#include "rush/extra/dereferencing_iterator.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stack>

namespace rush::ast {
   class scope;
   class scope_frame;

   enum class scope_kind {
      global,
      function,
      struct_,
      class_,
      enum_,
      concept,
      interface,
      extension,
      module,
      block,
   };

   class symbol {
      friend class scope_frame;

      std::string _name;
      std::unordered_set<ast::nominal_declaration const*> _decls;

   public:
      bool is_undefined() const noexcept {
         return _decls.empty();
      }

      bool is_overloaded() const noexcept {
         return _decls.size() > 1;
      }

      std::string const& name() const noexcept {
         return _name;
      }

      auto overloads() {
         return rush::make_iterator_range(
            rush::make_deref_iterator(_decls.begin()),
            rush::make_deref_iterator(_decls.end()));
      }

      auto overloads() const {
         return rush::make_iterator_range(
            rush::make_deref_iterator(_decls.begin()),
            rush::make_deref_iterator(_decls.end()));
      }

   public:
      explicit symbol(std::string name)
         : _name { std::move(name) } {}

      explicit symbol(std::string_view name)
         : _name { name } {}

      void insert(ast::nominal_declaration const& decl) {
         _decls.insert(&decl);
      }
   };

   class scope_frame {
      friend class scope;

   public:
      scope_frame(scope_kind kind, ast::scope_frame* parent = nullptr)
         : _kind { kind }
         , _parent { parent } {}

      //! \brief Gets the kind of this scope.
      scope_kind kind() const noexcept {
         return _kind;
      }

      //! \brief Returns true if this scope_frame is the root frame in the scope chain; otherwise false.
      bool is_root() const noexcept {
         return !parent();
      }

      //! \brief Gets the depth of the scope_frame relative to its ancestor scopes.
      std::size_t depth() const noexcept {
			return !is_root() ? parent()->depth() + 1 : 0;
		}

      //! \brief Gets the parent scope_frame, nullptr if this scope_frame is the root.
      ast::scope_frame* parent() noexcept {
         return _parent;
      }

      //! \brief Gets the parent scope_frame, nullptr if this scope_frame is the root.
      ast::scope_frame const* parent() const noexcept {
         return _parent;
      }

   private:
      mutable std::unordered_map<
         std::string_view,
         std::unique_ptr<ast::symbol>> _symbols;
      ast::scope_frame* _parent;
      ast::scope_kind _kind;

      //! \brief Inserts the specified declaration as a symbol into the scope frame.
      void insert(ast::nominal_declaration const&);

      //! \brief Performs lookup of symbols with the specified name within the scope chain.
      ast::symbol const& lookup(std::string_view name) const;
   };

   class scope {
   public:
      //! \brief Pops the current scope frame from the stack.
      void pop();

      //! \brief Pushes a new scope frame on the stack.
      void push(scope_kind);

      //! \brief Gets the current scope frame, the frame on top of the stack.
      ast::scope_frame const& current() const noexcept;

      //! \brief Inserts the declaration into the current scope frame.
      void insert(ast::nominal_declaration const&);

      //! \brief Performs lookup of symbols with the specified name within the scope chain.
      ast::symbol const& lookup(std::string_view name) const;

   private:
      std::stack<ast::scope_frame> _frames;
   };
}

#endif // RUSH_AST_SCOPE_HPP
