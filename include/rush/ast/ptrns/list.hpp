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

#ifndef RUSH_AST_PTRNS_LIST_HPP
#define RUSH_AST_PTRNS_LIST_HPP

#include "rush/ast/ptrns/pattern.hpp"

#include <memory>
#include <vector>

namespace rush::ast {
   class pattern_list : public node_list<ast::pattern> {
   public:
      using node_list<ast::pattern>::accept;
      using node_list<ast::pattern>::node_list;
      virtual void accept(ast::visitor& v) const override {
         v.visit_ptrn_list(*this);
      }
   };
}

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::pattern_list> list(
      std::vector<std::unique_ptr<ast::pattern>> patterns)
      { return std::make_unique<ast::pattern_list>(std::move(patterns)); }

   inline std::unique_ptr<ast::pattern_list> list()
   { return ptrns::list({}); }
}

#endif // RUSH_AST_PTRNS_LIST_HPP
