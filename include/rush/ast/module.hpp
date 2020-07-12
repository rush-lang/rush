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

#ifndef RUSH_AST_MODULE_HPP
#define RUSH_AST_MODULE_HPP

#include "rush/ast/source.hpp"
#include "rush/ast/scope.hpp"

#include <vector>

namespace rush::ast {
   class module_node : public ast::node_list<ast::source_node, ast::node> {
   public:
      module_node(std::string name)
         : ast::node_list<ast::source_node, ast::node> {}
         , _name { std::move(name) } {}

      module_node(std::string name, std::vector<std::unique_ptr<ast::source_node>> srcs)
         : ast::node_list<ast::source_node, ast::node> { std::move(srcs) }
         , _name { std::move(name) } {}

      std::string_view name() const { return _name; }

      using node_list<ast::source_node, ast::node>::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_module(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context& ctx) override;
      virtual void detached(ast::context& ctx) override;

   private:
      std::string _name;
   };

   inline std::unique_ptr<ast::module_node> module_(std::string name) {
      return std::make_unique<ast::module_node>(std::move(name));
   }

   inline std::unique_ptr<ast::module_node> module_(std::string name, std::vector<std::unique_ptr<ast::source_node>> srcs) {
      return std::make_unique<ast::module_node>(std::move(name), std::move(srcs));
   }
} // rush::ast

#endif // RUSH_AST_MODULE_HPP
