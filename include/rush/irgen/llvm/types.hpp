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

#ifndef RUSH_IRGEN_LLVM_TYPES_HPP
#define RUSH_IRGEN_LLVM_TYPES_HPP

#include "rush/ast/visitor.hpp"
#include "rush/ast/types.hpp"
#include "rush/irgen/llvm/generator.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

namespace rush::irgen {
   class llvm_ir_type_generator : public llvm_ir_generator<llvm::Type> {
   public:
      llvm_ir_type_generator()
         : _result { nullptr } {}

      virtual llvm::Type* result() const override { return _result; }

      virtual void visit_builtin_void_type(ast::builtin_void_type const&) override;
      virtual void visit_builtin_bool_type(ast::builtin_bool_type const&) override;
      virtual void visit_builtin_integral_type(ast::builtin_integral_type const&) override;
      virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const&) override;

      virtual void visit_function_type(ast::function_type const&) override;

   private:
      llvm::Type* _result;
   };
} // rush::irgen

#endif // RUSH_IRGEN_LLVM_TYPES_HPP
