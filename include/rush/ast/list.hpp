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

#ifndef RUSH_AST_NODE_LIST_HPP
#define RUSH_AST_NODE_LIST_HPP

#include "rush/extra/dereferencing_iterator.hpp"
#include "rush/ast/node.hpp"

#include <vector>
#include <memory>
#include <algorithm>

namespace rush::ast {
   template <typename NodeT = ast::node, typename BaseNodeT = NodeT>
   class node_list : public BaseNodeT {
   private:
      std::vector<std::unique_ptr<NodeT>> _children;

   public:
      using BaseNodeT::attach;
      using BaseNodeT::detach;

      using iterator = decltype(make_deref_iterator(_children.begin()));
      using const_iterator = decltype(make_deref_iterator(_children.cbegin()));

      using value_type = typename iterator::value_type;
      using pointer = typename iterator::pointer;
      using reference = typename iterator::reference;
      using const_pointer = typename const_iterator::reference;
      using const_reference = typename const_iterator::reference;
      using size_type = typename std::vector<std::unique_ptr<NodeT>>::size_type;

      node_list()
      : _children {} {}

      explicit node_list(std::vector<std::unique_ptr<NodeT>> nodes)
         : _children { std::move(nodes) } {
            std::for_each(_children.begin(), _children.end(),
               [this](auto& p) { this->adopt(*p); });
         }

      bool empty() const noexcept { return _children.empty(); }
      size_type size() const noexcept { return _children.size(); }

      ptrdiff_t index_of(const_reference) const noexcept;

      reference front() noexcept { return *_children.front(); }
      reference back() noexcept { return *_children.back(); }

      const_reference front() const noexcept { return *_children.front(); }
      const_reference back() const noexcept { return *_children.back(); }

      iterator begin() noexcept { return make_deref_iterator(_children.begin()); }
      iterator end() noexcept { return make_deref_iterator(_children.end()); }

      const_iterator cbegin() const noexcept { return make_deref_iterator(_children.cbegin()); }
      const_iterator cend() const noexcept { return make_deref_iterator(_children.cend()); }

      const_iterator begin() const noexcept { return make_deref_iterator(_children.begin()); }
      const_iterator end() const noexcept { return make_deref_iterator(_children.end()); }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         std::for_each(_children.begin(), _children.end(),
            [this, &scope](auto& p) { attach(scope, *p); });
      }

      virtual void detached(ast::context&) override {
         std::for_each(_children.begin(), _children.end(),
            [this](auto& p) { detach(*p); });
      }
   };
} // rush::ast

#endif // RUSH_AST_NODE_LIST_HPP
