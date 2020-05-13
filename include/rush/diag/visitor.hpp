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
