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

#ifndef RUSH_AST_STMTS_LOOP_HPP
#define RUSH_AST_STMTS_LOOP_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <memory>

namespace rush::ast {
   class iteration_statement : public statement {
   public:
      using node::accept;
      virtual void accept(ast::visitor& v) const override {

      }

      virtual void attach(ast::node&, ast::context& context) override {}
      virtual void detach(ast::node&, ast::context& context) override {}
   };
} // rush::ast


namespace rush::ast::stmts {
   // inline std::unique_ptr<iteration_statement> for_(
   //    std::unique_ptr<expression> collection,
   //    std::unique_ptr<statement> do) {
   //       return std::make_unique<iteration_statement>(
   //          statement_kind::if_,
   //          std::move(cond),
   //          std::move(then),
   //          iteration_statement::factory_tag_t {});
   //    }

   // inline std::unique_ptr<iteration_statement> for_(
   //    std::unique_ptr<expression> ident,
   //    std::unique_ptr<expression> then,
   //    std::unique_ptr<statement> do) {
   //       return std::make_unique<iteration_statement>(
   //          state
   //       )
   //    }
} // rush::ast::stmts

#endif // RUSH_AST_STMTS_LOOP_HPP
