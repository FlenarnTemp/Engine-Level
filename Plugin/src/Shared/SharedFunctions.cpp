#include "SharedFunctions.h"

using namespace RE::BSScript;

namespace RE
{
	namespace Cascadia
	{
		namespace GFxUtilities
		{
			void RegisterArray(Scaleform::GFx::Value* destination, Scaleform::GFx::ASMovieRootBase* asMovieRoot, const char* name, Scaleform::GFx::Value* array)
			{
				asMovieRoot->CreateArray(array);
				destination->SetMember(name, array);
			}

			void RegisterString(Scaleform::GFx::Value* destination, Scaleform::Ptr<Scaleform::GFx::ASMovieRootBase> asMovieRoot, const char* name, const char* string)
			{
				Scaleform::GFx::Value fxValue;
				asMovieRoot->CreateString(&fxValue, string);
				destination->SetMember(name, &fxValue);
			}

			void RegisterNumber(Scaleform::GFx::Value* destination, const char* name, double value)
			{
				Scaleform::GFx::Value fxValue;
				fxValue = value;
				destination->SetMember(name, &fxValue);
			}

			void RegisterInt(Scaleform::GFx::Value* destination, const char* name, std::uint32_t value)
			{
				Scaleform::GFx::Value fxValue;
				fxValue = value;
				destination->SetMember(name, &fxValue);
			}

			void RegisterBool(Scaleform::GFx::Value* destination, const char* name, bool value)
			{
				Scaleform::GFx::Value fxValue;
				fxValue = value;
				destination->SetMember(name, &fxValue);
			}
		}

		// Takes a Float and converts it to a Precise String.
		// eg. FloatToPreciseString(3.141592, 2) would return "3.14"
		std::string FloatToPreciseString(float a_value, std::uint32_t a_precision)
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(a_precision) << a_value;
			return stream.str();
		}

		//	Converts a UInt8 percentage to float (eg. 38% becomes 0.38)
		float ConvertPercentageToFloat(std::uint8_t percentage)
		{
			return (percentage / static_cast<float>(100));
		}

		bool HasVMScript(TESForm* form, const char* scriptName)
		{
			BSTSmartPointer<BSScript::IVirtualMachine> vm = GameVM::GetSingleton()->GetVM();
			const BSScript::IObjectHandlePolicy& handlePolicy = vm->GetObjectHandlePolicy();
			std::size_t handle = handlePolicy.GetHandleForObject(form->formType.underlying(), form);

			BSTSmartPointer<Object> identifier;

			return vm->FindBoundObject(handle, scriptName, 1, identifier, 0);
		}

		std::string trim(const std::string& str) {
			size_t start = str.find_first_not_of(" \t\n\r");
			size_t end = str.find_last_not_of(" \t\n\r");
			return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
		}

		bool InMenuMode()
		{
			UI* ui = UI::GetSingleton();
			return (
				(ui->menuMode >= 1)
				|| ui->GetMenuOpen("CookingMenu")
				|| ui->GetMenuOpen("FaderMenu")
				|| ui->GetMenuOpen("FavoritesMenu")
				|| ui->GetMenuOpen("PowerArmorModMenu")
				|| ui->GetMenuOpen("RobotModMenu")
				|| ui->GetMenuOpen("VATSMenu")
				|| ui->GetMenuOpen("WorkshopMenu")
				|| ui->GetMenuOpen("DialogueMenu")
			);
		}

		std::uint32_t GetAvailableComponentCount(BGSInventoryList* a_list, TESForm* a_form)
		{
			auto reverseInv = a_list->data;
			std::reverse(reverseInv.begin(), reverseInv.end());

			std::uint32_t amount = 0;

			for (BGSInventoryItem& item : reverseInv)
			{
				if (item.object->formType == ENUM_FORM_ID::kMISC)
				{
					TESObjectMISC* miscItem = (TESObjectMISC*)(item.object);
					
					if (miscItem->componentData)
					{
						for (auto& componentData : *miscItem->componentData)
						{
							if (componentData.first == a_form)
							{
								std::uint32_t count = [item]
									{
										std::uint32_t _count = 0;
										BSTSmartPointer<BGSInventoryItem::Stack> pointer = item.stackData;

										while (pointer)
										{
											_count += pointer->GetCount();
											pointer = pointer->nextStack;
										}

										return _count;
									}();
								
								amount += componentData.second.i * count;
								break;
							}
						}
					}
				}
			}

			return amount;
		}

		bool IsXPMetervisible()
			{
				BSFixedString menuString("HUDMenu");
				IMenu* menu = UI::GetSingleton()->GetMenu(menuString).get();
				Scaleform::GFx::Value openValue;
				menu->uiMovie->asMovieRoot->GetVariable(&openValue, "root.HUDNotificationsGroup_mc.XPMeter_mc.visible");

				return openValue.GetBoolean();
			}
	}
}
