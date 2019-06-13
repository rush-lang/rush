#pragma once

#ifndef RUSH_DIAG_DUMP_HPP
#define RUSH_DIAG_DUMP_HPP

#include <iostream>

namespace rush {
   namespace diag {
   class syntax_error;
   class semantic_error;
   class semantic_warning;
   class semantic_information;
   }

   void dump(diag::syntax_error const&, std::ostream& = std::cout);
   void dump(diag::semantic_error const&, std::ostream& = std::cout);
   void dump(diag::semantic_warning const&, std::ostream& = std::cout);
   void dump(diag::semantic_information const&, std::ostream& = std::cout);
} // rush::diag

#endif // RUSH_DIAG_DUMP_HPP
