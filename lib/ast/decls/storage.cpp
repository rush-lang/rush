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
#include "rush/ast/iterator.hpp"
#include "rush/ast/decls/storage.hpp"
#include "rush/ast/ptrns/named.hpp"
#include "rush/ast/ptrns/binding.hpp"
#include "rush/ast/ptrns/type_annotation.hpp"

namespace rush::ast {

   auto storage_declaration::names() const -> decltype(ast::iterator_range<ast::named_pattern>()) {
      return ast::iterator_range<ast::named_pattern>(_ptrn.get());
   }

   auto storage_declaration::annotations() const -> decltype(ast::iterator_range<ast::type_annotation_pattern>()) {
      return ast::iterator_range<ast::type_annotation_pattern>(_ptrn.get());
   }

   auto storage_declaration::initializers() const -> decltype(ast::iterator_range<ast::binding_pattern>()) {
      return ast::iterator_range<ast::binding_pattern>(_ptrn.get());
   }
} // rush::ast
