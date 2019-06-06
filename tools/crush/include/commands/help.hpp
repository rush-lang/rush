#pragma once

#ifndef CRUSH_COMMANDS_HELP_HPP
#define CRUSH_COMMANDS_HELP_HPP

#include <iostream>

namespace crush::commands {
   void help(std::string msg) {
      std::cout << msg << std::endl;
   }
}

#endif // CRUSH_COMMANDS_HELP_HPP
