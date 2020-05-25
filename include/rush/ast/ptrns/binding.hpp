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

#ifndef RUSH_AST_PTRNS_BINDING_HPP
#define RUSH_AST_PTRNS_BINDING_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/exprs/expression.hpp"

#include <memory>

namespace rush::ast {
   class binding_pattern : public pattern {
   private:
      std::unique_ptr<pattern> _pattern;
      std::unique_ptr<ast::expression> _expression;
   };
}

namespace rush::ast::ptrns {
   std::unique_ptr<ast::binding_pattern> binding(
      std::unique_ptr<ast::pattern> lhs,
      std::unique_ptr<ast::expression> rhs) {
         return nullptr;
      }
}

#endif // RUSH_AST_PTRNS_BINDING_HPP
