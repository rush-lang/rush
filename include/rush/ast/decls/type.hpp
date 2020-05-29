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

#ifndef RUSH_AST_DECLS_TYPE_HPP
#define RUSH_AST_DECLS_TYPE_HPP

#include "rush/ast/types/type.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/decls/nominal.hpp"

namespace rush::ast {
   class user_type;
   class type_declaration;

   //! \brief Wraps a type_declaration and provides convenience functions
   //!        that proxy to the declaration. This enables making for
   //!        a distinction between visitation of a type or declaration
   //!        for algorithmic purposes.
   class user_type : public type {
   public:
      user_type(ast::type_declaration const& decl)
         : _decl { decl } {}

      std::string_view name() const;

      virtual ast::type_kind kind() const final {
         return ast::type_kind::user_type;
      }

      ast::type_declaration const& declaration() const noexcept {
         return _decl;
      }

      virtual void accept(ast::visitor& v) const final {
         v.visit_user_type(*this);
      }

   private:
      ast::type_declaration const& _decl;
   };

   //! \brief Base class for declarations that are themselves types.
   //!        Such declarations include, but are not limited to:
   //!        classes, structs, and enums.
   class type_declaration
      : public nominal_declaration {
   public:
      type_declaration(std::string name)
         : _name { std::move(name) }
         , _type { *this } {}


      virtual ast::type_ref type() const noexcept final {
         return { _type };
      }

      virtual std::string_view name() const noexcept final {
         return _name;
      }

   private:
      std::string _name;
      ast::user_type _type;
   };

   inline std::string_view user_type::name() const {
      return _decl.name();
   }

} // rush::ast

#endif // RUSH_AST_DECLS_TYPE_HPP
