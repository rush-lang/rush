#pragma once

#ifndef RUSH_DIAG_VISITOR_HPP
#define RUSH_DIAG_VISITOR_HPP

namespace rush::diag {
   class syntax_error;
   class semantic_error;
   class semantic_warning;
   class semantic_information;

   class visitor {
   public:
      virtual void visit_syntax_error(syntax_error const&) {};
      virtual void visit_semantic_error(semantic_error const&) {};
      virtual void visit_semantic_warning(semantic_warning const&) {};
      virtual void visit_semantic_information(semantic_information const&) {};
   };
}

#endif // RUSH_DIAG_VISITOR_HPP
