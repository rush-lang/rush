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

#ifndef RUSH_PARSER_STRING_PARSER_HPP
#define RUSH_PARSER_STRING_PARSER_HPP

#include "rush/parser/parser.hpp"
#include "rush/ast/exprs/string.hpp"
#include "rush/lexer/lex.hpp"
#include "rush/core/ascii.hpp"

#include <vector>

namespace rush {
   //! \brief Parses the contents of string literal tokens.
   //         If string template punctuation is found within the string,
   //         the tokens between the punctuation are extracted and fed
   //         to the primary parser for evaluation.
   //         The result is a string template or, in the case of no
   //         template punctuation, a string literal expression.
   class string_parser {
   public:
      string_parser(rush::parser& primary)
         : _primary { primary } {}

      rush::parse_result<ast::expression> parse(rush::lexical_token tok) {
         using namespace rush::ast;
         using namespace rush::diag;

         auto str = tok.text();
         if (str.begin() == str.end())
            return exprs::literal(std::move(str), *(_primary._context));

         std::vector<std::unique_ptr<ast::expression>> exprs;

         auto it1 = str.begin();
         auto it2 = find_template_substitution(it1, str.end());
         if (it1 != it2) exprs.push_back(exprs::literal({ it1, it2 }, *(_primary._context)));

         bool is_template = it2 != str.end();
         while (it1 != str.end() && it2 != str.end()) {
            it1 = it2 + 1;
            if (*it1 == '{') {
               if ((it2 = find_closing_bracket(++it1, str.end())) == str.end()) {
                  tok = rush::tokens::quote(tok.location().next_column(tok.size() + 1));
                  return errs::expected_closing_bracket(tok); }
               auto lxa = rush::lex(std::string { it1, it2 });
               if (!lxa.empty()) {
                  auto expr = parse_expr(lxa);
                  if (expr.failed()) {
                     return errs::template_string_parse_error(tok,
                        (*expr.errors().begin())->message());
                  }
                  exprs.push_back(std::move(expr));
               }

               // move the start iterator past the closing '}'
               it1 = it2 + 1;
            } else {
               it2 = find_non_ident_char(it1, str.end());
               auto lxa = rush::lex(std::string { it1, it2 });
               if (!lxa.empty()) {
                  auto expr = parse_expr(lxa);
                  if (expr.failed()) {
                     return errs::template_string_parse_error(tok,
                        (*expr.errors().begin())->message());
                  }
                  exprs.push_back(std::move(expr));
               }

               // move the start iterator past the identifier.
               it1 = it2;
            }

            if (it1 != str.end()) {
               it2 = find_template_substitution(it1, str.end());
               exprs.push_back(exprs::literal({ it1, it2 }, *(_primary._context)));
            }
         }

         return is_template
              ? exprs::string_template(std::move(str), std::move(exprs))
              : std::move(exprs.back());
      }
   private:
      rush::parser& _primary;

      rush::parse_result<ast::expression> parse_expr(rush::lexical_analysis const& lxa) {
         auto prev = _primary.inject(lxa.begin(), lxa.end());
         auto expr = _primary.parse_expr();
         _primary.inject(prev.first, prev.second);
         return std::move(expr);
      }

      std::string::iterator find_template_substitution(std::string::iterator first, std::string::iterator last) {
         auto prev = first;
         for (; first != last; prev = first++) {
            if (*first == '$' && *prev != '\\') {
               auto next = first + 1;
               if (next != last) {
                  if (*next != '_') {
                     if ((ascii::is_ident_head(*next) || *next == '{')) return first;
                  } else if ((next + 1 != last && ascii::is_ident_body(*(next + 1)))) {
                     return first;
                  }
               }
            }
         }

         return first;
      }

      std::string::iterator find_closing_bracket(std::string::iterator first, std::string::iterator last) {
         auto count = 1;
         for (; first != last && count > 0; ++first) {
            if (*first == '{') ++count;
            else if (*first == '}') {
               if (--count == 0) return first;
            }
         }

         return first;
      }

      std::string::iterator find_non_ident_char(std::string::iterator first, std::string::iterator last) {
         if (first == last || !rush::ascii::is_ident_head(*first)) return first;
         for (++first; first != last && rush::ascii::is_ident_body(*first); ++first)
            if (!rush::ascii::is_ident_body(*first)) return first;
         return first;
      }
   };
}

#endif // RUSH_PARSER_STRING_PARSER_HPP
