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
#include "rush/sema/engine.hpp"
#include "rush/sema/analyzer.hpp"

namespace rush::sema {
   void analyzer::log(std::unique_ptr<rush::diagnostic> diag) {
      _diags.push_back(std::move(diag));
   }

   void analyzer::run(analyzer& a, ast::node const& n) {
      auto results = rush::visit(n, a).results();
      std::move(
         results.begin(),
         results.end(),
         std::back_inserter(_diags));
   }

   void analyzer::initialize(sema::engine&) {
   }
}
