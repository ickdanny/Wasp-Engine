#pragma once

#include <variant>

#include "FileLoadable.h"
#include "ManifestLoadable.h"
namespace resource {
	using InternalOrigin = std::monostate;
	using ResourceOriginVariant 
		= std::variant<FileOrigin, ManifestOrigin, InternalOrigin>;
}