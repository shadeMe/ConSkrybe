#pragma once

#include "skse/PluginAPI.h"
#include "skse/skse_version.h"
#include "skse/GameTypes.h"
#include "skse/GameForms.h"

#include "[Libraries]\SME Sundries\SME_Prefix.h"
#include "[Libraries]\SME Sundries\MemoryHandler.h"

using namespace SME;
using namespace SME::MemoryHandler;

extern IDebugLog		gLog;
extern PluginHandle		g_pluginHandle;

_DeclareMemHdlr(ConsoleExecuteCommandCallback, "if you're Vorians and you're reading this, 'meh'");
_DeclareMemHdlr(ConsolePrintHandler, "more spurious hackery");

void StartSkrybing(void);