#include "scope.hpp"

namespace rush {
	scope* scope::parent() const noexcept {
		return _parent;
	}

	bool scope::is_global() const noexcept {
		return _parent == nullptr;
	}

	bool scope::is_local() const noexcept {
		return !is_global();
	}

	std::size_t scope::depth() const noexcept {
		return is_local()
			? parent()->depth() + 1
			: 0;
	}

	void scope::attach(scope& sc) {
		sc._parent = this;
	}

	void scope::detach() {
		_parent = nullptr;
	}
}