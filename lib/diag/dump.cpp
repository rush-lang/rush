#include "rush/diag/dump.hpp"
#include "rush/diag/syntax_error.hpp"
#include "rush/diag/semantic_error.hpp"
#include "rush/diag/semantic_warning.hpp"

#include "fmt/format.h"

static void dump_diag(rush::diagnostic const& d, std::string_view type, std::ostream& out) {
   out << fmt::format("[{}:RS{:04}, source:{}, line:{}, column:{}]: {}",
      type, d.code(), d.source(), d.start().line(),
      d.start().column(), d.message());
}

namespace rush {
   void dump(diag::syntax_error const& err, std::ostream& out) { dump_diag(err, "error", out); }
   void dump(diag::semantic_error const& err, std::ostream& out) { dump_diag(err, "error", out); }
   void dump(diag::semantic_warning const& wrn, std::ostream& out) { dump_diag(wrn, "warning", out); }
}
