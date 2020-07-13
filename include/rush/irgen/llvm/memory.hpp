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

#ifndef RUSH_IRGEN_LLVM_MEMORY_HPP
#define RUSH_IRGEN_LLVM_MEMORY_HPP

#include "rush/ast/decls/nominal.hpp"
#include "rush/ast/decls/storage.hpp"
#include "rush/ast/exprs/expression.hpp"

#include "llvm/IR/IRBuilder.h"

namespace rush::irgen::llvm {
   void alloc(llvm_ir_generator&, ast::storage_declaration const&);
   void store(llvm_ir_generator&, ast::nominal_declaration const&, ast::expression const&);
}

#endif // RUSH_IRGEN_LLVM_ALLOC_HPP
