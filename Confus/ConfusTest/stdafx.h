// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include <ConfusShared/Player.h>
#include <ConfusShared/Physics/PhysicsWorld.h>
#include <ConfusShared/Flag.h>
#include <ConfusShared/Physics/RigidBody.h>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<>
			static std::wstring ToString<irr::core::vector3df>(const irr::core::vector3df  & vector)
			{
				return std::to_wstring(vector.X) + L" " + std::to_wstring(vector.Y) + L" " + std::to_wstring(vector.Z);
			}
		}
	}
}

// TODO: reference additional headers your program requires here
