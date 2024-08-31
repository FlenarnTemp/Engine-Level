#pragma once

#include "F4SE/Interfaces.h"
#include "Json.h"
#include "Shared/SharedFunctions.h"

namespace Cascadia
{
	namespace Serialization
	{
		constexpr std::int32_t ID = 'CAS';

		class CASSerialization
		{
		public:
			~CASSerialization() = default;

			static auto GetSingleton() -> CASSerialization*;

			void Serialize(F4SE::SerializationInterface* a_f4se);

			void Deserialize(F4SE::SerializationInterface* a_f4se);

			void Revert();
		
		private:
			static constexpr std::uint32_t SerializationVersion = 1;
			static constexpr std::uint32_t SerializationType = 'CAS';
		};
	}
}
