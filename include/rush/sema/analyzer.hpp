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

#ifndef RUSH_SEMA_ANALYZER_HPP
#define RUSH_SEMA_ANALYZER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"

#include "rush/diag/semantic_error.hpp"
#include "rush/diag/semantic_warning.hpp"

#include <vector>
#include <memory>

namespace rush::sema {
   class engine;

   class analyzer : public virtual ast::visitor {
      friend class engine;

      std::vector<std::unique_ptr<rush::diagnostic>> results() {
         return std::move(_diags);
      }

   protected:
      void log(std::unique_ptr<rush::diagnostic>);
      void run(analyzer&, ast::node const&);

   private:
      std::vector<std::unique_ptr<rush::diagnostic>> _diags;

      void initialize(sema::engine&);
   };
}

#endif // RUSH_SEMA_ANALYZER_HPP
