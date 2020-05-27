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
#include "rush/ast/ptrns/named.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/destructure.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"
#include "rush/ast/types/extension.hpp"
#include "rush/ast/decls/constant.hpp"
#include "rush/ast/decls/variable.hpp"
#include "rush/ast/decls/function.hpp"
#include "rush/ast/decls/parameter.hpp"
#include "rush/ast/visitor.hpp"

using namespace rush;

class pattern_type_resolver : public ast::visitor {
public:
   pattern_type_resolver()
      : _type { ast::types::undefined } {}

   ast::type_ref result() const noexcept { return _type; }

   virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
      _type = ptrn.expression().result_type();
   }

   virtual void visit_destructure_ptrn(ast::destructure_pattern const& ptrn) override {
      // todo: find and extract types of the destructured member.
      _type = ptrn.type();
   }

   virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
      struct strip_type_extension : ast::visitor {
         ast::type_ref result;
         strip_type_extension(ast::type_ref type) : result { std::move(type) } {}
         virtual void visit_type_extension(ast::type_extension const& type)
         { result = type.underlying_type(); }
      };

      _type = rush::visit(
         ptrn.type(),
         strip_type_extension { ptrn.type() }).result;
   }

   // virtual void visit_parameter_decl(ast::parameter const&) {}
	virtual void visit_constant_decl(ast::constant_declaration const& decl) override { _type = decl.type(); }
	virtual void visit_variable_decl(ast::variable_declaration const& decl) override { _type = decl.type(); }

private:
   ast::type_ref _type;
};

class named_pattern_declaration_resolver : public ast::visitor {
public:
   named_pattern_declaration_resolver()
      : _decl { nullptr } {}

   ast::declaration const* result() const noexcept {
      return _decl;
   }

   virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
      if (ptrn.parent()) ptrn.parent()->accept(*this);
   }

   virtual void visit_destructure_ptrn(ast::destructure_pattern const& ptrn) override {
      if (ptrn.parent()) ptrn.parent()->accept(*this);
   }

   virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
      if (ptrn.parent()) ptrn.parent()->accept(*this);
   }

   // virtual void visit_parameter_decl(ast::parameter const&) {}
	virtual void visit_constant_decl(ast::constant_declaration const& decl) override { _decl = &decl; }
	virtual void visit_variable_decl(ast::variable_declaration const& decl) override { _decl = &decl; }
   virtual void visit_function_decl(ast::function_declaration const& decl) override { _decl = &decl; }

private:
   ast::declaration const* _decl;
};

namespace rush::ast {

   ast::type_ref named_pattern::resolve_type() const {
      return parent()
           ? rush::visit(*parent(), pattern_type_resolver {}).result()
           : ast::types::undeclared;
   }

   ast::declaration const* named_pattern::resolve_declaration() const {
      return parent()
           ? rush::visit(*parent(), named_pattern_declaration_resolver {}).result()
           : nullptr;
   }

   ast::type_ref destructure_pattern::resolve_type() const {
      return parent()
           ? rush::visit(*parent(), pattern_type_resolver {}).result()
           : ast::types::undeclared;
   }
} // rush::ast
