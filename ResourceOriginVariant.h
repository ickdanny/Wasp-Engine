#pragma once

#include <variant>

#include "FileLoadable.h"
#include "ManifestLoadable.h"
namespace resource {
	using ResourceOriginVariant = std::variant<FileOrigin, ManifestOrigin>;
}