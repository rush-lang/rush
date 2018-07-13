#pragma once

#ifndef RUSH_CORE_SOURCE_REGISTRY_HPP
#define RUSH_CORE_SOURCE_REGISTRY_HPP

#include <llvm/Support/SourceMgr.h>

namespace rush {
	class source_manager {
		llvm::SourceMgr _llvmMgr;
	};
} // rush

#endif // RUSH_CORE_SOURCE_REGISTRY_HPP
