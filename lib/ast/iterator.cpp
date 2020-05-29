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
#include "rush/ast/types.hpp"
#include "rush/ast/patterns.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"

#include "rush/ast/visitor.hpp"
#include "rush/ast/iterator.hpp"

using namespace rush;
using namespace rush::ast;

namespace rush::ast::detail {
   class node_iterator_traversal : public ast::visitor {
   public:
      node_iterator_traversal(ast::node_iterator& it) : _it { it } {}

      virtual void visit_nil_expr(ast::nil_expression const& expr) override {
         if (_it._curr == &expr) { pop(expr); return; }
         push(expr);
      }

      virtual void visit_binary_expr(ast::binary_expression const& expr) override {
         if (_it._curr == &expr.right_operand()) { pop(expr); return; }
         if (_it._curr == &expr.left_operand()) { push(expr.right_operand()); return; }
         if (_it._curr == &expr) { push(expr.left_operand()); return; }
         push(expr);
      }

      virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
         if (_it._curr == &ptrn) { pop(ptrn); return; }
         push(ptrn);
      }

      virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
         if (_it._curr == &ptrn.expression()) { pop(ptrn); return; }
         if (_it._curr == &ptrn.pattern()) { push(ptrn.expression()); return; }
         if (_it._curr == &ptrn) { push(ptrn.pattern()); return; }
         push(ptrn);
      }

      virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
         if (_it._curr == &ptrn.pattern()) { pop(ptrn); return; }
         if (_it._curr == &ptrn) { push(ptrn.pattern()); return; }
         push(ptrn);
      }

      virtual void visit_object_destructure_ptrn(ast::object_destructure_pattern const& ptrn) override {
         if (_it._curr == &ptrn.pattern()) { pop(ptrn); return; }
         if (_it._curr == &ptrn) { push(ptrn.pattern()); return; }
         push(ptrn);
      }

      virtual void visit_array_destructure_ptrn(ast::array_destructure_pattern const& ptrn) override {
         if (_it._curr == &ptrn.pattern()) { pop(ptrn); return; }
         if (_it._curr == &ptrn) { push(ptrn.pattern()); return; }
         push(ptrn);
      }

      virtual void visit_ptrn_list(ast::pattern_list const& ptrn) override {
         if (_it._curr == ptrn.parent()) { push(ptrn); return; }
         if (_it._curr == &ptrn.back()) { pop(ptrn); return; }
         if (_it._curr == &ptrn) { push(ptrn.front()); return; }

         // fixme?: very inefficient, but without a stack I see no way to maintain
         // the current position in every list of the AST that may be traversed.
         // It might not be so bad though as I don't imagine that iteration over
         // an AST will be very lengthy. Iterators are intended for short operations
         // in sub-trees of the AST, like collecting all the names in a pattern or
         // finding a particular kind of ancestor node.
         auto prev = std::find_if(ptrn.begin(), ptrn.end(),
            [this](auto& c) { return &c == _it._curr; });
         if (prev != ptrn.end()) push(*(++prev));
         else pop(ptrn);
      }

      virtual void visit_parameter_decl(ast::parameter_declaration const& decl) override {
         if (_it._curr == &decl.pattern()) { pop(decl); return; }
         if (_it._curr == &decl) { push(decl.pattern()); return; }
         push(decl);
      }

      virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
         if (_it._curr == &decl.pattern()) { pop(decl); return; }
         if (_it._curr == &decl) { push(decl.pattern()); return; }
         push(decl);
      }

      virtual void visit_variable_decl(ast::variable_declaration const& decl) override {
         if (_it._curr == &decl.pattern()) { pop(decl); return; }
         if (_it._curr == &decl) { push(decl.pattern()); return; }
         push(decl);
      }

   private:
      ast::node_iterator& _it;

      void push(ast::node const& node) {
         _it._curr = &node;
      }

      void pop(ast::node const& node) {
         _it._curr = &node;
         if (node.parent()) {
            node.parent()->accept(*this);
         } else {
            _it._curr = nullptr;
         }
      }
   };
}

namespace rush::ast {
   ast::node_iterator& node_iterator::operator ++() {
      if (_curr != nullptr) {
         rush::visit(*_curr, detail::node_iterator_traversal { *this });
         if (_curr != nullptr) _last = _curr; }
      return *this;
   }

   ast::node_iterator& node_iterator::operator --() {
      if (_curr == nullptr) { _curr = _last; return *this; }
      // visit parent/sibling and update curr.
      return *this;
   }

   ast::node_iterator node_iterator::operator ++(int) {
      auto temp = *this;
      this->operator++();
      return temp;
   }

   ast::node_iterator node_iterator::operator --(int) {
      auto temp = *this;
      this->operator--();
      return temp;
   }

   ast::ancestor_node_iterator& ancestor_node_iterator::operator ++() {
      if (_curr != nullptr) {
         _last = _curr;
         _curr = _curr->parent(); }
      return *this;
   }

   ast::ancestor_node_iterator& ancestor_node_iterator::operator --() {
      if (_curr == nullptr) { _curr = _last; return *this; }
      // do thing.
      return *this;
   }

   ast::ancestor_node_iterator ancestor_node_iterator::operator ++(int) {
      auto temp = *this;
      this->operator++();
      return temp;
   }

   ast::ancestor_node_iterator ancestor_node_iterator::operator --(int) {
      auto temp = *this;
      this->operator--();
      return temp;
   }
} // rush::ast
