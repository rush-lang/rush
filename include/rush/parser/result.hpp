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

#ifndef RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
#define RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP

#include "fmt/format.h"

#include "rush/ast/node.hpp"
#include "rush/ast/types/type_ref.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/context.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/core/location.hpp"
#include "rush/extra/iterator_range.hpp"
#include "rush/diag/syntax_error.hpp"

#include <memory>
#include <vector>
#include <type_traits>

namespace rush {
   class parser;
	class parser_options;
	class lexical_analysis;

   template <typename NodeT>
   class parse_result;

   class parse_type_result {
   public:
      using error_type = std::unique_ptr<diag::syntax_error>;
      using error_range_type = std::vector<error_type>;
      using const_error_range_type = rush::iterator_range<std::vector<error_type>::const_iterator>;

      parse_type_result()
         : _type { ast::types::undefined }
         , _errors { } {}

      parse_type_result(ast::type_ref type)
         : _type { type }
         , _errors { } {}

      parse_type_result(std::unique_ptr<diag::syntax_error> err)
         : _type { ast::types::undefined }
         , _errors { } { _errors.push_back(std::move(err)); }

      parse_type_result(parse_type_result&& other)
         : _type { std::move(other._type) }
         , _errors { std::move(other._errors) } {}

      parse_type_result& operator = (parse_type_result&& other) {
         if (&other == this) return *this;
         _type = std::move(other._type);
         _errors = std::move(other._errors);
         return *this;
      }

      parse_type_result& operator = (ast::type_ref type) {
         _type = type;
         _errors.clear();
         return *this;
      }

      parse_type_result& operator = (std::unique_ptr<diag::syntax_error> err) {
         _type = rush::ast::types::undefined;
         _errors.clear();
         _errors.push_back(std::move(err));
         return *this;
      }

      bool success() const noexcept {
         return !failed();
      }

      bool failed() const noexcept {
         return is_undefined() && !_errors.empty();
      }

      bool is_undefined() const noexcept {
         return _type == rush::ast::types::undefined;
      }

      error_range_type errors() && noexcept {
         return std::move(_errors);
      }

      const_error_range_type errors() const & noexcept {
         return _errors;
      }

      ast::type_ref type() const noexcept {
         return _type;
      }

   private:
      ast::type_ref _type;
      std::vector<std::unique_ptr<diag::syntax_error>> _errors;
   };


   template <typename NodeT>
   class parse_result {
      friend class syntax_analysis;

      parse_result(parse_result const&) = delete;
      void operator = (parse_result const&) = delete;

      template <typename NodeU>
      friend class parse_result;

   public:
      using error_type = std::unique_ptr<diag::syntax_error>;
      using error_range_type = std::vector<error_type>;
      using const_error_range_type = rush::iterator_range<error_range_type::const_iterator>;

      parse_result()
         : _node { nullptr }
         , _errors { } {}

      parse_result(std::nullptr_t)
         : _node { nullptr }
         , _errors { } {}

      parse_result(error_type err)
         : _node { nullptr }
         , _errors { } { _errors.push_back(std::move(err)); }

      parse_result(error_range_type errs)
         : _node { nullptr }
         , _errors { std::move(errs) } {}

      template <typename NodeU, typename = std::enable_if_t<std::is_base_of_v<NodeT, NodeU>>>
      parse_result(std::unique_ptr<NodeU>&& node)
         : _node { std::move(node) }
         , _errors { } {}

      template <typename NodeU, typename = std::enable_if_t<std::is_base_of_v<NodeT, NodeU>>>
      parse_result(parse_result<NodeU>&& other)
         : _node { std::move(other._node) }
         , _errors { std::move(other._errors) } {}

      parse_result& operator = (std::unique_ptr<diag::syntax_error> err) {
         _node.reset();
         _errors.clear();
         _errors.push_back(std::move(err));
         return *this;
      }

      template <typename NodeU>
      std::enable_if_t<std::is_base_of_v<NodeT, NodeU>,
         parse_result&> operator = (std::unique_ptr<NodeU>&& node) {
            _node = std::move(node);
            _errors.clear();
            return *this;
         }

      template <typename NodeU>
      std::enable_if_t<std::is_base_of_v<NodeT, NodeU>,
         parse_result&> operator = (parse_result<NodeU>&& other) {
            if (&other == this) return *this;
            _node = std::move(other._node);
            _errors = std::move(other._errors);
            return *this;
         }

      bool success() const noexcept {
         return !failed();
      }

      bool failed() const noexcept {
         return _node == nullptr;
      }

      error_range_type errors() && noexcept {
         return std::move(_errors);
      }

      const_error_range_type errors() const & noexcept {
         return _errors;
      }

      operator std::unique_ptr<NodeT>() && noexcept {
         return std::move(_node);
      }

      template <typename NodeU>
      parse_result<NodeU> as() && {
         if constexpr (!std::is_base_of_v<NodeT, NodeU>) {
            return std::move(_errors);
         } else {
            return success()
               ? parse_result<NodeU> { std::unique_ptr<NodeU> { dynamic_cast<NodeU*>(_node.release()) } }
               : parse_result<NodeU> { std::move(_errors) };
         }
      }

      void accept(ast::visitor& v) const {
         if (_node) _node->accept(v);
      }

      void accept(ast::visitor&& v) const {
         if (_node) _node->accept(std::move(v));
      }

   private:
      std::unique_ptr<NodeT> _node;
      std::vector<error_type> _errors;

      void attach(ast::context& ctx) {
         if (_node) _node->attach(ctx);
      }

      void detach(ast::context& ctx) {
         if (_node) _node->detach(ctx);
      }
   };

	class syntax_analysis {
		friend class parser;

	public:
      using error_range_type = rush::parse_result<ast::node>::const_error_range_type;

      error_range_type errors() const noexcept {
         return _result.errors();
      }

      void accept(ast::visitor& v) const {
         _result.accept(v);
      }

      void accept(ast::visitor&& v) const {
         _result.accept(std::move(v));
      }

	private:
		rush::parse_result<ast::node> _result;
      std::unique_ptr<ast::context> _context;

		syntax_analysis(std::unique_ptr<ast::context> ctx)
			: _context { std::move(ctx) }
         , _result { nullptr } {}

		syntax_analysis(std::unique_ptr<ast::context> ctx, rush::parse_result<ast::node> ast)
			: _context { std::move(ctx) }
			, _result { std::move(ast) } {
            if (!_context) throw std::invalid_argument(
               "null argument std::unique_ptr<ast::context> 'ctx'");
            _result.attach(*_context);
         }
	};
} // rush

#endif // RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
