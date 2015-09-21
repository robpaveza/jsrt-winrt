#pragma once

#include <collection.h>
#include <ppltasks.h>

// To support IE10 or IE11 mode, don't define USE_EDGEMODE_JSRT
// This will present a strong compatibility guarantee but won't allow the script
// engine to version as the system's chakra.dll evolves.
#define USE_EDGEMODE_JSRT
#include <jsrt.h>

