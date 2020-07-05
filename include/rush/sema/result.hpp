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

#ifndef RUSH_SEMA_RESULT_HPP
#define RUSH_SEMA_RESULT_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/extra/iterator_range.hpp"
#include "rush/extra/dereferencing_iterator.hpp"

#include "rush/ast/node.hpp"

#include <vector>

namespace rush {
   namespace sema { class engine; }

   class semantic_analysis {
      friend class sema::engine;

   public:
      auto const& ast() const noexcept {
         return *_ast;
      }

      auto diagnostics() const noexcept {
         return rush::make_iterator_range(
            rush::make_deref_iterator(_diags.begin()),
            rush::make_deref_iterator(_diags.end()));
      }

   private:
      std::unique_ptr<ast::node> _ast;
      std::vector<std::unique_ptr<rush::diagnostic>> _diags;

      semantic_analysis(
         std::unique_ptr<ast::node> ast,
         std::vector<std::unique_ptr<rush::diagnostic>> diags)
         : _ast { std::move(ast) }
         , _diags { std::move(diags) } {}
   };
} // rush

#endif // RUSH_SEMA_RESULT_HPP
