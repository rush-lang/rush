#pragma once

#ifndef RUSH_AST_EXPRS_CALL_HPP
#define RUSH_AST_EXPRS_CALL_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/builtin.hpp"

#include <string>
#include <memory>

namespace rush::ast {
	class call_expression : public expression {
   public:
      virtual ast::type_ref result_type() const override {
         return types::error_type;
      }
   };

	namespace exprs {
		std::unique_ptr<call_expression> call(std::string name);
	}
}

#endif // RUSH_AST_EXPRS_CALL_HPP
