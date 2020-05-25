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

#ifndef RUSH_AST_PATTERN_HPP
#define RUSH_AST_PATTERN_HPP

#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/decls/parameter.hpp"

#include <vector>

namespace rush::ast {
   enum class pattern_kind : std::uint8_t {
      named,
      discard,
      binding,
      destructure,
      type_annotation,
      list,
   };

   //! \brief pattern is the base for all pattern-like ways of
   //!        describing declaration identifiers; variables, parameters, etc.
   //!        Such patterns include:
   //!           named-pattern -> a
   //!           discard-pattern -> _
   //!           binding-pattern -> a = b OR a: b (where the second form is applicable to expressions only)
   //!           destructure-pattern -> {a, b, c}
   //!           type-annotation-pattern -> a: type (applicable to declarations only)
   //!           list-pattern -> a, b, c
   class pattern : public virtual node {
   public:
      // virtual pattern_kind kind() { return pattern_kind::identifier; }
   };
} // rush::ast

#endif // RUSH_AST_PATTERN_HPP
