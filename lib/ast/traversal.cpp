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
#include "rush/ast/types.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/visitor.hpp"


using namespace rush;

namespace rush::ast {
   class traversal_roundtrip : public traversal {
      friend class traversal;
   protected:
      virtual void accept(ast::node const& ast) override { ast.accept(*_v); }
   private:
      ast::visitor* _v;
      traversal_roundtrip(ast::visitor* v) : _v { v } {}
   };

   void traversal::traverse(ast::node const& ast) {
      auto v = traversal_roundtrip { this };
      ast.accept(v);
   }
}
