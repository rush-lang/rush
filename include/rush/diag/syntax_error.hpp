#pragma once

#ifndef RUSH_DIAG_SYNTAX_ERROR_HPP
#define RUSH_DIAG_SYNTAX_ERROR_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/diag/visitor.hpp"
#include "rush/lexer/token.hpp"

#include <string>

namespace rush::diag {
   class syntax_error : public diagnostic {
   public:
      using code_type = diagnostic::code_type;
      syntax_error(code_type code, std::string msg)
         : diagnostic { code, std::move(msg) } {}

      virtual location const& end() const override { return location::undefined; }
      virtual location const& start() const override { return location::undefined; }

      virtual diagnostic_kind kind() const override {
         return diagnostic_kind::syntax;
      };

      virtual diagnostic_level level() const override {
         return diagnostic_level::error;
      };

      using diagnostic::accept;
      virtual void accept(diag::visitor& v) const override {
         v.visit_syntax_error(*this);
      }

   private:
      // rush::lexical_token _tok;
   };
}

#endif // RUSH_DIAG_SYNTAX_ERROR_HPP
