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

#ifndef RUSH_AST_PTRNS_NAMED_HPP
#define RUSH_AST_PTRNS_NAMED_HPP

#include "rush/ast/ptrns/pattern.hpp"
#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_ref.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace rush::ast {
   class named_pattern :
      public pattern,
      public nominal_declaration {
   public:
      explicit named_pattern(std::string name)
         : _name { std::move(name) }
         , _type { ast::types::undefined }
         , _decl { nullptr }
         , _resolving_type { false } {}

      //! \brief Gets the resolved type of the named pattern
      //!        obtained by walking the patterns ancestors until the root
      //!        declaration has been reached.
      virtual ast::type_ref type() const noexcept override {
         return (_type == ast::types::undefined)
              ? _type = resolve_type()
              : _type;
      }

      //! \brief Gets the root declaration this named pattern is bound to.
      //!        Possible roots include but are not limited to variable,
      //!        constant, or parameter declarations.
      ast::declaration const& declaration() const noexcept {
         // resolve the root declaration by traversing the parent
         // patterns until an ancestral declaration has been found.
         return (_decl == nullptr)
              ? *(_decl = resolve_declaration())
              : *(_decl);
      }

      //! \brief Gets the designated name of the pattern,
      //!        assigned at construction.
      virtual std::string_view name() const noexcept override { return _name; }

      //! \brief Gets the kind of declaration this named pattern is bound to.
      //!        Possible kinds include but are not limited to variable,
      //!        constant, or parameter declarations.
      virtual ast::declaration_kind kind() const noexcept override { return declaration().kind(); }

      using node::accept;
      virtual void accept(ast::visitor& v) const override {
         v.visit_named_ptrn(*this);
      }

   protected:
      virtual void attached(ast::node*, ast::context&) override {}
      virtual void detached(ast::node*, ast::context&) override {
         _type = ast::types::undefined;
         _decl = nullptr;
      }

   private:
      std::string _name;
      mutable ast::declaration const* _decl;
      mutable ast::type_ref _type;
      mutable bool _resolving_type;

      ast::type_ref resolve_type() const;
      ast::declaration const* resolve_declaration() const;
   };
}

namespace rush::ast::ptrns {
   inline std::unique_ptr<ast::named_pattern> name(std::string name) {
      return std::make_unique<ast::named_pattern>(std::move(name));
   }
}

#endif // RUSH_AST_PTRNS_NAMED_HPP
