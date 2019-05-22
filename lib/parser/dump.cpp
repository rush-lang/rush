#include "rush/ast/node.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"

#include "rush/parser/options.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"

#include "fmt/format.h"
#include <iostream>

using namespace rush::ast;
namespace rush {
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_printer : public visitor {
		void indent() { ++_indent; }
		void dedent() { --_indent; }

		void write_indent() {
			auto depth = _indent - _current_indent;
			for (std::size_t i = 0; i < depth; ++i) {
				if (i + 1 != depth) _ostr << "   ";
				else _ostr << " - ";
			}
		}

		void writeln() {
			_ostr << std::endl;
			_current_indent = 0;
		}

		template <typename... Args>
		void write(std::string str, Args&&... args) {
			write_indent();
			_ostr << fmt::format(str, std::forward<Args>(args)...);
			_current_indent = _indent;
		}

		template <typename... Args>
		void writeln(std::string str, Args&&... args) {
			write(str, std::forward<Args>(args)...);
			writeln();
		}

	public:
		basic_printer(std::basic_ostream<CharT, Traits>& out)
			: _indent(0)
			, _current_indent(0)
			, _ostr(out) {}

		virtual void visit_builtin_type(ast::builtin_type const& t) override {
			write(t.name());
		}

		virtual void visit_unary_expr(unary_expression const& expr) override {
			switch (expr.opkind()) {
			default: throw;
			case unary_operator::plus: print_expression("plus", expr); break;
			case unary_operator::negate: print_expression("negate", expr); break;
			case unary_operator::increment: print_expression("increment", expr); break;
			case unary_operator::decrement: print_expression("decrement", expr); break;
			case unary_operator::logical_not: print_expression("logical-not", expr); break;
			case unary_operator::bitwise_not: print_expression("bitwise-not", expr); break;
			}
			writeln();
			indent();
			expr.operand().accept(*this);
			dedent();
		}

		virtual void visit_binary_expr(binary_expression const& expr) override {
			switch (expr.opkind()) {
			default: throw;
			case binary_operator::equal: print_expression("equals", expr); break;
			case binary_operator::not_equal: print_expression("not-equal", expr); break;
			case binary_operator::addition: print_expression("addition", expr); break;
			case binary_operator::subtraction: print_expression("subtraction", expr); break;
			case binary_operator::multiplication: print_expression("multiplication", expr); break;
			case binary_operator::division: print_expression("division", expr); break;
			case binary_operator::modulo: print_expression("modulo", expr); break;
			case binary_operator::logical_or: print_expression("logical-or", expr); break;
			case binary_operator::logical_and: print_expression("logical-and", expr); break;
			case binary_operator::less_than: print_expression("less-than", expr); break;
			case binary_operator::less_equals: print_expression("less-equals", expr); break;
			case binary_operator::greater_than: print_expression("greater-than", expr); break;
			case binary_operator::greater_equals: print_expression("greater-equals", expr); break;
			}

			writeln();
			indent();
			write("left: "); expr.left_operand().accept(*this);
			write("right: "); expr.right_operand().accept(*this);
			dedent();
		}

		virtual void visit_literal_expr(nil_literal_expression const& expr) override {
			print_expression("nil", expr);
			writeln();
		}

		virtual void visit_literal_expr(string_literal_expression const& expr) override {
			print_literal_expr(fmt::format("\"{}\"", expr.value()), expr);
		}

		virtual void visit_literal_expr(boolean_literal_expression const& expr) override {
			print_literal_expr(expr.value() ? "true" : "false", expr);
		}

		virtual void visit_literal_expr(integer_literal_expression const& expr) override {
			print_literal_expr(fmt::to_string(expr.value()), expr);
		}

		virtual void visit_literal_expr(floating_literal_expression const& expr) override {
			print_literal_expr(fmt::to_string(expr.value()), expr);
		}

		virtual void visit_identifier_expr(identifier_expression const& expr) override {
			print_expression("identifier", expr);
			writeln(" [name: \"{}\"]", expr.name());
		}

		virtual void visit_constant_decl(constant_declaration const& decl) override {
			print_storage_decl("constant", decl);
		}

		virtual void visit_variable_decl(variable_declaration const& decl) override {
			print_storage_decl("variable", decl);
		}

      virtual void visit_function_decl(function_declaration const& decl) override {
         write("<function:");
         decl.type().accept(*this);
         writeln(" [decl]> [name: {}]", decl.name());
         indent();
         writeln("body: ");
         indent();
         decl.body().accept(*this);
         dedent();
         dedent();
      }

      virtual void visit_return_stmt(return_statement const& stmt) override {
         writeln("<return [stmt]>");
         indent();
         if (stmt.expr() != nullptr)
            stmt.expr()->accept(*this);
         dedent();
      }

	private:
		std::size_t _indent;
		std::size_t _current_indent;
		std::basic_ostream<CharT, Traits>& _ostr;

		void print_expression(std::string name, expression const& expr) {
			write("<{}:", name);
			expr.result_type().accept(*this);
			write(" [expr]>");
		}

		void print_literal_expr(std::string value, literal_expression const& expr) {
			write("<literal:");
			expr.result_type().accept(*this);
			writeln(" [expr]> [value: {}]", value);
		}

		void print_storage_decl(std::string name, storage_declaration const& decl) {
			write("<{}:", name);
			decl.type().accept(*this);
			write(" [decl]>");
			writeln(" [name: \"{}\"]", decl.name());
			indent();
			if (decl.initializer() != nullptr)
				write("initializer: "); decl.initializer()->accept(*this);
			dedent();
		}
	};


   using printer = basic_printer<char>;
   using wprinter = basic_printer<wchar_t>;

   void dump(std::string input, std::ostream& out) { dump(parse(input), out); }
   void dump(std::istream& input, std::ostream& out) { dump(parse(input), out); }
   void dump(std::string input, parser_options const& opts, std::ostream& out) { dump(parse(input, opts), out); }
   void dump(std::istream& input, parser_options const& opts, std::ostream& out) { dump(parse(input, opts), out); }
   void dump(rush::parse_result const& input, std::ostream& out) { dump(*input.ast(), out); }
   void dump(ast::node const& input, std::ostream& out) { input.accept(printer { out }); }
} // rush
