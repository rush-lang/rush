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

#ifndef RUSH_AST_PTRNS_TYPE_ANNOTATION_HPP
#define RUSH_AST_PTRNS_TYPE_ANNOTATION_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <memory>

namespace rush::ast {
   class type_annotation_pattern : public pattern {
   public:
      type_annotation_pattern(
         std::unique_ptr<ast::pattern> ptrn,
         ast::type_ref type)
         : _ptrn { std::move(ptrn) }
         , _type { std::move(type) } {}

      ast::type_ref type() const noexcept {
         return _type;
      }

      ast::pattern const& pattern() const noexcept {
         return *_ptrn;
      }

      virtual void accept(ast::visitor& v) const override {
         v.visit_type_annotation_ptrn(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {
         attach(*_ptrn);
      };

      virtual void detached(ast::node*, ast::context&) override {
         detach(*_ptrn);
      };

   private:
      ast::type_ref _type;
      std::unique_ptr<ast::pattern> _ptrn;
   };
} // rush::ast

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::type_annotation_pattern> annotation(
      std::unique_ptr<ast::pattern> ptrn,
      ast::type_ref type) {
         return std::make_unique<ast::type_annotation_pattern>(
            std::move(ptrn),
            std::move(type));
      }
} // rush::ast::ptrns

#endif // RUSH_AST_PTRNS_TYPE_ANNOTATION_HPP
