#pragma once

#ifndef RUSH_AST_TYPES_TYPE_MATCHER_HPP
#define RUSH_AST_TYPES_TYPE_MATCHER_HPP

#include "rush/ast/types/type_ref.hpp"

namespace rush::ast::types {

   bool match(ast::type_ref, ast::type_ref);
}

#endif // RUSH_AST_TYPES_TYPE_MATCHER_HPP
