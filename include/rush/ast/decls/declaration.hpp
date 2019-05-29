#pragma once

#ifndef RUSH_AST_DECLS_DECLARATION_HPP
#define RUSH_AST_DECLS_DECLARATION_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/identifier.hpp"
#include "rush/ast/types/type.hpp"

#include <string>
#include <cstdint>
#include <vector>

namespace rush::ast {
	enum class declaration_kind : std::uint8_t {
      parameter,
		constant,
		variable,
		function,
		alias,
		enum_,
		class_,
		struct_,
		concept,
		interface,
		extension,
		operator_,
	};

	class declaration : public node {
		declaration(declaration const&) = delete;
		void operator = (declaration const&) = delete;

	public:
      virtual std::string name() const = 0;

      virtual ast::type_ref type() const = 0;

		virtual declaration_kind kind() const = 0;

      ast::identifier identifier() const {
         return { this };
      };

      struct resolver_callback_t {
         virtual ~resolver_callback_t() = default;
         virtual void operator ()(ast::declaration const*) const = 0;
      };

      /*! \brief When parsing some identifiers may appear before the associated
       *         declaration has been parsed.
       *         In which case an identifier expression is created with a resolver callback
       *         to resolve the identifier at the moment the declaration is inserted
       *         into the scope. */
      class resolver {
      public:
         void listen(resolver_callback_t* cb) {
            _cbs.push_back(cb);
         }

         void resolve(ast::declaration const* decl) {
            std::for_each(_cbs.begin(), _cbs.end(),
               [&decl](auto& cb) { cb->operator()(decl); });
         }

      private:
         std::vector<resolver_callback_t*> _cbs;
      };

   protected:
      declaration() = default;
	};
}

#endif // RUSH_AST_DECLS_DECLARATION_HPP
