#pragma once

#ifndef RUSH_AST_TYPES_NAMED_HPP
#define RUSH_AST_TYPES_NAMED_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/type.hpp"

#include <string>

namespace rush::ast {
   class named_type : public type {
   public:
      named_type(std::string name)
         : _name(name) {}

      std::string name() const noexcept {
			return _name;
		}

   private:
      std::string _name;
   };
}

#endif // RUSH_AST_TYPES_NAMED_HPP
