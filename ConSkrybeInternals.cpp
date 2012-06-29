#pragma warning(disable : 4005)
#include "ConSkrybeInternals.h"

IDebugLog						gLog("ConSkrybe.log");
PluginHandle					g_pluginHandle = kPluginHandle_Invalid;

_DefineHookHdlr(ConsoleExecuteCommandCallback, 0x00841198);
_DefineHookHdlr(ConsolePrintHandler, 0x00842848);

void StartSkrybing()
{
	_MemHdlr(ConsoleExecuteCommandCallback).WriteJump();
	_MemHdlr(ConsolePrintHandler).WriteJump();
}

void __stdcall ConsoleSiphon(const char* SiphonedText, bool UserInput)
{
	if (strlen(SiphonedText))
	{
		if (UserInput)
			gLog.Outdent();

		_MESSAGE("%s", SiphonedText);

		if (UserInput)
			gLog.Indent();
	}
}

#define _hhName	ConsoleExecuteCommandCallback
_hhBegin()
{
	_hhSetVar(Retn, 0x0084119D);
	_hhSetVar(Call, 0x005113F0);
	__asm
	{
		pushad
		push	1
		push	eax
		call	ConsoleSiphon
		popad

		call	[_hhGetVar(Call)]
		jmp		[_hhGetVar(Retn)]
	}
}

#define _hhName	ConsolePrintHandler
_hhBegin()
{
	_hhSetVar(Retn, 0x0084284D);
	_hhSetVar(Call, 0x00F4A171);
	__asm
	{
		call	[_hhGetVar(Call)]

		pushad
		push	0
		push	ebx
		call	ConsoleSiphon
		popad

		jmp		[_hhGetVar(Retn)]
	}
}