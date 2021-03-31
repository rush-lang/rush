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
#include "rush/irgen/llvm/statements.hpp"
#include "rush/irgen/llvm/expressions.hpp"
#include "rush/irgen/llvm/declarations.hpp"

namespace rush::irgen::llvm {
   void llvm_ir_statement_generator::visit_if_stmt(ast::conditional_statement const& stmt) {
      struct conditional_branch_generator : public llvm_ir_generator<::llvm::Value> {
      private:
         ::llvm::Value* _cond;
         ::llvm::Value* _result;

      public:
         conditional_branch_generator(::llvm::Value* cond)
            : _cond { cond }
            , _result { nullptr } {}

         virtual ::llvm::Value* result() const override {
            return _result;
         }

         virtual void visit_block_stmt(ast::statement_block const& stmt) override {
            auto func = builder().GetInsertBlock()->getParent();

            auto then = ::llvm::BasicBlock::Create(context(), "then", func);
            auto end = ::llvm::BasicBlock::Create(context(), "endif", func);
            builder().CreateCondBr(_cond, then, end);

            // Then
            builder().SetInsertPoint(then);
            auto gen = llvm_ir_statement_generator {};
            auto body = generate(stmt, gen).result();
            if (body) return;

            then = builder().GetInsertBlock();

            // Emit merge block.
            func->getBasicBlockList().push_back(then);
            builder().SetInsertPoint(end);
            ::llvm::PHINode* pn =
               builder().CreatePHI(::llvm::Type::getDoubleTy(context()), 2, "iftmp");

            pn->addIncoming(body, then);
            _result = pn;

            builder().SetInsertPoint(end);
         }

         virtual void visit_alternating_stmt(ast::alternating_statement const& stmt) override {
         }
      };

      auto cond = generate(stmt.condition(), llvm_ir_expression_generator {}).result();
      _result = generate(stmt.body(), conditional_branch_generator { cond }).result();
   }

   void llvm_ir_statement_generator::visit_storage_decl(ast::storage_declaration const& decl) {
      _result = generate(decl, llvm_ir_declaration_generator {}).result();
   }

   void llvm_ir_statement_generator::visit_return_stmt(ast::result_statement const& stmt) {
      _result = builder().CreateRet(generate(stmt.expression(), irgen::llvm::llvm_ir_expression_generator {}).result());
   }
} // rush::irgen
