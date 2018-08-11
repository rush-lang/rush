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
				else _ostr << "- ";
			}
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
			_ostr << std::endl;
			_current_indent = 0;
		}

	public:
		basic_printer(std::basic_ostream<CharT, Traits>& out)
			: _indent(0)
			, _current_indent(0)
			, _ostr(out) {}

		virtual void visit_type(ast::type const&) override {
			write("type");
		}

		virtual void visit_unary_expr(unary_expression const& expr) override {
			switch (expr.opkind()) {
			case unary_operator::plus: write("<unary-plus : "); break;
			case unary_operator::negate: write("<unary-negate : "); break;
			case unary_operator::increment: write("<unary-increment : "); break;
			case unary_operator::decrement: write("<unary-decrement : "); break;
			}

			expr.result_type().accept(*this); write(">");
			indent();
			expr.operand().accept(*this);
			dedent();
		}

		virtual void visit_binary_expr(binary_expression const& expr) override {
			switch (expr.opkind()) {
			case binary_operator::addition: write("<binary-addition : "); break;
			case binary_operator::subtraction: write("<binary-subtraction : "); break;
			case binary_operator::multiplication: write("<binary-multiplication : "); break;
			case binary_operator::division: write("<binary-division : "); break;
			case binary_operator::modulo: write("<binary-modulo : "); break;
			case binary_operator::logical_or: write("<logical-or : "); break;
			case binary_operator::logical_and: write("<logical-and : "); break;
			}

			expr.result_type().accept(*this); writeln(">");

			indent();
			write("left: "); expr.left_operand().accept(*this);
			write("right: "); expr.right_operand().accept(*this);
			dedent();
		}

		virtual void visit_literal_expr(literal_expression const& expr) override {
			write("<literal : ");
			expr.result_type().accept(*this); writeln(">");
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
			write("<identifier : ");
			expr.result_type().accept(*this);
			write(" : \"");
			write(expr.name());
			writeln("\">");
		}

		virtual void visit_constant_decl(constant_declaration const& decl) override {
			print_storage_decl("variable", decl);
		}

		virtual void visit_variable_decl(variable_declaration const& decl) override {
			print_storage_decl("variable", decl);
		}

	private:
		std::size_t _indent;
		std::size_t _current_indent;
		std::basic_ostream<CharT, Traits>& _ostr;

		void print_literal_expr(std::string value, literal_expression const& expr) {
			write("<literal : ");
			expr.result_type().accept(*this); writeln(">");
			indent();
			writeln("value: {}", value);
			dedent();
		}

		void print_storage_decl(std::string name, storage_declaration const& decl) {
			write("<");
			write(name);
			write("-declaration : ");
			decl.type().accept(*this);
			writeln(">");

			indent();
			write("name: \""); write(decl.name()); writeln("\"");
			write("init: "); decl.initializer().accept(*this);
			dedent();
		}
	};

	using printer = basic_printer<char>;
	using wprinter = basic_printer<wchar_t>;
} // rush::ast

#endif // RUSH_AST_PRINTER_HPP
