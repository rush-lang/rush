#pragma once

#ifndef RUSH_PARSER_BASIC_PRINTER_HPP
#define RUSH_PARSER_BASIC_PRINTER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/traversal.hpp"

#include "rush/extra/guard.hpp"

#include "fmt/format.h"
#include <iostream>

namespace rush::ast {
	template <typename CharT, typename Traits = std::char_traits<CharT>>
	class basic_printer : public ast::traversal {
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
			: _ostr { out }
			, _indent { 0 }
			, _current_indent { 0 } {}

      virtual void visit_module(ast::module const& mdl) override {
         writeln("<module: \"{}\">", mdl.id());
         indent_traverse(mdl);
      }

      virtual void visit_import_decl(ast::import_declaration const& decl) override {
         writeln("<import_decl: (name=\"{}\")>", decl.name());
      }

      virtual void visit_module_decl(ast::module_declaration const& decl) override {
         auto r = rush::guard(_maccess, decl.access());
         traverse(decl);
      }

      virtual void visit_builtin_error_type(ast::builtin_error_type const& type) override {
         write("<error-type: \"{}\">", type.message());
      }

      virtual void visit_builtin_void_type(ast::builtin_void_type const& type) override {
         write("void");
      }

      virtual void visit_builtin_bool_type(ast::builtin_bool_type const& type) override {
         write("bool");
      }

      virtual void visit_builtin_string_type(ast::builtin_string_type const& type) override {
         write("string");
      }

      virtual void visit_builtin_integral_type(ast::builtin_integral_type const& type) override {
         // write(type.is_signed() ? "builtin.int{}" : "builtin.uint{}", type.bit_width());
         switch (type.unit()) {
         case ast::integral_kind::byte: type.is_signed() ? write("sbyte") : write("byte"); break;
         case ast::integral_kind::word: type.is_signed() ? write("short") : write("ushort"); break;
         case ast::integral_kind::dword: type.is_signed() ? write("int") : write("uint"); break;
         case ast::integral_kind::qword: type.is_signed() ? write("long") : write("ulong"); break;
         }
      }

      virtual void visit_builtin_floating_type(ast::builtin_floating_point_type const& type) override {
         switch (type.fpkind()) {
         case ast::floating_point_kind::ieee16: write("builtin.ieee16"); break;
         // case ast::floating_point_kind::ieee32: write("builtin.ieee32"); break;
         // case ast::floating_point_kind::ieee64: write("builtin.ieee64"); break;
         case ast::floating_point_kind::ieee32: write("float"); break;
         case ast::floating_point_kind::ieee64: write("double"); break;
         case ast::floating_point_kind::ieee80: write("builtin.ieee80"); break;
         case ast::floating_point_kind::ieee128: write("builtin.ieee128"); break;
         case ast::floating_point_kind::ppc128: write("builtin.ppc128"); break;
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

         write(" -> ");
         type.return_type().accept(*this);
      }

		virtual void visit_unary_expr(ast::unary_expression const& expr) override {
#        define RUSH_UNARY_EXPRESSION_PRINT_VISIT_SWITCH
#        include "rush/ast/exprs/_operators.hpp"
         indent_traverse(expr);
		}

		virtual void visit_binary_expr(ast::binary_expression const& expr) override {
#        define RUSH_BINARY_EXPRESSION_PRINT_VISIT_SWITCH
#        include "rush/ast/exprs/_operators.hpp"
         indent_traverse(expr);
		}

      virtual void visit_simple_stmt(ast::simple_statement const& stmt) override {
#        define RUSH_SIMPLE_STATEMENT_PRINT_VISIT_SWITCH
#        include "rush/ast/stmts/_statements.hpp"
         indent_traverse(stmt);
      }

      virtual void visit_result_stmt(ast::result_statement const& stmt) override {
#        define RUSH_RESULT_STATEMENT_PRINT_VISIT_SWITCH
#        include "rush/ast/stmts/_statements.hpp"
         indent_traverse(stmt);
      }

      virtual void visit_conditional_stmt(ast::conditional_statement const& stmt) override {
#        define RUSH_SIMPLE_CONDITIONAL_PRINT_VISIT_SWITCH
#        include "rush/ast/stmts/_statements.hpp"
         indent_traverse(stmt);
      }

      virtual void visit_ternary_expr(ast::ternary_expression const& expr) override {
         print_expression("ternary", expr);
         indent_traverse(expr);
      }

		virtual void visit_literal_expr(ast::nil_literal_expression const& expr) override {
			print_expression("nil", expr);
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

      virtual void visit_invocation_expr(ast::invocation_expression const& expr) override {
         print_expression("invocation", expr);
         indent_traverse(expr);
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
         write(" (name=\"{}\"", decl.name());
         switch (_maccess) {
         case ast::module_access::internal: writeln(", access=internal)>"); break;
         case ast::module_access::exported: writeln(", access=exported)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_block_stmt(ast::statement_block const& stmt) override {
         writeln("<block_stmt>");
         indent_traverse(stmt);
      }

   private:
      std::size_t _indent;
      std::size_t _current_indent;
      rush::ast::module_access _maccess;
      std::basic_ostream<CharT, Traits>& _ostr;

      void indent_traverse(ast::node const& ast) {
         indent();
         traverse(ast);
         dedent();
      }

		void print_expression(std::string name, ast::expression const& expr) {
			write("<{}_expr: ", name);
			expr.result_type().accept(*this);
			writeln(">");
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
