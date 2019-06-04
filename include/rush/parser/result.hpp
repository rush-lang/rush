#pragma once

#ifndef RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
#define RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP

#include "fmt/format.h"

#include "rush/ast/node.hpp"
#include "rush/ast/visitor.hpp"
#include "rush/core/location.hpp"
#include "rush/core/iterator_range.hpp"

#include <memory>
#include <vector>
#include <type_traits>

namespace rush {
   class parser;
	class parser_options;
	class lexical_analysis;

   template <typename NodeT>
   class parse_result;


   struct syntax_error {
      friend class parser;

      syntax_error(syntax_error const&) = delete;
      void operator = (syntax_error const&) = delete;

   public:
      syntax_error(std::string msg, rush::location loc)
         : _msg { std::move(msg) }
         , _loc { std::move(loc) } {}

      syntax_error(syntax_error&&) = default;
      syntax_error& operator = (syntax_error&&) = default;

      std::string const& message() const noexcept { return _msg; }
      rush::location const& location() const noexcept { return _loc; }

   private:
      std::string _msg;
      rush::location _loc;
   };

   template <typename NodeT>
   class parse_result {
      friend class parser;

      parse_result(parse_result const&) = delete;
      void operator = (parse_result const&) = delete;

      template <typename NodeU>
      friend class parse_result;

   public:
      using error_range_type = rush::iterator_range<
         std::vector<syntax_error>::const_iterator>;

      parse_result()
         : _node { nullptr }
         , _errors { } {}

      parse_result(std::nullptr_t)
         : _node { nullptr }
         , _errors { } {}

      parse_result(rush::syntax_error err)
         : _node { nullptr }
         , _errors { } { _errors.push_back(std::move(err)); }

      template <typename NodeU, typename = std::enable_if_t<std::is_base_of_v<NodeT, NodeU>>>
      parse_result(std::unique_ptr<NodeU>&& node)
         : _node { std::move(node) }
         , _errors { } {}

      template <typename NodeU, typename = std::enable_if_t<std::is_base_of_v<NodeT, NodeU>>>
      parse_result(parse_result<NodeU>&& other)
         : _node { std::move(other._node) }
         , _errors { std::move(other._errors) } {}

      parse_result& operator = (rush::syntax_error err) {
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

      operator std::unique_ptr<NodeT>() && noexcept {
         return std::move(_node);
      }

      error_range_type errors() const noexcept {
         return _errors;
      }

      template <typename NodeU>
      parse_result<NodeU> as() && {
         // _node = std::move(other._node);
         auto result = parse_result<NodeU> {};
         std::move(
            _errors.begin(),
            _errors.end(),
            std::back_inserter(result._errors));
         return std::move(result);
      }

      void accept(ast::visitor& v) const {
         if (_node) _node->accept(v);
      }

      void accept(ast::visitor&& v) const {
         if (_node) _node->accept(std::move(v));
      }

   private:
      std::unique_ptr<NodeT> _node;
      std::vector<rush::syntax_error> _errors;
   };

	class syntax_analysis {
		friend syntax_analysis parse(
			lexical_analysis const&,
			parser_options const&);

	public:
      using error_range_type = rush::parse_result<ast::node>::error_range_type;


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

		syntax_analysis()
			: _result { nullptr } {}

		syntax_analysis(rush::parse_result<ast::node> ast)
			: _result { std::move(ast) } {}
	};
} // rush

#endif // RUSH_AST_ABSTRACT_SYNTAX_TREE_HPP
