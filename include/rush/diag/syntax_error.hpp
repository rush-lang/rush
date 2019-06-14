#pragma once

#ifndef RUSH_DIAG_SYNTAX_ERROR_HPP
#define RUSH_DIAG_SYNTAX_ERROR_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/diag/visitor.hpp"
#include "rush/lexer/token.hpp"

#include "fmt/format.h"

#include <string>
#include <string_view>

namespace rush::diag {
   class syntax_error : public diagnostic {
   public:
      using code_type = diagnostic::code_type;
      syntax_error(code_type code, std::string msg, rush::lexical_token tok)
         : diagnostic { code, tok.source(), std::move(msg) }
         , _tok { tok }
         , _end { tok.location().next_column(tok.size()) } {}

      virtual location const& end() const override {
         return _tok.location();
      }

      virtual location const& start() const override {
         return _end;
      }

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
      rush::lexical_token _tok;
      rush::location _end;
   };

   namespace errs {
#     define RUSH_DIAG_SYNTAX_ERROR_FUNC_IMPL
#     include "rush/diag/_diagnostics.hpp"
   }
}

#endif // RUSH_DIAG_SYNTAX_ERROR_HPP
