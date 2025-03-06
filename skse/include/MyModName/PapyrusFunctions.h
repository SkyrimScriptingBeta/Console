#include <RE/Skyrim.h>

namespace ChooseYourOwnAdventure {
    namespace PapyrusFunctions {
        bool GetPlayerChoice(RE::StaticFunctionTag*, RE::BSFixedString a_choice) {
            if (a_choice == "Yes") {
                return true;
            } else {
                return false;
            }
        }

        bool BindFunctions(RE::BSScript::IVirtualMachine* vm) {
            vm->RegisterFunction("GetPlayerChoice", "ChooseYourOwnAdventure", GetPlayerChoice);
            return true;
        }
    }
}
