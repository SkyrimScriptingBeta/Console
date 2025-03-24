#pragma once

/*
    These functions provided so you can easily interact with the console from your C++

    The source of these functions comes from ConsoleUtilSSS-NG:
    - https://github.com/VersuchDrei/ConsoleUtilSSE

    License: MIT
*/

#include <RE/Skyrim.h>

#include <string_view>

namespace SkyrimScripting::Console::Native {

    inline void ExecuteCommand(std::string_view commandText) {
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script        = scriptFactory ? scriptFactory->Create() : nullptr;
        if (script) {
            const auto selectedRef = RE::Console::GetSelectedRef();
            script->SetCommand(commandText.data());
            script->CompileAndRun(selectedRef.get());
            delete script;
        }
    }

    inline RE::TESObjectREFR* GetSelectedReference() {
        const auto selectedRef = RE::Console::GetSelectedRef();
        return selectedRef.get();
    }

    inline void SetSelectedReference(RE::TESObjectREFR* reference) {
        if (reference) {
            const auto factory          = RE::MessageDataFactoryManager::GetSingleton();
            const auto interfaceStrings = RE::InterfaceStrings::GetSingleton();
            const auto creator =
                factory && interfaceStrings
                    ? factory->GetCreator<RE::ConsoleData>(interfaceStrings->consoleData)
                    : nullptr;

            const auto consoleData  = creator ? creator->Create() : nullptr;
            const auto messageQueue = RE::UIMessageQueue::GetSingleton();
            if (consoleData && messageQueue) {
                consoleData->type    = static_cast<RE::ConsoleData::DataType>(1);
                consoleData->pickRef = reference->CreateRefHandle();
                messageQueue->AddMessage(
                    interfaceStrings->console, RE::UI_MESSAGE_TYPE::kUpdate, consoleData
                );
            }
        } else {
            const auto ui      = RE::UI::GetSingleton();
            const auto console = ui ? ui->GetMenu<RE::Console>() : nullptr;
            if (console) {
                const RE::ObjectRefHandle nothing;
                console->SetSelectedRef(nothing);
            }
        }
    }

    inline const char* GetLastConsoleOutput() {
        if (auto* consoleLog = RE::ConsoleLog::GetSingleton()) return consoleLog->lastMessage;
        else return nullptr;
    }
}
