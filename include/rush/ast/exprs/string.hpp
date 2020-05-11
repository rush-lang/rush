#pragma once

#ifndef RUSH_AST_EXPRS_STRING_HPP
#define RUSH_AST_EXPRS_STRING_HPP

#include "rush/ast/exprs/expression.hpp"
#include "rush/ast/types/string.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <algorithm>

namespace rush::ast::exprs {
   std::unique_ptr<ast::string_template_expression>
   string_template(
      std::string original,
      std::vector<std::unique_ptr<ast::expression>> parts);
} // rush::ast::exprs

namespace rush::ast {
   class string_template_expression : public ast::expression {
      struct factory_tag_t {};
      friend std::unique_ptr<ast::string_template_expression>
             exprs::string_template(
               std::string original,
               std::vector<std::unique_ptr<ast::expression>> parts);
   public:
      string_template_expression(
         std::string original,
         std::vector<std::unique_ptr<ast::expression>> parts,
         factory_tag_t)
         : _template { std::move(original) }
         , _parts { std::move(parts) }
         , _type { ast::types::undefined } {}

         std::string_view original() const noexcept {
            return _template;
         }

         std::vector<std::unique_ptr<ast::expression>> const& parts() const noexcept {
            return _parts;
         }

         virtual ast::type_ref result_type() const noexcept override {
            return _type;
         };

         using node::accept;
         virtual void accept(ast::visitor& v) const override {
            v.visit_string_template_expr(*this);
         }

         virtual void attach(ast::node& parent, ast::context& context) override {
            _type = context.string_type();
            std::for_each(_parts.begin(), _parts.end(),
               [&parent, &context](auto& elem) { elem->attach(parent, context); });
         }

         virtual void detach(ast::node& parent, ast::context& context) override {
            _type = ast::types::undefined;
            std::for_each(_parts.begin(), _parts.end(),
               [&parent, &context](auto& elem) { elem->detach(parent, context); });
         }

   private:
      std::string _template;
      std::vector<std::unique_ptr<ast::expression>> _parts;
      ast::type_ref _type;
   };
} // rush::ast

namespace rush::ast::exprs {
   inline std::unique_ptr<ast::string_template_expression>
   string_template(
      std::string original,
      std::vector<std::unique_ptr<ast::expression>> parts) {
         return std::make_unique<ast::string_template_expression>(
            std::move(original), std::move(parts),
            string_template_expression::factory_tag_t {});
      }
} // rush::ast::exprs


#endif // RUSH_AST_EXPRS_STRING_HPP
