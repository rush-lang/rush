#pragma once

#ifndef RUSH_SEMA_GLOBAL_SCOPE_HPP
#define RUSH_SEMA_GLOBAL_SCOPE_HPP

namespace rush {
	class scope;

	scope& ensure_global_scope();
	extern scope& global_scope;
} // rush

#endif // RUSH_SEMA_GLOBAL_SCOPE_HPP
