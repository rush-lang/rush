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

#ifndef CRUSH_COMMANDS_HELP_HPP
#define CRUSH_COMMANDS_HELP_HPP

#include "rush/version.hpp"

#include <iostream>

namespace crush::commands {
   void help(std::string msg) {
      std::cout << msg << std::endl;
   }

   void version() {
      std::cout << "crush version " << RUSH_VERSION << std::endl;
   }
}

#endif // CRUSH_COMMANDS_HELP_HPP
