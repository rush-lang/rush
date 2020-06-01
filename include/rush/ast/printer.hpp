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

#ifndef RUSH_PARSER_BASIC_PRINTER_HPP
#define RUSH_PARSER_BASIC_PRINTER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/patterns.hpp"
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
         writeln("<[decl] import: (name=\"{}\")>", decl.name());
      }

      virtual void visit_module_decl(ast::module_declaration const& decl) override {
         auto r = rush::guard(_maccess, decl.access());
         traverse(decl);
      }

      virtual void visit_type_extension(ast::type_extension const& type) override {
         write("@");
         type.underlying_type().accept(*this);
      }

      virtual void visit_optional_type(ast::optional_type const& type) override {
         type.underlying_type().accept(*this);
         write("?");
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

      virtual void visit_array_type(ast::array_type const& type) override {
         write("[");
         type.underlying_type().accept(*this);
         write("]");
      }

      virtual void visit_tuple_type(ast::tuple_type const& type) override {
         if (type.types().size() == 1 &&
            !type.types().front().is<ast::tuple_type>() &&
            !type.types().front().is<ast::function_type>()) {
               type.types().front().accept(*this);
         } else {
            write("(");
            auto it = type.types().begin();
            for (; it != type.types().end(); ++it) {
               it->accept(*this);
               if (it != type.types().end() - 1)
                  write(", ");
            }
            write(")");
         }
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
         type.parameter_types().accept(*this);
         write(" -> ");
         type.return_type().accept(*this);
      }

      virtual void visit_user_type(ast::user_type const& type) override {
         write("{}", type.name());
      }

      virtual void visit_named_ptrn(ast::named_pattern const& ptrn) override {
         write("<[decl] ");
         switch (ptrn.kind()) {
         case ast::declaration_kind::variable: write("variable: "); break;
         case ast::declaration_kind::constant: write("constant: "); break;
         case ast::declaration_kind::parameter: write("parameter: "); break;
         default: assert("impossible.");
         }

         ptrn.type().accept(*this);
         writeln(" (name=\"{}\")>", ptrn.name());
      }

      virtual void visit_discard_ptrn(ast::discard_pattern const& ptrn) override {
         writeln("<[ptrn] discard>");
      }

      virtual void visit_binding_ptrn(ast::binding_pattern const& ptrn) override {
         switch (ptrn.kind()) {
         case ast::binding_kind::unknown: write("<binding: "); break;
         case ast::binding_kind::parameter: write("<[expr] parameter-binding: "); break;
         case ast::binding_kind::initializer: write("<[expr] initializer-binding: "); break;
         case ast::binding_kind::default_value: write("<[expr] default-value-binding: "); break;
         }
         ptrn.result_type().accept(*this);
         writeln(">");
         indent_traverse(ptrn);
      }

      virtual void visit_array_destructure_ptrn(ast::array_destructure_pattern const& ptrn) override {
         write("<[ptrn] destructure-array: ");
         ptrn.type().accept(*this);
         writeln(">");
         indent_traverse(ptrn);
      }

      virtual void visit_object_destructure_ptrn(ast::object_destructure_pattern const& ptrn) override {
         write("<[ptrn] destructure-object: ");
         ptrn.type().accept(*this);
         writeln(">");
         indent_traverse(ptrn);
      }

      virtual void visit_type_annotation_ptrn(ast::type_annotation_pattern const& ptrn) override {
         write("<[ptrn] type-annotation: (type=");
         ptrn.type().accept(*this);
         writeln(")>");
         indent_traverse(ptrn);
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

      virtual void visit_iteration_stmt(ast::iteration_statement const& stmt) override {
#        define RUSH_SIMPLE_ITERATION_PRINT_VISIT_SWITCH
#        include "rush/ast/stmts/_statements.hpp"
         indent_traverse(stmt);
      }

      virtual void visit_ternary_expr(ast::ternary_expression const& expr) override {
         print_expression("ternary", expr);
         indent_traverse(expr);
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

      virtual void visit_literal_expr(ast::array_literal_expression const& expr) override {
         print_expression("array-literal", expr);
         indent_traverse(expr);
      }

      virtual void visit_literal_expr(ast::tuple_literal_expression const& expr) override {
         print_expression("tuple-literal", expr);
         indent_traverse(expr);
      }

		virtual void visit_identifier_expr(ast::identifier_expression const& expr) override {
			write("<[expr] identifier: ");
			expr.result_type().accept(*this);
			writeln(" (name=\"{}\")>", expr.name());
		}

      virtual void visit_member_access_expr(ast::member_access_expression const& expr) override {
         print_expression("member-access", expr);
         indent_traverse(expr);
      }

      virtual void visit_string_template_expr(string_template_expression const& expr) override {
         write("<[expr] template: ");
			expr.result_type().accept(*this);
			writeln(" (original=\"{}\")>", expr.original());
         indent_traverse(expr);
      }

      virtual void visit_invoke_expr(ast::invoke_expression const& expr) override {
         print_expression("invoke", expr);
         indent();
         expr.callable().accept(*this);
         if (!expr.arguments().empty()) {
            writeln("<arguments>");
            indent_accept(expr.arguments());
         }
         dedent();
      }

      virtual void visit_lambda_expr(ast::lambda_expression const& expr) override {
         print_expression("lambda", expr);
         indent_traverse(expr);
      }

      virtual void visit_nil_expr(ast::nil_expression const& expr) override {
			print_expression("nil", expr);
		}

      virtual void visit_new_expr(ast::new_expression const& expr) override {
			print_expression("new", expr);
         indent_traverse(expr);
		}

      virtual void visit_this_expr(ast::this_expression const& expr) override {
			print_expression("this", expr);
		}

		virtual void visit_constant_decl(ast::constant_declaration const& decl) override {
			print_storage_decl("constant", decl);
		}

		virtual void visit_variable_decl(ast::variable_declaration const& decl) override {
			print_storage_decl("variable", decl);
		}

      virtual void visit_parameter_decl(ast::parameter_declaration const& decl) override {
         print_storage_decl("parameter", decl);
      }

      virtual void visit_function_decl(ast::function_declaration const& decl) override {
         write("<[decl] function: ");
         decl.type().accept(*this);
         write(" (name=\"{}\"", decl.name());
         switch (_maccess) {
         case ast::module_access::internal: writeln(", access=internal)>"); break;
         case ast::module_access::exported: writeln(", access=exported)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_class_decl(ast::class_declaration const& decl) override {
         write("<[decl] class: {}", decl.name());
         switch (_maccess) {
         case ast::module_access::internal: writeln(" (access=internal)>"); break;
         case ast::module_access::exported: writeln(" (access=exported)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_struct_decl(ast::struct_declaration const& decl) override {
         write("<[decl] struct: {}", decl.name());
         switch (_maccess) {
         case ast::module_access::internal: writeln(" (access=internal)>"); break;
         case ast::module_access::exported: writeln(" (access=exported)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_variable_field_decl(ast::variable_field_declaration const& decl) override {
         print_field_decl("variable", decl);
      }

      virtual void visit_constant_field_decl(ast::constant_field_declaration const& decl) override {
         print_field_decl("constant", decl);
      }

      virtual void visit_property_getter_decl(ast::property_getter_declaration const& decl) override {
         write("<[decl] property-get: ");
         decl.type().accept(*this);
         write(" (name=\"{}\"", decl.name());
         switch (decl.access()) {
         case ast::member_access::internal: writeln(", access=internal)>"); break;
         case ast::member_access::public_: writeln(", access=public)>"); break;
         case ast::member_access::private_: writeln(", access=private)>"); break;
         case ast::member_access::protected_: writeln(", access=protected)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_property_setter_decl(ast::property_setter_declaration const& decl) override {
         write("<[decl] property-set: ");
         decl.type().accept(*this);
         write(" (name=\"{}\"", decl.name());
         switch (decl.access()) {
         case ast::member_access::internal: writeln(", access=internal)>"); break;
         case ast::member_access::public_: writeln(", access=public)>"); break;
         case ast::member_access::private_: writeln(", access=private)>"); break;
         case ast::member_access::protected_: writeln(", access=protected)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_method_decl(ast::method_declaration const& decl) override {
         write("<[decl] method: ");
         decl.type().accept(*this);
         write(" (name=\"{}\"", decl.name());
         switch (decl.access()) {
         case ast::member_access::internal: writeln(", access=internal)>"); break;
         case ast::member_access::public_: writeln(", access=public)>"); break;
         case ast::member_access::private_: writeln(", access=private)>"); break;
         case ast::member_access::protected_: writeln(", access=protected)>"); break;
         }
         indent_traverse(decl);
      }

      virtual void visit_block_stmt(ast::statement_block const& stmt) override {
         writeln("<[stmt] block>");
         indent_traverse(stmt);
      }

   private:
      std::size_t _indent;
      std::size_t _current_indent;
      rush::ast::module_access _maccess;
      std::basic_ostream<CharT, Traits>& _ostr;

      void indent_accept(ast::node const& ast) {
         indent();
         ast.accept(*this);
         dedent();
      }

      void indent_traverse(ast::node const& ast) {
         indent();
         traverse(ast);
         dedent();
      }

		void print_expression(std::string name, ast::expression const& expr) {
			write("<[expr] {}: ", name);
			expr.result_type().accept(*this);
			writeln(">");
		}

		void print_literal_expr(std::string value, ast::literal_expression const& expr) {
			write("<[expr] literal: ");
			expr.result_type().accept(*this);
			writeln(" (value={})>", value);
		}

		void print_storage_decl(std::string kind, ast::storage_declaration const& decl) {
			write("<[decl] storage: (kind={}", kind);
         if (_current_indent == 1) {
            switch (_maccess) {
            case ast::module_access::internal: write(", access=internal"); break;
            case ast::module_access::exported: write(", access=exported"); break;
            }
         }
         writeln(")>");
         indent_traverse(decl);
		}

      void print_field_decl(std::string kind, ast::field_declaration const& decl) {
         write("<[decl] field: (kind={}", kind);
         switch (decl.access()) {
         case ast::member_access::internal: write(", access=internal"); break;
         case ast::member_access::public_: write(", access=public"); break;
         case ast::member_access::private_: write(", access=private"); break;
         case ast::member_access::protected_: write(", access=protected"); break;
         }
         writeln(")>");
         indent_traverse(decl);
      }
	};

   using printer = basic_printer<char>;
   using wprinter = basic_printer<wchar_t>;
} // rush

#endif // RUSH_PARSER_BASIC_PRINTER_HPP
