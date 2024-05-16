#include "pch.h"
#include "CppUnitTest.h"
#include "../isrealrender/geometry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(true, isInside(Vec3f(0, 2), Vec3f(2, 2), Vec3f(1, 0), Vec3f(1, 1)));
		}

	};
}
