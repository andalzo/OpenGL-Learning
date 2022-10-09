#pragma once

namespace test
{
	class Test
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	};
}



