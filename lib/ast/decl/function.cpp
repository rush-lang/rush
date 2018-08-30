#include "rush/ast/decls/function.hpp"
#include "rush/ast/statement.hpp"

namespace rush::ast {

	function_declaration::function_declaration(
		sema::symbol symbol,
		std::vector<std::unique_ptr<parameter>> params,
		std::unique_ptr<statement> body,
		factory_tag_t)
			: _symbol(symbol)
			, _body(std::move(body))
			, _params(std::move(params)) {}

	namespace decls {
		std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<statement> body) {
				return decls::function(
					rush::global_scope,
					std::move(name),
					std::move(body));
			}

		std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<statement> body) {
				return decls::function(
					rush::global_scope,
					std::move(name),
					return_type,
					std::move(body));
			}

		std::unique_ptr<function_declaration> function(
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					rush::global_scope,
					std::move(name),
					std::move(params),
					std::move(body));
			}

		std::unique_ptr<function_declaration> function(
			std::string name,
			ast::type return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return decls::function(
					rush::global_scope,
					std::move(name),
					return_type,
					std::move(params),
					std::move(body));
			}

		std::unique_ptr<function_declaration> function(
			rush::scope& sc,
			std::string name,
			std::unique_ptr<statement> body) {
				return nullptr;
			}

		std::unique_ptr<function_declaration> function(
			rush::scope& sc,
			std::string name,
			ast::type return_type,
			std::unique_ptr<statement> body) {
				return nullptr;
			}

		std::unique_ptr<function_declaration> function(
			rush::scope& sc,
			std::string name,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return nullptr;
			}

		std::unique_ptr<function_declaration> function(
			rush::scope& sc,
			std::string name,
			ast::type return_type,
			std::unique_ptr<parameter_list> params,
			std::unique_ptr<statement> body) {
				return nullptr;
			}
	}
}