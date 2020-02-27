#pragma once
#include <vector>
#include <Object.h>
#include "Vbo.h"

namespace renderer::Util {
	
	const std::vector<Object<Vec3>> UNIT_CUBE_VERTICES = {
		Vec3{ -1.0f, -1.0f, -1.0f },
		Vec3{ +1.0f, -1.0f, -1.0f },
		Vec3{ -1.0f, +1.0f, -1.0f },
		Vec3{ -1.0f, +1.0f, -1.0f },
		Vec3{ +1.0f, +1.0f, -1.0f },
		Vec3{ +1.0f, -1.0f, -1.0f },

		Vec3{ -1.0f, -1.0f, +1.0f },
		Vec3{ +1.0f, -1.0f, +1.0f },
		Vec3{ -1.0f, +1.0f, +1.0f },
		Vec3{ -1.0f, +1.0f, +1.0f },
		Vec3{ +1.0f, +1.0f, +1.0f },
		Vec3{ +1.0f, -1.0f, +1.0f },

		Vec3{ -1.0f, -1.0f, -1.0f },
		Vec3{ -1.0f, +1.0f, -1.0f },
		Vec3{ -1.0f, -1.0f, +1.0f },
		Vec3{ -1.0f, -1.0f, +1.0f },
		Vec3{ -1.0f, +1.0f, +1.0f },
		Vec3{ -1.0f, +1.0f, -1.0f },

		Vec3{ +1.0f, -1.0f, -1.0f },
		Vec3{ +1.0f, +1.0f, -1.0f },
		Vec3{ +1.0f, -1.0f, +1.0f },
		Vec3{ +1.0f, -1.0f, +1.0f },
		Vec3{ +1.0f, +1.0f, +1.0f },
		Vec3{ +1.0f, +1.0f, -1.0f },

		Vec3{ -1.0f, -1.0f, -1.0f },
		Vec3{ +1.0f, -1.0f, -1.0f },
		Vec3{ -1.0f, -1.0f, +1.0f },
		Vec3{ -1.0f, -1.0f, +1.0f },
		Vec3{ +1.0f, -1.0f, +1.0f },
		Vec3{ +1.0f, -1.0f, -1.0f },

		Vec3{ -1.0f, +1.0f, -1.0f },
		Vec3{ +1.0f, +1.0f, -1.0f },
		Vec3{ -1.0f, +1.0f, +1.0f },
		Vec3{ -1.0f, +1.0f, +1.0f },
		Vec3{ +1.0f, +1.0f, +1.0f },
		Vec3{ +1.0f, +1.0f, -1.0f }
	};
}
