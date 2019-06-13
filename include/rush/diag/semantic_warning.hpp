#pragma once

#ifndef RUSH_DIAG_SEMANTIC_WARNING_HPP
#define RUSH_DIAG_SEMANTIC_WARNING_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/diag/visitor.hpp"

namespace rush::diag {
   class semantic_warning : public diagnostic {
   public:
      using code_type = diagnostic::code_type;
      semantic_warning(code_type code, std::string msg)
         : diagnostic { code, std::move(msg) } {}

      virtual location const& end() const override { return location::undefined; }
      virtual location const& start() const override { return location::undefined; }

      virtual diagnostic_kind kind() const override {
         return diagnostic_kind::semantic;
      };

      virtual diagnostic_level level() const override {
         return diagnostic_level::warning;
      };

      using diagnostic::accept;
      virtual void accept(diag::visitor& v) const override {
         v.visit_semantic_warning(*this);
      }
   };

   namespace wrns {
#     define RUSH_DIAG_SEMANTIC_WARNING_FUNC_IMPL
#     include "rush/diag/_diagnostics.hpp"
   }
}

#endif // RUSH_DIAG_SEMANTIC_WARNING_HPP
