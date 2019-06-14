#pragma once

#ifndef RUSH_DIAG_SEMANTIC_ERROR_HPP
#define RUSH_DIAG_SEMANTIC_ERROR_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/diag/visitor.hpp"
#include "rush/lexer/token.hpp"

#include "fmt/format.h"

#include <string>
#include <string_view>

namespace rush::diag {
   class semantic_error : public diagnostic {
   public:
      using code_type = diagnostic::code_type;
      semantic_error(code_type code, std::string msg)
         : diagnostic { code, rush::source::none, std::move(msg) } {}

      virtual location const& end() const override { return location::undefined; }
      virtual location const& start() const override { return location::undefined; }

      virtual diagnostic_kind kind() const override {
         return diagnostic_kind::semantic;
      };

      virtual diagnostic_level level() const override {
         return diagnostic_level::error;
      };

      using diagnostic::accept;
      virtual void accept(diag::visitor& v) const override {
         v.visit_semantic_error(*this);
      }
   };

   namespace errs {
#     define RUSH_DIAG_SEMANTIC_ERROR_FUNC_IMPL
#     include "rush/diag/_diagnostics.hpp"
   }
}

#endif // RUSH_DIAG_SEMANTIC_ERROR_HPP
