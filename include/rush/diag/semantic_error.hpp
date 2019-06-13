#pragma once

#ifndef RUSH_DIAG_SEMANTIC_ERROR_HPP
#define RUSH_DIAG_SEMANTIC_ERROR_HPP

#include "rush/diag/diagnostic.hpp"
#include "rush/diag/visitor.hpp"

namespace rush::diag {
   class semantic_error : public diagnostic {
   public:
      using code_type = diagnostic::code_type;
      semantic_error(code_type code, std::string msg)
         : diagnostic { code, std::move(msg) } {}

      virtual location const& end() const override { return location::undefined; }
      virtual location const& start() const override { return location::undefined; }

      virtual diagnostic_kind kind() const override {
         return diagnostic_kind::semantic;
      };

      virtual diagnostic_level level() const override {
         return diagnostic_level::error;
      };

      using diagnostic::accept;
      virtual void accept(diag::visitor& v) const {
         v.visit_semantic_error(*this);
      }
   };
}

#endif // RUSH_DIAG_SEMANTIC_ERROR_HPP
