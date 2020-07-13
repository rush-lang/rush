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
#include "rush/irgen/llvm.hpp"
#include "rush/irgen/llvm/types.hpp"
#include "rush/irgen/llvm/expressions.hpp"
#include "rush/irgen/llvm/generator.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/declarations.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>

using namespace rush;

struct ::irgen::irgenerator_result::impl {
   std::unique_ptr<::llvm::LLVMContext> context;
   std::unique_ptr<::llvm::Module> module;
};


class llvm_irgen_context {

};

class llvm_ir_storage_generator : public irgen::llvm_ir_generator<llvm::Value> {
public:
   virtual void visit_constant_decl(ast::constant_declaration const& decl) {

   }
};


class llvm_ir_function_generator : public irgen::llvm_ir_generator<llvm::Function> {
private:
   ::llvm::Function* _function;
   ::llvm::BasicBlock* _entry;

   struct parameter_binder : public ast::traversal {
   private:
      llvm::Function::arg_iterator _first;
      llvm::Function::arg_iterator _last;
      llvm_ir_function_generator& _gen;
      llvm::IRBuilder<> _builder;
   public:

      parameter_binder(
         llvm_ir_function_generator& gen,
         llvm::iterator_range<llvm::Function::arg_iterator> args)
         : _first { args.begin() }
         , _last { args.end() }
         , _gen { gen }
         , _builder {
            &gen._function->getEntryBlock(),
            gen._function->getEntryBlock().begin() } {}

      virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
         if (_first != _last) {
            _first->setName(std::string { ptrn.name() });
            auto type = _gen.generate(ptrn.type(), irgen::llvm_ir_type_generator {}).result();
            auto value = _builder.CreateAlloca(type, 0, std::string { ptrn.name() });
            _gen.insert(ptrn, value);
            ++_first;
         }
      }

      virtual void visit_object_destructure_ptrn(ast::object_destructure_pattern const& ptrn) override {
         if (_first != _last) {
            _first->setName("$obj-destr");
            ++_first;
         }
      }

      virtual void visit_array_destructure_ptrn(ast::array_destructure_pattern const& ptrn) override {
         if (_first != _last) {
            _first->setName("$arr-destr");
            ++_first;
         }
      }
   };

public:
   llvm_ir_function_generator()
      : _function { nullptr }
      , _entry { nullptr } {}

   virtual void visit_function_decl(ast::function_declaration const& decl) override {
      auto type = (llvm::FunctionType*)generate(
         decl.type(), irgen::llvm_ir_type_generator {})
         .result();

      _function = llvm::Function::Create(type,
         llvm::Function::ExternalLinkage,
         std::string { decl.name() },
         module_());

      _entry = llvm::BasicBlock::Create(context(), "entry", _function);
      builder().SetInsertPoint(_entry);

      insert(decl, _function);
      rush::visit(decl.parameters(),
         parameter_binder { *this, _function->args() });
      decl.body().accept(*this);
   }

   virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
      struct named_pattern_extractor : public ast::traversal {
         std::vector<ast::named_pattern const*> names;
         virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
            names.push_back(&ptrn);
         }
      };

      struct initializer : public llvm_ir_generator<llvm::Value> {
         std::vector<ast::named_pattern const*> names;
         virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
            names.push_back(&ptrn);
         }

         virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
            ptrn.pattern().accept(*this);
            auto value = generate(ptrn.expression(), irgen::llvm_ir_expression_generator {}).result();
            for (auto name : names) { builder().CreateStore(lookup(*name), value); }
            names.clear();
         }
      };

      auto ptrns = std::move(rush::visit(decl.pattern(), named_pattern_extractor {}).names);
      for (auto ptrn : ptrns) {
         auto type = generate(ptrn->type(), irgen::llvm_ir_type_generator {}).result();
         auto value = builder().CreateAlloca(type, 0, std::string { ptrn->name() });
         insert(*ptrn, value);
      }

      generate(decl.pattern(), initializer {});
   }

   virtual void visit_return_stmt(ast::result_statement const& stmt) override {
      builder().CreateRet(generate(stmt.expression(), irgen::llvm_ir_expression_generator {}).result());
   }

   virtual void visit_block_stmt(ast::statement_block const& stmt) override {
      traverse(stmt);
   }
};

class llvm_ir_module_generator : public irgen::llvm_ir_generator<llvm::Module> {
   using value_map_type = std::unordered_map<ast::nominal_declaration const*, llvm::Value*>;
private:
   std::unique_ptr<::llvm::Module> _module;
   ::llvm::IRBuilder<> _builder;
   ::llvm::LLVMContext& _context;
   value_map_type _values;

public:
   llvm_ir_module_generator(::llvm::LLVMContext& ctx)
      : _module { nullptr }
      , _context { ctx }
      , _builder { ctx }
      , _values { } {}

   virtual llvm::Module* result() const override {
      return _module.get();
   }

   virtual std::unique_ptr<::llvm::Module> module_() {
      return std::move(_module);
   }

   virtual void visit_module(ast::module_node const& mdl) override {
      _module = std::make_unique<::llvm::Module>(std::string { mdl.name() }, _context);
      traverse(mdl);
   }

   virtual void visit_source(ast::source_node const& src) override {
      if (_module) {
         _module->setSourceFileName(std::string { src.id() });
         initialize(*_module, _context, _builder, _values);
         traverse(src);
      }
   }

   virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
      // rush::visit(decl, llvm_or_global_constant_generator { _context, *_module, _builder });
   }

   virtual void visit_variable_decl(ast::variable_declaration const& decl) override {
      // rush::visit(decl, llvm_or_global_variable_generator { _context, *_module, _builder });
   }

   virtual void visit_function_decl(ast::function_declaration const& decl) override {
      generate(decl, llvm_ir_function_generator {});
   }
};

namespace rush::irgen {
   irgenerator_result::~irgenerator_result() = default;
   irgenerator_result::irgenerator_result(std::unique_ptr<impl> pimpl)
      : _pimpl { std::move(pimpl) } {}

   void irgenerator_result::dump() {
      _pimpl->module->print(::llvm::outs(), nullptr);
   }

   irgenerator_result llvm(rush::syntax_analysis const& ast) {
      auto pimpl = std::make_unique<irgenerator_result::impl>();
      pimpl->context = std::make_unique<::llvm::LLVMContext>();
      pimpl->module = rush::visit(ast, llvm_ir_module_generator { *pimpl->context }).module_();
      return { std::move(pimpl) };
   }
}
