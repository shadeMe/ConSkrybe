#pragma warning(disable : 4005)
#include "ConSkrybeInternals.h"

IDebugLog						gLog("ConSkrybe.log");
PluginHandle					g_pluginHandle = kPluginHandle_Invalid;

static const char*				kToggleConSkrybeCommandName = "ToggleSkrybing";
static bool						g_EnableSkrybing = true;

_DefineHookHdlr(ConsoleExecuteCommandCallback, 0x00847758);
_DefineHookHdlr(ConsolePrintHandler, 0x00848E08);
_DefinePatchHdlr(ToggleCmdAlias, 0x0124CE6C);
_DefinePatchHdlr(ToggleCmdHandler, 0x0124CE80);

void StartSkrybing()
{
	_MemHdlr(ConsoleExecuteCommandCallback).WriteJump();
	_MemHdlr(ConsolePrintHandler).WriteJump();
	_MemHdlr(ToggleCmdAlias).WriteUInt32((UInt32)kToggleConSkrybeCommandName);
	_MemHdlr(ToggleCmdHandler).WriteUInt32((UInt32)ToggleConsoleOutput);
}

void __stdcall ConsoleSiphon(const char* SiphonedText, bool UserInput)
{
	if (g_EnableSkrybing && strlen(SiphonedText))
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
	_hhSetVar(Retn, 0x0084775D);
	_hhSetVar(Call, 0x00513950);
	__asm
	{
		pushad
		push	1
		push	eax
		call	ConsoleSiphon
		popad

		call	_hhGetVar(Call)
		jmp		_hhGetVar(Retn)
	}
}

#define _hhName	ConsolePrintHandler
_hhBegin()
{
	_hhSetVar(Retn, 0x00848E0D);
	_hhSetVar(Call, 0x00F51831);
	__asm
	{
		call	_hhGetVar(Call)

		pushad
		push	0
		push	ebx
		call	ConsoleSiphon
		popad

		jmp		_hhGetVar(Retn)
	}
}

bool ToggleConsoleOutput( void* paramInfo,
						 void * arg1,
						 TESObjectREFR* thisObj,
						 UInt32 arg3,
						 Script* scriptObj,
						 void* eventList,
						 double* result,
						 UInt32* opcodeOffsetPtr )
{
	if (g_EnableSkrybing)
	{
		Console_Print("Suspended ConSkrybing");
		g_EnableSkrybing = false;
	}
	else
	{
		g_EnableSkrybing = true;
		Console_Print("Resumed ConSkrybing");
	}

	return true;
}
