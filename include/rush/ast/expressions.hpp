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

#ifndef RUSH_AST_EXPRESSION_HPP
#define RUSH_AST_EXPRESSION_HPP

#include "rush/ast/exprs/list.hpp"
#include "rush/ast/exprs/nil.hpp"
#include "rush/ast/exprs/unary.hpp"
#include "rush/ast/exprs/binary.hpp"
#include "rush/ast/exprs/ternary.hpp"
#include "rush/ast/exprs/literal.hpp"
#include "rush/ast/exprs/identifier.hpp"
#include "rush/ast/exprs/member.hpp"
#include "rush/ast/exprs/invoke.hpp"
#include "rush/ast/exprs/lambda.hpp"
#include "rush/ast/exprs/tuple.hpp"
#include "rush/ast/exprs/array.hpp"
#include "rush/ast/exprs/string.hpp"
#include "rush/ast/exprs/this.hpp"
#include "rush/ast/exprs/new.hpp"
#include "rush/ast/exprs/spread.hpp"
#include "rush/ast/exprs/subscript.hpp"

#endif // RUSH_AST_EXPRESSION_HPP
