#pragma once

#ifndef RUSH_AST_STMTS_LOOP_HPP
#define RUSH_AST_STMTS_LOOP_HPP

#include "rush/ast/stmts/statement.hpp"
#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/decls/declaration.hpp"

#include <memory>

namespace rush::ast {
   class iteration_statement : public statement {
   public:
      using node::accept;
      virtual void accept(ast::visitor& v) const override {

      }
   };
} // rush::ast


namespace rush::ast::stmts {
} // rush::ast::stmts

#endif // RUSH_AST_STMTS_LOOP_HPP
