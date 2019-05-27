#pragma once

#ifndef RUSH_PARSER_BASIC_PRINTER_HPP
#define RUSH_PARSER_BASIC_PRINTER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/visitor.hpp"

#include "fmt/format.h"
#include <iostream>

namespace rush {
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_printer : public ast::visitor {
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

      virtual void visit_builtin_void_type(ast::builtin_void_type const& type) override {
         write("void");
      }

      virtual void visit_builtin_bool_type(ast::builtin_bool_type const& type) override {
         write("bool");
      }

      virtual void visit_builtin_integral_type(ast::builtin_integral_type const& type) override {
         switch (type.unit()) {
         case ast::integral_kind::byte: type.is_signed() ? write("sbyte") : write("byte"); break;
         case ast::integral_kind::word: type.is_signed() ? write("short") : write("ushort"); break;
         case ast::integral_kind::dword: type.is_signed() ? write("int") : write("uint"); break;
         case ast::integral_kind::qword: type.is_signed() ? write("long") : write("ulong"); break;
         }
      }

      virtual void visit_builtin_floating_type(ast::builtin_floating_type const& type) override {
         switch (type.fpkind()) {
         case ast::floating_kind::ieee16: write("builtin.ieee16"); break;
         case ast::floating_kind::ieee32: write("float"); break;
         case ast::floating_kind::ieee64: write("double"); break;
         case ast::floating_kind::ieee80: write("builtin.ieee80"); break;
         case ast::floating_kind::ieee128: write("builtin.ieee128"); break;
         case ast::floating_kind::ppc128: write("builtin.ppc218"); break;
         }
      }


      virtual void visit_function_type(ast::function_type const& type) override {
         if (type.parameters().empty()) {
            write("()");
         } else if (type.parameters().count() == 1) {
            type.parameters()
               .first()->type()
               .accept(*this);
         } else {
            write("(");
            std::for_each(type.parameters().begin(), type.parameters().end(),
               [this, &type](auto& p) {
                  p->type().accept(*this);
                  if (p != type.parameters().last()) write(", ");
               });
            write(")");
         }

         write(" => ");
         type.return_type().accept(*this);
      }

		virtual void visit_unary_expr(ast::unary_expression const& expr) override {
			switch (expr.opkind()) {
			default: throw;
			case ast::unary_operator::positive: print_expression("positive", expr); break;
			case ast::unary_operator::negative: print_expression("negative", expr); break;
			case ast::unary_operator::pre_increment: print_expression("pre_increment", expr); break;
			case ast::unary_operator::pre_decrement: print_expression("pre_decrement", expr); break;
			case ast::unary_operator::post_increment: print_expression("post_increment", expr); break;
			case ast::unary_operator::post_decrement: print_expression("post_decrement", expr); break;
			case ast::unary_operator::logical_negation: print_expression("logical_negation", expr); break;
			case ast::unary_operator::bitwise_negation: print_expression("bitwise_negation", expr); break;
			}
			writeln();
			indent();
			expr.operand().accept(*this);
			dedent();
		}

		virtual void visit_binary_expr(ast::binary_expression const& expr) override {
			switch (expr.opkind()) {
			default: throw;
			case ast::binary_operator::equal: print_expression("equals", expr); break;
			case ast::binary_operator::not_equal: print_expression("not_equal", expr); break;
			case ast::binary_operator::addition: print_expression("add", expr); break;
			case ast::binary_operator::subtraction: print_expression("subtract", expr); break;
			case ast::binary_operator::multiplication: print_expression("multiply", expr); break;
			case ast::binary_operator::division: print_expression("divide", expr); break;
			case ast::binary_operator::modulo: print_expression("modulo", expr); break;
			case ast::binary_operator::logical_or: print_expression("logical_or", expr); break;
			case ast::binary_operator::logical_and: print_expression("logical_and", expr); break;
			case ast::binary_operator::less_than: print_expression("less_than", expr); break;
			case ast::binary_operator::less_equals: print_expression("less_equals", expr); break;
			case ast::binary_operator::greater_than: print_expression("greater_than", expr); break;
			case ast::binary_operator::greater_equals: print_expression("greater_equals", expr); break;
			}

			writeln();
			indent();
			expr.left_operand().accept(*this);
			expr.right_operand().accept(*this);
			dedent();
		}

		virtual void visit_literal_expr(ast::nil_literal_expression const& expr) override {
			print_expression("nil", expr);
			writeln();
		}

		virtual void visit_literal_expr(ast::string_literal_expression const& expr) override {
			print_literal_expr(fmt::format("\"{}\"", expr.value()), expr);
		}

		virtual void visit_literal_expr(ast::boolean_literal_expression const& expr) override {
			print_literal_expr(expr.value() ? "true" : "false", expr);
		}

		virtual void visit_literal_expr(ast::integer_literal_expression const& expr) override {
			print_literal_expr(fmt::to_string(expr.value()), expr);
		}

		virtual void visit_literal_expr(ast::floating_literal_expression const& expr) override {
			print_literal_expr(fmt::to_string(expr.value()), expr);
		}

		virtual void visit_identifier_expr(ast::identifier_expression const& expr) override {
			write("<identifier_expr: ");
			expr.result_type().accept(*this);
			writeln(" (name=\"{}\")>", expr.name());
		}

		virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
			print_storage_decl("constant", decl);
		}

		virtual void visit_variable_decl(ast::variable_declaration const& decl) override {
			print_storage_decl("variable", decl);
		}

      virtual void visit_parameter_decl(ast::parameter const& param) override {
         write("<param: ");
         param.type().accept(*this);
         writeln(" (name=\"{}\")>", param.name());
      }

      virtual void visit_function_decl(ast::function_declaration const& decl) override {
         write("<function_decl: ");
         decl.type().accept(*this);
         writeln(" (name=\"{}\")>", decl.name());
         indent();
         decl.parameters().accept(*this);
         decl.body().accept(*this);
         dedent();
      }

      virtual void visit_return_stmt(ast::return_statement const& stmt) override {
         writeln("<return_stmt>");
         indent();
         if (stmt.expr() != nullptr)
            stmt.expr()->accept(*this);
         dedent();
      }

	private:
		std::size_t _indent;
		std::size_t _current_indent;
		std::basic_ostream<CharT, Traits>& _ostr;

		void print_expression(std::string name, ast::expression const& expr) {
			write("<{}_expr: ", name);
			expr.result_type().accept(*this);
			write(">");
		}

		void print_literal_expr(std::string value, ast::literal_expression const& expr) {
			write("<literal_expr: ");
			expr.result_type().accept(*this);
			writeln(" (value={})>", value);
		}

		void print_storage_decl(std::string name, ast::storage_declaration const& decl) {
			write("<{}_decl: ", name);
			decl.type().accept(*this);
			writeln(" (name=\"{}\")>", decl.name());

			if (decl.initializer() != nullptr) {
			   indent();
				decl.initializer()->accept(*this);
			   dedent();
         }
		}
	};

   using printer = basic_printer<char>;
   using wprinter = basic_printer<wchar_t>;
} // rush

#endif // RUSH_PARSER_BASIC_PRINTER_HPP
