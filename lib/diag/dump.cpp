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
#include "rush/diag/dump.hpp"
#include "rush/diag/syntax_error.hpp"
#include "rush/diag/semantic_error.hpp"
#include "rush/diag/semantic_warning.hpp"

#include "fmt/format.h"

static void dump_diag(rush::diagnostic const& d, std::string_view type, std::ostream& out) {
   out << fmt::format("[{}:R{:04}", type, d.code());
   if (!d.source().id().empty())
      out << " \'" << d.source().id() << "\'";
   out << fmt::format(", ln:{}, col:{}]: {}",
      d.start().line(), d.start().column(), d.message());
}

namespace rush {
   void dump(diag::syntax_error const& err, std::ostream& out) { dump_diag(err, "error", out); }
   void dump(diag::semantic_error const& err, std::ostream& out) { dump_diag(err, "error", out); }
   void dump(diag::semantic_warning const& wrn, std::ostream& out) { dump_diag(wrn, "warning", out); }
}
