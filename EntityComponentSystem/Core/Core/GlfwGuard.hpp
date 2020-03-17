#pragma once

namespace renderer {
	class GlfwGuard
	{
	public:
		GlfwGuard();
		~GlfwGuard();
		GlfwGuard(GlfwGuard&) = delete;
		GlfwGuard& operator=(GlfwGuard&) = delete;
		GlfwGuard(GlfwGuard&&) = delete;
		GlfwGuard& operator=(GlfwGuard&&) = delete;
	};
}

