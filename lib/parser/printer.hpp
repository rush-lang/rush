#pragma once

#ifndef RUSH_AST_PRINTER_HPP
#define RUSH_AST_PRINTER_HPP

#include <iostream>

#include "rush/ast/expression.hpp"
#include "rush/ast/declaration.hpp"

#include "fmt/format.h"

namespace rush::ast {
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

		virtual void visit_type(ast::type const& t) override {
			write(t.name());
		}

		virtual void visit_unary_expr(unary_expression const& expr) override {
			switch (expr.opkind()) {
			case unary_operator::plus: print_expression("unary-plus", expr); break;
			case unary_operator::negate: print_expression("unary-negate", expr); break;
			case unary_operator::increment: print_expression("unary-increment", expr); break;
			case unary_operator::decrement: print_expression("unary-decrement", expr); break;
			}
			writeln();
			indent();
			expr.operand().accept(*this);
			dedent();
		}

		virtual void visit_binary_expr(binary_expression const& expr) override {
			switch (expr.opkind()) {
			case binary_operator::addition: print_expression("binary-addition", expr); break;
			case binary_operator::subtraction: print_expression("binary-subtraction", expr); break;
			case binary_operator::multiplication: print_expression("binary-multiplication", expr); break;
			case binary_operator::division: print_expression("binary-division", expr); break;
			case binary_operator::modulo: print_expression("binary-modulo", expr); break;
			case binary_operator::logical_or: print_expression("logical-or", expr); break;
			case binary_operator::logical_and: print_expression("logical-and", expr); break;
			}
			writeln();
			indent();
			write("left: "); expr.left_operand().accept(*this);
			write("right: "); expr.right_operand().accept(*this);
			dedent();
		}

		virtual void visit_literal_expr(literal_expression const& expr) override {
			print_expression("literal", expr);
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

	private:
		std::size_t _indent;
		std::size_t _current_indent;
		std::basic_ostream<CharT, Traits>& _ostr;

		void print_expression(std::string name, expression const& expr) {
			write("<{}: ", name);
			expr.result_type().accept(*this);
			write(">");
		}

		void print_literal_expr(std::string value, literal_expression const& expr) {
			print_expression("literal", expr);
			writeln(" [value: {}]", value);
		}

		void print_storage_decl(std::string name, storage_declaration const& decl) {
			write("<{}: ", name);
			decl.type().accept(*this);
			write(">");
			writeln(" [name: \"{}\"]", decl.name());
			indent();
			write("init: "); decl.initializer().accept(*this);
			dedent();
		}
	};

	using printer = basic_printer<char>;
	using wprinter = basic_printer<wchar_t>;
} // rush::ast

#endif // RUSH_AST_PRINTER_HPP
