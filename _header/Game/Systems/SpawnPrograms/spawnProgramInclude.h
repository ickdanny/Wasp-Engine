#pragma once

#include "SpawnProgramUtil.h"
#include "ScriptProgramUtil.h"
#include "Game/Systems/EntityBuilder.h"
#include "Game/Resources/BitmapStorage.h"

#ifndef SPAWN_PROGRAM_TYPEDEFS
#define SPAWN_PROGRAM_TYPEDEFS \
	using Angle = math::Angle; \
	using AABB = math::AABB; \
	using Point2 = math::Point2; \
	using Vector2 = math::Vector2; \
	using ComponentTupleSharedPointer = std::shared_ptr<ComponentTupleBase>; \
	using SpawnInstructions = components::SpawnInstructions; \
	using SpawnNode = components::SpawnNode; \
	template <typename T> \
	using SpawnNodeData = components::SpawnNodeData<T>; \
	using SpawnProgram = components::SpawnProgram; \
	using SpawnNodeSharedPointer = std::shared_ptr<SpawnNode>; \
	using ScriptInstructions = components::ScriptInstructions; \
	using ScriptNode = components::ScriptNode; \
	template <typename Internal, typename External> \
	using ScriptNodeData = components::ScriptNodeData<Internal, External>; \
	using ScriptProgram = components::ScriptProgram; \
	using ScriptNodeSharedPointer = std::shared_ptr<ScriptNode>;
#endif