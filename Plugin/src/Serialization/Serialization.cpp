#include "Serialization.h"

namespace Cascadia
{
	/**namespace Serialization
	{
		auto CASSerialization::GetSingleton() -> CASSerialization*
		{
			static CASSerialization singleton{};
			return std::addressof(singleton);
		}

		/* Level Up Menu Related *
		std::int32_t playerSkillPoints = 0;
		std::set<std::uint32_t> taggedSkills;
		bool playerReadyToLevelUp = false;

		void CASSerialization::Serialize(F4SE::SerializationInterface* a_f4se)
		{
			DEBUG("Serialize save data.");
			Json::StreamWriterBuilder builder;
			builder["commentStyle"] = "None";
			builder["indentation"] = "";

			const std::unique_ptr<Json::StreamWriter> writer = std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
			Json::OStringStream ss;
			Json::Value value;

			writer->write(value, std::addressof(ss));
			auto str = ss.str();

			a_f4se->WriteRecord(
				SerializationType,
				SerializationVersion,
				str.data(),
				static_cast<std::uint32_t>(str.size()));
			
		}

		void CASSerialization::Deserialize(F4SE::SerializationInterface* a_f4se)
		{
			DEBUG("Deserialize save data.");
			std::uint32_t type;
			std::uint32_t version;
			std::uint32_t length;

			a_f4se->GetNextRecordInfo(type, version, length);

			if (type != SerializationType)
			{
				FATAL("Serialization type mismatch.");
				return;
			}

			if (version != SerializationVersion)
			{
				FATAL("Serialization version mismatch.");
				return;
			}

			Json::CharReaderBuilder builder;
			const auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());

			std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length);
			a_f4se->ReadRecordData(buffer.get(), length);

			std::unique_ptr<char[]>::pointer begin = buffer.get();
			std::unique_ptr<char[]>::pointer end = begin + length;

			Json::Value value;
			std::string errors;

			if (!reader->parse(begin, end, std::addressof(value), nullptr))
			{
				FATAL("Failed to parse serialized data.");
				return;
			}

			if (!value.isObject())
			{
				FATAL("Failed to parse serialized data.");
				return;
			}
		}

		void CASSerialization::Revert()
		{
			DEBUG("Revert serialization data.");
			playerSkillPoints = 0;
			playerReadyToLevelUp = false;
			taggedSkills.clear();
		}
	}*/
}


