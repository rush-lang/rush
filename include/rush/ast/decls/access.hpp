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

#ifndef RUSH_AST_DECLS_ACCESS_HPP
#define RUSH_AST_DECLS_ACCESS_HPP

#include <cstdint>

namespace rush::ast {
   enum class module_access : std::uint8_t {
      internal,
      exported
   };

   enum class member_access : std::uint8_t {
      public_,
      private_,
      protected_,
      internal
   };
}

#endif // RUSH_AST_DECLS_ACCESS_HPP
