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

#ifndef RUSH_AST_DECLS_DECLARATION_HPP
#define RUSH_AST_DECLS_DECLARATION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <cstdint>

namespace rush::ast {
	enum class declaration_kind : std::uint8_t {
      undefined,
      undeclared,
      module,
		constant,
		variable,
      parameter,
		function,
		alias,
		enum_,
		class_,
		struct_,
		concept,
		interface,
		extension,
		operator_,
	};

	class declaration : public virtual node {
		declaration(declaration const&) = delete;
		void operator = (declaration const&) = delete;

	public:
      virtual declaration_kind kind() const = 0;

   protected:
      declaration() = default;
	};

   class typed_declaration : public declaration {
   public:
       virtual ast::type_ref type() const = 0;

   protected:
      typed_declaration() = default;
   };
}

#endif // RUSH_AST_DECLS_DECLARATION_HPP
