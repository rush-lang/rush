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
   class list_pattern : public pattern {
   public:
      explicit list_pattern(std::vector<std::unique_ptr<pattern>> patterns)
      : _patterns { std::move(patterns) } {}

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         std::for_each(_patterns.begin(), _patterns.end(),
            [&v](auto& p) { p->accept(v); });
      }

   protected:
      virtual void attached(ast::node* parent, ast::context&) override {
         std::for_each(_patterns.begin(), _patterns.end(),
            [this, &parent](auto& p) { attach(*p, parent); });
      }

      virtual void detached(ast::node*, ast::context&) override {
         std::for_each(_patterns.begin(), _patterns.end(),
            [this](auto& p) { detach(*p); });
      }

   private:
      std::vector<std::unique_ptr<pattern>> _patterns;
   };
}

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::list_pattern> list(
      std::vector<std::unique_ptr<ast::pattern>> patterns) {
         return std::make_unique<ast::list_pattern>(std::move(patterns));
      }
}

#endif // RUSH_AST_PTRNS_LIST_HPP
