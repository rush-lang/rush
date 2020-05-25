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

#ifndef RUSH_AST_PTRNS_DISCARD_HPP
#define RUSH_AST_PTRNS_DISCARD_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/node.hpp"

#include <memory>

namespace rush::ast {
   // represents the lone underscore pattern
   // which tells the compiler that some part of the
   // over-arching pattern should be ignored or discarded.
   class discard_pattern : public pattern {
   public:
      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_discard_ptrn(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {}
      virtual void detached(ast::node*, ast::context&) override {}
   };
} // rush::ast

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::discard_pattern> discard() {
      return std::make_unique<ast::discard_pattern>();
   }
} // rush::ast::ptrns

#endif // RUSH_AST_PTRNS_DISCARD_HPP
