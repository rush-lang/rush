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

#ifndef RUSH_AST_EXPRS_LIST_HPP
#define RUSH_AST_EXPRS_LIST_HPP

#include "rush/ast/exprs/expression.hpp"

#include <vector>
#include <iterator>
#include <algorithm>

namespace rush::ast {
   class expression_list : public ast::node_list<ast::expression, ast::node> {
   public:
      using node_list<ast::expression, ast::node>::accept;
      using node_list<ast::expression, ast::node>::node_list;

      std::vector<ast::type_ref> result_types() const {
         auto types = std::vector<ast::type_ref> {};
         std::transform(this->begin(), this->end(),
            std::back_inserter(types), [](auto& x) { return x.result_type(); });
         return std::move(types);
      }

      virtual void accept(ast::visitor& v) const override {
         v.visit_expr_list(*this);
      }
   };

   namespace exprs {
      inline std::unique_ptr<ast::expression_list> list() {
         return std::make_unique<ast::expression_list>();
      }

      inline std::unique_ptr<ast::expression_list> list(std::vector<std::unique_ptr<ast::expression>> exprs) {
         return std::make_unique<ast::expression_list>(std::move(exprs));
      }
   } // exprs
} // rush::ast



#endif // RUSH_AST_EXPRS_LIST_HPP
