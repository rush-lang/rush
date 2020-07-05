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

#ifndef RUSH_DIAG_DIAGNOSTIC_HPP
#define RUSH_DIAG_DIAGNOSTIC_HPP

#include "rush/core/source.hpp"
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

      virtual ~diagnostic() = default;

      code_type code() const noexcept { return _code; }
      std::string const& message() const noexcept { return _msg; }
      rush::source const& source() const noexcept { return *_src; }


      virtual location const& end() const = 0;
      virtual location const& start() const = 0;

      virtual diagnostic_kind kind() const = 0;
      virtual diagnostic_level level() const = 0;

      virtual void accept(diag::visitor& v) const = 0;
      virtual void accept(diag::visitor&& v) const { accept(v); }

   protected:
      diagnostic(code_type code, rush::source const& src, std::string msg)
         : _code { code }
         , _src { &src }
         , _msg { std::move(msg) } {}

   private:
      std::string _msg;
      rush::source const* _src;
      code_type _code;
   };

   bool operator == (diagnostic const&, diagnostic const&);
   bool operator != (diagnostic const&, diagnostic const&);
}

#endif // RUSH_DIAG_DIAGNOSTIC_HPP
