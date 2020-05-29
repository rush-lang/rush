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

#ifndef RUSH_AST_ITERATOR_HPP
#define RUSH_AST_ITERATOR_HPP

#include "rush/ast/node.hpp"
#include "rush/extra/iterator_range.hpp"

#include <cstdlib>
#include <iterator>

namespace rush::ast {
   class node_iterator;
   class ancestor_node_iterator;

   template <typename NodeT> class typed_node_iterator;
   template <typename NodeT> class typed_ancestor_node_iterator;

   namespace detail {
      class node_iterator_traversal;
   }

   class node_iterator : public std::iterator<std::forward_iterator_tag, ast::node> {
      friend class detail::node_iterator_traversal;

   public:
      explicit node_iterator(ast::node const* n = nullptr)
         : _curr { n }
         , _last { n } {}

      ast::node const& operator *() const noexcept { return *_curr; }
      ast::node const* operator ->() const noexcept { return _curr; }

      ast::node_iterator& operator ++();
      ast::node_iterator& operator --();

      ast::node_iterator operator ++(int);
      ast::node_iterator operator --(int);

      friend bool operator == (
         ast::node_iterator const& lhs,
         ast::node_iterator const& rhs) {
            return lhs._curr == rhs._curr;
         }

      friend bool operator != (
         ast::node_iterator const& lhs,
         ast::node_iterator const& rhs) {
            return !(lhs == rhs);
         }

      friend bool operator == (
         ast::node const* lhs,
         ast::node_iterator const& rhs) {
            return lhs == rhs._curr;
         }

      friend bool operator != (
         ast::node const* lhs,
         ast::node_iterator const& rhs) {
            return !(lhs == rhs);
         }

      friend bool operator == (
         ast::node_iterator const& lhs,
         ast::node const* rhs) {
            return lhs._curr == rhs;
         }

      friend bool operator != (
         ast::node_iterator const& lhs,
         ast::node const* rhs) {
            return !(lhs == rhs);
         }

   private:
      ast::node const* _curr;
      ast::node const* _last;
   };

   class ancestor_node_iterator {
   public:
      explicit ancestor_node_iterator(ast::node const* n = nullptr)
         : _curr { n }
         , _last { n } {}

      ast::node const& operator *() const noexcept { return *_curr; }
      ast::node const* operator ->() const noexcept { return _curr; }

      ast::ancestor_node_iterator& operator ++();
      ast::ancestor_node_iterator& operator --();

      ast::ancestor_node_iterator operator ++(int);
      ast::ancestor_node_iterator operator --(int);

      friend bool operator == (ast::ancestor_node_iterator const& lhs, ast::ancestor_node_iterator const& rhs) {
         return lhs._curr == rhs._curr;
      }

      friend bool operator != (ast::ancestor_node_iterator const& lhs, ast::ancestor_node_iterator const& rhs) {
         return !(lhs == rhs);
      }

   private:
      ast::node const* _curr;
      ast::node const* _last;
   };

   template <typename NodeT>
   class typed_node_iterator : public std::iterator<std::forward_iterator_tag, NodeT> {
      template <typename NodeU>
      friend class typed_node_iterator;

   public:
      typed_node_iterator()
         : _it { node_iterator {} } {}

      explicit typed_node_iterator(ast::node_iterator it)
         : _it { it } {}

      NodeT const& operator *() const noexcept { return dynamic_cast<NodeT const&>(_it.operator *()); }
      NodeT const* operator ->() const noexcept { return dynamic_cast<NodeT const*>(_it.operator ->()); }

      ast::typed_node_iterator<NodeT>& operator ++() {
         while ((_it.operator ->() != nullptr)
            && (!dynamic_cast<NodeT const*>((++_it).operator ->())));
         return *this;
      }

      ast::typed_node_iterator<NodeT>& operator --() {
         while ((_it.operator ->() != nullptr)
            && (!dynamic_cast<NodeT const*>((--_it).operator ->())));
         return *this;
      }

      ast::typed_node_iterator<NodeT> operator ++(int) {
         auto temp = *this;
         this->operator ++();
         return temp;
      }

      ast::typed_node_iterator<NodeT> operator --(int) {
         auto temp = *this;
         this->operator --();
         return temp;
      }

      template<typename NodeU>
      friend bool operator == (typed_node_iterator<NodeT> const& lhs, typed_node_iterator<NodeU> const& rhs)
      { return lhs._it == rhs._it; }

      template<typename NodeU>
      friend bool operator != (typed_node_iterator<NodeT> const& lhs, typed_node_iterator<NodeU> const& rhs)
      { return lhs._it != rhs._it; }

      friend bool operator == (node_iterator const& lhs, typed_node_iterator<NodeT> const& rhs) { return lhs == rhs._it; }
      friend bool operator != (node_iterator const& lhs, typed_node_iterator<NodeT> const& rhs) { return lhs != rhs._it; }

      friend bool operator == (typed_node_iterator<NodeT> const& lhs, node_iterator const& rhs) { return rhs == lhs; }
      friend bool operator != (typed_node_iterator<NodeT> const& lhs, node_iterator const& rhs) { return rhs != lhs; }

   private:
      ast::node_iterator _it;
   };

   template <typename NodeT>
   class typed_ancestor_node_iterator {
      template <typename NodeU>
      friend class typed_ancestor_node_iterator;

   public:
      typed_ancestor_node_iterator()
         : _it { ancestor_node_iterator {} } {}

      explicit typed_ancestor_node_iterator(ast::ancestor_node_iterator it)
         : _it { it } {}

      NodeT const& operator *() const noexcept { return dynamic_cast<NodeT const&>(_it.operator *()); }
      NodeT const* operator ->() const noexcept { return dynamic_cast<NodeT const*>(_it.operator ->()); }

      ast::typed_ancestor_node_iterator<NodeT>& operator ++() {
         while ((_it.operator ->() != nullptr)
            && (!dynamic_cast<NodeT const*>((++_it).operator ->())));
         return *this;
      }

      ast::typed_ancestor_node_iterator<NodeT>& operator --() {
         while ((_it.operator ->() != nullptr)
            && (!dynamic_cast<NodeT const*>((--_it).operator ->())));
         return *this;
      }

      ast::typed_ancestor_node_iterator<NodeT> operator ++(int) {
         auto temp = *this;
         this->operator ++();
         return temp;
      }

      ast::typed_ancestor_node_iterator<NodeT> operator --(int) {
         auto temp = *this;
         this->operator --();
         return temp;
      }

      template<typename NodeU>
      friend bool operator == (typed_ancestor_node_iterator<NodeT> const& lhs, typed_ancestor_node_iterator<NodeU> const& rhs)
      { return lhs._it == rhs._it; }

      template<typename NodeU>
      friend bool operator != (typed_ancestor_node_iterator<NodeT> const& lhs, typed_ancestor_node_iterator<NodeU> const& rhs)
      { return lhs._it != rhs._it; }

      friend bool operator == (ancestor_node_iterator const& lhs, typed_ancestor_node_iterator<NodeT> const& rhs) { return lhs == rhs._it; }
      friend bool operator != (ancestor_node_iterator const& lhs, typed_ancestor_node_iterator<NodeT> const& rhs) { return lhs != rhs._it; }

      friend bool operator == (typed_ancestor_node_iterator<NodeT> const& lhs, ancestor_node_iterator const& rhs) { return rhs == lhs; }
      friend bool operator != (typed_ancestor_node_iterator<NodeT> const& lhs, ancestor_node_iterator const& rhs) { return rhs != lhs; }

   private:
      ast::ancestor_node_iterator _it;
   };


   inline node_iterator iterator(ast::node const* node = nullptr)
   { return node_iterator { node }; }

   inline ancestor_node_iterator ancestor_iterator(ast::node const* node = nullptr)
   { return ancestor_node_iterator { node }; }

   template <typename NodeT>
   inline typed_node_iterator<NodeT> iterator()
   { return typed_node_iterator<NodeT> { iterator() }; }

   template <typename NodeT>
   inline typed_ancestor_node_iterator<NodeT> ancestor_iterator()
   { return typed_ancestor_node_iterator<NodeT> { ancestor_iterator() }; }

   template <typename NodeT>
   inline typed_node_iterator<NodeT> find_child(ast::node const* node = nullptr) {
      auto it = typed_node_iterator<NodeT> { iterator(node) };
      if (node != nullptr && !dynamic_cast<NodeT const*>(node)) ++it;
      return it;
   }

   template <typename NodeT>
   inline typed_ancestor_node_iterator<NodeT> find_ancestor(ast::node const* node = nullptr) {
      auto it = typed_ancestor_node_iterator<NodeT> { ancestor_iterator(node) };
      if (node != nullptr && !dynamic_cast<NodeT const*>(node)) ++it;
      return it;
   }

   inline rush::iterator_range<node_iterator> iterator_range(ast::node const* node = nullptr)
   { return { iterator(node), node ? iterator(node->parent()) : iterator() }; }

   template <typename NodeT>
   inline rush::iterator_range<typed_node_iterator<NodeT>> iterator_range(ast::node const* node = nullptr)
   { return { find_child<NodeT>(node), node ? typed_node_iterator<NodeT> { iterator(node->parent()) } : iterator<NodeT>() }; }
} // rush::ast

#endif // RUSH_AST_ITERATOR_HPP
