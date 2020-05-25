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

#ifndef RUSH_AST_PTRNS_DESTRUCTURE_HPP
#define RUSH_AST_PTRNS_DESTRUCTURE_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"

#include <memory>

namespace rush::ast {
   class destructure_pattern : public ast::pattern {
   public:
      destructure_pattern(std::unique_ptr<ast::pattern> patt)
         : _pattern { std::move(patt) }
         , _type { ast::types::undefined } {}

      ast::type_ref type() const noexcept {
         return _type == ast::types::undefined
              ? _type = resolve_type()
              : _type;
      }

      ast::pattern const& pattern() const noexcept {
         return *_pattern;
      }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_destructure_ptrn(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_pattern);
      }

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_pattern);
      }

   private:
      std::unique_ptr<ast::pattern> _pattern;
      mutable ast::type_ref _type;

      ast::type_ref resolve_type() const;
   };
} // rush::ast

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::destructure_pattern> destructure(
      std::unique_ptr<ast::pattern> patt) {
         return std::make_unique<ast::destructure_pattern>(std::move(patt));
      }
} // rush::ast::ptrns

#endif // RUSH_AST_PTRNS_DESTRUCTURE_HPP
