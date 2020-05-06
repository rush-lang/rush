
#pragma once

#ifndef RUSH_DECLS_UNDECLARED_HPP
#define RUSH_DECLS_UNDECLARED_HPP


#include "rush/ast/types/builtin.hpp"
#include "rush/ast/decls/declaration.hpp"

namespace rush::ast {

   //! \brief Sentinel type that is used as a substitute for
   //         identifiers that were not resolved after parsing,
   //         and the scope chain has been destroyed.
   class undeclared_identifier : public ast::declaration {
   public:
      undeclared_identifier(std::string name)
         : _name { std::move(name) } {}

      virtual std::string_view name() const {
         return _name;
      };

      virtual ast::type_ref type() const {
         return ast::types::undeclared;
      }

      virtual ast::declaration_kind kind() const {
         return ast::declaration_kind::undeclared;
      }

      using node::accept;
      virtual void accept(ast::visitor&) const {};

      virtual void attach(ast::node&, ast::context&) {};
      virtual void detach(ast::node&, ast::context&) {};

   private:
      std::string _name;
   };
}

#endif // RUSH_DECLS_UNDECLARED_HPP
