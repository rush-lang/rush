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
#include "rush/ast/patterns.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"

#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"

#include "rush/ast/visitor.hpp"
#include "rush/ast/iterator.hpp"

using namespace rush;
using namespace rush::ast;

namespace rush::ast::detail {
   class node_iterator_traversal : public ast::visitor {
   public:
      node_iterator_traversal(ast::node_iterator& it) : _it { it } {}

      virtual void visit_module(ast::module_node const& mdl) override {
         visit_node_list(mdl);
      }

      virtual void visit_source(ast::source_node const& src) override {
         visit_node_list(src);
      }

      virtual void visit_nil_expr(ast::nil_expression const& expr) override {
         if (_it._curr == &expr) { pop(expr); return; }
         push(expr);
      }

      virtual void visit_unary_expr(ast::unary_expression const& expr) override {
         if (_it._curr == &expr.operand()) { pop(expr); return; }
         if (_it._curr == &expr) { push(expr.operand()); return; }
         push(expr);
      }

      virtual void visit_binary_expr(ast::binary_expression const& expr) override {
         if (_it._curr == &expr.right_operand()) { pop(expr); return; }
         if (_it._curr == &expr.left_operand()) { push(expr.right_operand()); return; }
         if (_it._curr == &expr) { push(expr.left_operand()); return; }
         push(expr);
      }

      virtual void visit_expr_list(ast::expression_list const& expr) override {
         visit_node_list(expr);
      }

      virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
         if (_it._curr == &ptrn) { pop(ptrn); return; }
         push(ptrn);
      }

      virtual void visit_discard_ptrn(ast::discard_pattern const& ptrn) override {
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
         visit_node_list(ptrn);
      }

      virtual void visit_member_section_decl(ast::member_section_declaration const& decl) override {
         if (_it._curr == &decl) { pop(decl); return; }
         push(decl);
      }

      virtual void visit_field_decl(ast::field_declaration const& decl) override {
         if (_it._curr == &decl.storage()) { pop(decl); return; }
         if (_it._curr == &decl) { push(decl.storage()); return; }
         push(decl);
      }

      virtual void visit_method_decl(ast::method_declaration const& decl) override {
         if (_it._curr == &decl) { pop(decl); return; }
         push(decl);
      }

      virtual void visit_parameter_decl(ast::parameter_declaration const& decl) override {
         if (_it._curr == &decl.pattern()) { pop(decl); return; }
         if (_it._curr == &decl) { push(decl.pattern()); return; }
         push(decl);
      }

      virtual void visit_storage_decl(ast::storage_declaration const& decl) override {
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
         if (node.parent())
         {
            if (_it._curr != _it._end)
               node.parent()->accept(*this);
         } else { _it._curr = nullptr; }
      }

      template <typename NodeT, typename BaseNodeT>
      void visit_node_list(ast::node_list<NodeT, BaseNodeT> const& list) {
         if (_it._curr == list.parent()) { push(list); return; }
         if (_it._curr == &list.back()) { pop(list); return; }
         if (_it._curr == &list) { push(list.front()); return; }

         // fixme?: very inefficient, but without a stack I see no way to maintain
         // the current position in every list of the AST that may be traversed.
         // It might not be so bad though as I don't imagine that iteration over
         // an AST will be very lengthy. Iterators are intended for short operations
         // in sub-trees of the AST, like collecting all the names in a pattern or
         // finding a particular kind of ancestor node.
         auto prev = std::find_if(list.begin(), list.end(),
            [this](auto& c) { return &c == _it._curr; });
         if (prev != list.end()) push(*(++prev));
         else pop(list);
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
