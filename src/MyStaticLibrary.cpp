#include "MyStaticLibrary.h"

#include <RE/Skyrim.h>

MyStaticLibrary::MyStaticLibrary() {}
MyStaticLibrary::~MyStaticLibrary() {}

void MyStaticLibrary::PrintSomethingInConsole() {
    if (auto* consoleLog = RE::ConsoleLog::GetSingleton())
        consoleLog->Print("Hello from MyStaticLibrary!");
}
