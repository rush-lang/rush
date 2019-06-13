#pragma once

#ifndef RUSH_DIAG_DIAGNOSTIC_HPP
#define RUSH_DIAG_DIAGNOSTIC_HPP

#include "rush/core/location.hpp"

#include <cstdlib>
#include <string>

namespace rush {
   namespace diag {
      class visitor;
   }

   enum class diagnostic_kind {
      syntax,
      semantic,
   };

   enum class diagnostic_level {
      information,
      warning,
      error,
   };

   class diagnostic {
   public:
      using code_type = std::size_t;

      code_type code() const noexcept { return _code; }
      std::string const& message() const noexcept { return _msg; }

      virtual location const& end() const = 0;
      virtual location const& start() const = 0;

      virtual diagnostic_kind kind() const = 0;
      virtual diagnostic_level level() const = 0;

      virtual void accept(diag::visitor& v) const = 0;
      virtual void accept(diag::visitor&& v) const { accept(v); }

   protected:
      diagnostic(code_type code, std::string msg)
         : _code { code }
         , _msg { std::move(msg) } {}

   private:
      std::string _msg;
      code_type _code;
   };

   bool operator == (diagnostic const&, diagnostic const&);
   bool operator != (diagnostic const&, diagnostic const&);
}

#endif // RUSH_DIAG_DIAGNOSTIC_HPP
