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

#ifndef RUSH_AST_PTRNS_REST_HPP
#define RUSH_AST_PTRNS_REST_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <memory>

namespace rush::ast {
   class rest_pattern : public pattern {
   public:
      rest_pattern(
         std::unique_ptr<ast::pattern> ptrn)
         : _ptrn { std::move(ptrn) }
         { adopt(*_ptrn); }

      ast::pattern const& pattern() const noexcept {
         return *_ptrn;
      }

      virtual void accept(ast::visitor& v) const override {
         v.visit_rest_ptrn(*this);
      }

   protected:
      virtual void attached(ast::scope& scope, ast::context&) override {
         attach(scope, *_ptrn);
      }

      virtual void detached(ast::context&) override {
         detach(*_ptrn);
      }

   private:
      std::unique_ptr<ast::pattern> _ptrn;
   };
} // rush::ast

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::rest_pattern> rest(
      std::unique_ptr<ast::pattern> ptrn) {
         return std::make_unique<ast::rest_pattern>(std::move(ptrn));
      }
} // rush::ast::ptrns

#endif // RUSH_AST_PTRNS_REST_HPP
