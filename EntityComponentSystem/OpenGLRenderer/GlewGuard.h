#pragma once

namespace RenderEngine {
	class GlewGuard
	{
	public:
		GlewGuard();
		~GlewGuard();
		GlewGuard(GlewGuard&) = delete;
		GlewGuard& operator=(GlewGuard&) = delete;
		GlewGuard(GlewGuard&&) = delete;
		GlewGuard& operator=(GlewGuard&&) = delete;
	};
}

