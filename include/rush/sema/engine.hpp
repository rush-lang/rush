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

#ifndef RUSH_SEMA_ENGINE_HPP
#define RUSH_SEMA_ENGINE_HPP

#include "rush/parser/result.hpp"
#include "rush/sema/analyzer.hpp"
#include "rush/sema/result.hpp"

namespace rush::sema {
   /*! \brief The Semantic Analysis Engine component is responsible
    *         for running semantic analyzers against an AST,
    *         and producing a semantic analysis result object.
    */
   class engine {
   public:
      rush::semantic_analysis analyze(rush::syntax_analysis const&);
   };
}

#endif // RUSH_SEMA_ENGINE_HPP
