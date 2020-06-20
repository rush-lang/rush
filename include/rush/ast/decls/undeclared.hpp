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

#ifndef RUSH_DECLS_UNDECLARED_HPP
#define RUSH_DECLS_UNDECLARED_HPP


#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/nominal.hpp"

namespace rush::ast {

   //! \brief Sentinel type that is used as a substitute for
   //         identifiers that were not resolved after parsing,
   //         and the scope chain has been destroyed.
   class undeclared_identifier : public ast::nominal_declaration {
   public:
      undeclared_identifier(std::string name)
         : _name { std::move(name) } {}

      virtual std::string_view name() const override {
         return _name;
      };

      virtual ast::type_ref type() const override {
         return ast::types::undeclared;
      }

      virtual ast::declaration_kind kind() const override {
         return ast::declaration_kind::undeclared;
      }

      using node::accept;
      virtual void accept(ast::visitor&) const override {};

   protected:
      virtual void attached(ast::scope&, ast::context&) override {};
      virtual void detached(ast::context&) override {};

   private:
      std::string _name;
   };
}

#endif // RUSH_DECLS_UNDECLARED_HPP
