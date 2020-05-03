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
