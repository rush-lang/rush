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

#ifndef RUSH_AST_SOURCE_HPP
#define RUSH_AST_SOURCE_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/list.hpp"
#include "rush/ast/scope.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/core/source.hpp"

#include <vector>
#include <memory>


namespace rush::ast {
   class source_node : public ast::node_list<ast::node> {
   public:

      source_node(rush::source const& src)
         : ast::node_list<ast::node> {}
         , _src { src } {}

      source_node(rush::source const& src, std::vector<std::unique_ptr<ast::node>> nodes)
         : ast::node_list<ast::node> { std::move(nodes) }
         , _src { src } {}

      std::error_code error() const { return _src.error(); }

      //! \brief Returns the identifier of the source; typically the file path.
      std::string_view id() const { return _src.id(); }

      //! \brief Returns a view over the source buffer; typically the contents of a file.
      std::string_view buffer() const { return _src.buffer(); }

      //! \brief Returns the length of the source in bytes; typically the size of the file.
      std::size_t size() const { return _src.size(); }

      //! \brief Returns the length of the source in bytes; typically the size of the file.
      std::size_t length() const { return _src.length(); }

      using node_list<ast::node>::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_source(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context& ctx) override;
      virtual void detached(ast::context& ctx) override;

   private:
      rush::source const& _src;
   };

   inline std::unique_ptr<ast::source_node> source(rush::source const& src) {
      return std::make_unique<ast::source_node>(src);
   }

   inline std::unique_ptr<ast::source_node> source(rush::source const& src, std::vector<std::unique_ptr<ast::node>> nodes) {
      return std::make_unique<ast::source_node>(src, std::move(nodes));
   }
} // rush::ast

#endif // RUSH_AST_SOURCE_HPP
