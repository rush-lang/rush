#include "rush/core/ascii.hpp"
#include "rush/core/source.hpp"
#include "rush/extra/algorithm.hpp"
#include "rush/extra/iterator.hpp"
#include "rush/extra/function_ref.hpp"
#include "rush/lexer/token.hpp"
#include "rush/lexer/lexer.hpp"
#include "rush/lexer/lexer_iterator.hpp"
#include "rush/lexer/lookahead.hpp"
#include "rush/lexer/indentation.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <streambuf>
#include <optional>

using namespace rush::ascii;
namespace tok = rush::tokens;

namespace rush {
   template <typename InIter>
   class basic_lexer {
      basic_lexer(basic_lexer const&) = delete;
      void operator = (basic_lexer const&) = delete;

   public:
      explicit basic_lexer(lexer_options opts = {}) noexcept
         : _opts(opts) {}

      basic_lexer(basic_lexer&& other) = default;
      basic_lexer& operator = (basic_lexer&& other) = default;

      std::vector<lexical_token> tokenize(InIter first, InIter last) {
         auto toks = std::vector<lexical_token> {};
         _lab = std::make_unique<lookahead_buffer_type>(
            lexer_iterator { first, *first, {} },
            lexer_iterator { last, 0, rush::location::undefined});
         _indent = {};

         if (!eof()) {
            auto tok = next_token();
            for (; !tok.is(symbols::eof); tok = next_token())
               toks.push_back(std::move(tok));
         }

         return std::move(toks);
      }

   private:
      using lookahead_buffer_type = typename rush::lookahead_buffer<
         lexer_iterator<InIter>, 8>;

      lexer_options _opts;
      std::unique_ptr<lookahead_buffer_type> _lab;
      rush::indentation _indent;
      rush::location _loc; // location pinned before every new token.

      bool eof() const noexcept {
         return _lab->empty();
      }

      bool is_line_start() const noexcept {
         return !eof() && _lab->peek().location().column() == 1;
      }

      rush::location location() const noexcept {
         return _loc;
      }

      bool check(codepoint_t cp, std::size_t offset = 0) {
         return !eof() && peek(offset) == cp;
      }

      bool icheck(codepoint_t cp, std::size_t offset = 0) {
         return !eof() && iequal(peek(offset), cp);
      }

      bool check(std::string str, std::size_t offset = 0) {
         for (std::size_t i = 0; i < str.length(); ++i)
            if (!check(str[i], offset + i)) return false;
         return true;
      }

      bool icheck(std::string str, std::size_t offset = 0) {
         for (std::size_t i = 0; i < str.length(); ++i)
            if (!icheck(str[i], offset + i)) return false;
         return true;
      }

      auto check_if(function_ref<bool(codepoint_t)> predicate, std::size_t offset = 0)
         -> decltype(predicate(codepoint_t{}), bool{}) {
            return !eof() && predicate(peek(offset));
         }

      codepoint_t peek(std::size_t offset = 0) {
         assert(!eof() && "unexpected end of source.");
         return offset < _lab->size()
            ? _lab->peek(offset).elem()
            : npos;
      }

      codepoint_t skip(std::size_t offset = 1) {
         assert(!eof() && "unexpected end of source.");
         _lab->skip(offset);
         return !eof() ? peek() : npos;
      }

      std::string scan(std::size_t count) {
         assert(!eof() && "unexpected end of source.");
         std::string result;
         for (; !eof() && count > 0; --count)
            result.push_back(_lab->next().elem());
         return std::move(result);
      }

      codepoint_t skip_while(function_ref<bool(codepoint_t)> pred) {
         // assert(!eof() && "unexpected end of source.");
         auto cp = npos;
         while (!eof() && pred(cp = _lab->peek().elem())) _lab->skip();
         return cp;
      }

      std::string scan_while(function_ref<bool(codepoint_t)> pred) {
         // assert(!eof() && "unexpected end of source.");
         std::string result;
         while (!eof() && pred(peek()))
            result.push_back(_lab->next().elem());
         return std::move(result);
      }

      lexical_token next_token() {
         auto indent = scan_whitespace();
         if (indent.has_value()) return *indent;

         if (!eof()) {
            _loc = _lab->peek().location(); // pin the location
            auto cp = peek();

            if (is_newline(cp)) return next_token();
            if (is_quote(cp)) return scan_string_literal();
            if (is_digit(cp)) return scan_numeric_literal();
            if (is_ident_head(cp)) return scan_identifier();

            auto symbol = symbols::to_value({ static_cast<char>(cp) });
            if (symbol != symbols::unknown) { return scan_symbol(symbol); }

            skip(); // skip unknown token.
            return tok::make_error_token("unexpected token", location());
         }

         return _unwind_indentation();
      }

      std::optional<lexical_token> scan_whitespace() {
         while (!eof()) {
            skip_while(is_vspace);
            if (eof()) break;

            if (is_line_start()) {
               _loc = _lab->peek().location(); // pin the location
               auto wspace = scan_while(is_hspace);
               if (eof()) break;

               if (!is_newline(peek())) {
                  auto indent = _indent.measure(wspace.begin(), wspace.end());
                  if (indent < _indent) { _indent.decrement(); return tok::dedent(location()); }
                  if (indent > _indent) { _indent.increment(); return tok::indent(location()); }
                  break;
               }
            } else {
               skip_while(is_hspace);
               if (eof() || !is_vspace(peek()))
                  break;
            }
         }
         return std::nullopt;
      }

      lexical_token scan_identifier() {
         assert(!eof() && "unexpected end of source.");
         assert(is_ident_head(peek()) && "expected a leading identifier character while attempting to scan an identifier.");

         auto ident = scan_while(is_ident_body);
         auto kw_val = keywords::to_value(ident);
         return kw_val != keywords::unknown
            ? tok::make_keyword_token(kw_val, location())
            : tok::identifier(ident, location());
      }

      lexical_token scan_string_literal() {
         assert(!eof() && "unexpected end of source.");
         assert(is_quote(peek()) && "expected a leading, double quotation mark, while attempting to scan a string literal.");

         auto prev = peek();
         auto escape = false;

         skip(); // consume quotation mark.
         auto result = scan_while([&](auto const& cp){
            escape = (prev == '\\' && !escape);
            if (is_quote(cp) && !escape)
            { prev = cp; return false; }
            prev = cp; return true;
         });

         if (!check_if(is_quote))
            return tok::make_error_token("expected closing \'\"\'");

         skip(); // consume end quotation mark.
         return tok::string_literal(std::move(result), location());
      }

      lexical_token_suffix scan_floating_literal_suffix() {
         if (icheck('f')) { skip(); return lexical_token_suffix::float_literal; }
         return lexical_token_suffix::none;
      }

      lexical_token_suffix scan_integer_literal_suffix() {
         if (icheck('u')) { skip(); return lexical_token_suffix::unsigned_literal; }
         if (icheck('l')) { skip(); return lexical_token_suffix::long_literal; }

         // if (icheck("ul")) {
         // 	skip(2);
         // 	return
         // 		lexical_token_suffix::long_literal |
         // 		lexical_token_suffix::unsigned_literal;
         // }

         return lexical_token_suffix::none;
      }

      lexical_token scan_integer_literal(function_ref<bool(codepoint_t)> predicate, int base) {
         auto value = scan_while(predicate);
         auto suffix = scan_integer_literal_suffix();
         return tok::suffixed_integer_literal(std::stoll(value, 0, base), suffix, location());
      }

      lexical_token scan_numeric_literal() {
         assert(!eof() && "unexpected end of source.");
         assert(check('.') || check_if(is_digit) && "expected a leading digit while attempting to scan an integer literal.");

         if (is_zero_digit(peek())) {
            skip(); // consume zero digit.

            if (icheck('x')) { skip(); return scan_integer_literal(is_hex_digit, 16); }
            if (icheck('b')) { skip(); return scan_integer_literal(is_bin_digit, 2); }
            if (icheck('o')) { skip(); return scan_integer_literal(is_oct_digit, 8); }

            if (check('.') && !check("..", 1)) {
               skip(); // consume decimal point.
               auto value = scan_while(is_digit);
               auto suffix = scan_floating_literal_suffix();
               return tok::suffixed_floating_literal(std::stod("0." + value), suffix, location());
            }

            auto suffix = scan_integer_literal_suffix();
            return tok::suffixed_integer_literal(0, suffix, location());
         }

         auto integer_part = scan_while(is_digit);
         if (check('.') && !check("..", 1)) {
            skip(); // consume decimal point.
            auto fractional_part = scan_while(is_digit);
            auto suffix = scan_floating_literal_suffix();
            return tok::suffixed_floating_literal(
               std::stod(integer_part + "." + fractional_part),
               suffix, location());
         }

         assert(!integer_part.empty() && "expected an integer digit");
         auto suffix = scan_integer_literal_suffix();
         return tok::suffixed_integer_literal(
            std::stoll(integer_part),
            suffix, location());
      }

      lexical_token scan_symbol(symbol_token_t symbol) {
         assert(symbol != symbols::unknown && "unknown symbol");

         switch (symbol) {
            case symbols::ampersand: {
               if (check('&', 1)) { skip(2); return tok::double_ampersand(location()); }
               if (check('=', 1)) { skip(2); return tok::ampersand_equals(location()); }
            } break;

            case symbols::pipe: {
               if (check('|', 1)) { skip(2); return tok::double_pipe(location()); }
               if (check('=', 1)) { skip(2); return tok::pipe_equals(location()); }
            } break;

            case symbols::plus: {
               if (check('+', 1)) { skip(2); return tok::plus_plus(location()); }
               if (check('=', 1)) { skip(2); return tok::plus_equals(location()); }
            } break;

            case symbols::minus: {
               if (check('>', 1)) { skip(2); return tok::thin_arrow(location()); }
               if (check('-', 1)) { skip(2); return tok::minus_minus(location()); }
               if (check('=', 1)) { skip(2); return tok::minus_equals(location()); }
            } break;

            case symbols::caret: {
               if (check('=', 1)) { skip(2); return tok::caret_equals(location()); }
            } break;

            case symbols::asterisk: {
               if (check('=', 1)) { skip(2); return tok::asterisk_equals(location()); }
            } break;

            case symbols::forward_slash: {
               if (check('=', 1)) { skip(2); return tok::forward_slash_equals(location()); }
            } break;

            case symbols::percent: {
               if (check('=', 1)) { skip(2); return tok::percent_equals(location()); }
            } break;

            case symbols::equals: {
               if (check('>', 1)) { skip(2); return tok::thick_arrow(location()); }
               if (check('=', 1)) { skip(2); return tok::equals_equals(location()); }
            } break;

            case symbols::exclamation_mark: {
               if (check('=', 1)) { skip(2); return tok::exclamation_mark_equals(location()); }
            } break;

            case symbols::left_chevron: {
               if (check('=', 1)) { skip(2); return tok::left_chevron_equals(location()); }
               if (check('<', 1)) { skip(2);
                  if (check('=')) { skip(1); return tok::double_left_chevron_equals(location()); }
                  return tok::double_left_chevron(location());
               }
            } break;

            case symbols::right_chevron: {
               if (check('=', 1)) { skip(2); return tok::right_chevron_equals(location()); }
               if (check('>', 1)) { skip(2);
                  if (check('=')) { skip(1); return tok::double_right_chevron_equals(location()); }
                  return tok::double_right_chevron(location());
               }
            } break;

            case symbols::period: {
               if (check("..", 1)) { skip(3); return tok::ellipses(location()); }
               if (check_if(is_digit, 1)) return scan_numeric_literal();
            } break;

            default:;
         }

         skip(); // consume symbol.
         return tok::make_symbol_token(symbol, location());
      }


      lexical_token _unwind_indentation() {
         if (_indent.depth() > 0) {
            _loc = rush::location {
               _loc.line() + 1,
               _indent.depth()
            };

            _indent.decrement();
            return tok::dedent(location());
         }
         return tok::eof(location());
      }
   };

	lexical_analysis lex(std::string const& input, lexer_options const& opts) {
      auto l = lexer { opts };
      return l.tokenize(input);
	}

   lexical_analysis lex(rush::source const& input, lexer_options const& opts) {
      auto l = lexer { opts };
      return l.tokenize(input.buffer());
	}

	lexical_analysis lex(std::istream& input, lexer_options const& opts) {
		auto l = lexer { opts };
		return l.tokenize(input);
	}

   lexical_analysis lexer::tokenize(std::istream& istr) {
		auto first = std::istreambuf_iterator<char> { istr };
		auto last = std::istreambuf_iterator<char> { };
      auto impl = basic_lexer<decltype(first)> { _opts };
      return lexical_analysis { impl.tokenize(first, last) };
   }

   lexical_analysis lexer::tokenize(std::string_view sv) {
      auto first = sv.begin();
      auto last = sv.end();
      auto impl = basic_lexer<decltype(first)> { _opts };
      return lexical_analysis { impl.tokenize(first, last) };
   }
}
