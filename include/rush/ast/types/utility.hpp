#pragma once

#ifndef RUSH_AST_TYPES_UTILITY_HPP
#define RUSH_AST_TYPES_UTILITY_HPP

#include "rush/ast/types/type.hpp"

namespace rush::ast::types {

   ast::type_ref reduce(ast::type_ref, ast::type_ref);
}

#endif // RUSH_AST_TYPES_UTILITY_HPP
