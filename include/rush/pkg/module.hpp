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

#ifndef RUSH_PKG_MODULE_HPP
#define RUSH_PKG_MODULE_HPP

#include <string>

#include "rush/core/source.hpp"

namespace rush::pkg {
   class module {
   public:
      std::string_view name() const;
      std::string_view version() const;
      std::string_view path() const;
      rush::source source() const;
   };
} // rush

#endif // RUSH_PKG_MODULE_HPP
