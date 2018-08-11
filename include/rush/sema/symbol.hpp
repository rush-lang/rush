#pragma once

#ifndef RUSH_SEMA_SYMBOL_HPP
#define RUSH_SEMA_SYMBOL_HPP

#include <string>

namespace rush {
	class symbol {
	public:
		std::string name() const noexcept {
			return "";
		}

		bool is_type() const noexcept {
			return is_defined()
				&& (is_function()
				|| false);
		}

		bool is_function() const noexcept {
			return is_defined() && false;
		}

		bool is_constant() const noexcept {
			return is_defined() && false;
		}

		bool is_variable() const noexcept {
			return is_defined() && false;
		}

		bool is_builtin() const noexcept {
			return true;
		}

		bool is_defined() const noexcept {
			return false;
		}
	};

	symbol make_builtin_symbol(std::string name);
	symbol make_constant_symbol(std::string name);
	symbol make_variable_symbol(std::string name);
	symbol make_function_symbol(std::string name);
}

#endif // RUSH_SEMA_SYMBOL_HPP
