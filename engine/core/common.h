#pragma once
#include <cstdint>

#include <cassert>

#include "primitive_types.h"
#include "id.h"

#include "spdlog/spdlog.h"

namespace ecs::entity{
	DEFINE_TYPED_ID(entity);
}

namespace ecs::components::transform{
	DEFINE_TYPED_ID(transform);
}

namespace ecs::components::point_light {
	DEFINE_TYPED_ID(point_light);
}
