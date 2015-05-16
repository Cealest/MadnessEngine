/* Copyright 2015 Myles Salholm */

#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

#include <stdio.h>

/* Engine information. */
#define TITLE "Madness Engine v0.03"
#define SHORT_TITLE "MAD"
#define VERSION 0.02f
#define WITH_EDITOR 1
#define SHIPPING 0

/* Target platform. */
#define WINDOWS 1
#if WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif
#define UNIX 0;

/* Global engine constants. */
#define MS_PER_UPDATE (1000.0f / 120.0f)

/* Which graphics APIs we're compiling. */
#define DIRECTX 1
#define OPENGL 0

/* What networking structure we're using. */
#define SERVER 0
#define CLIENT 0
#define PEER_TO_PEER 0

/* Input settings. */
#define KEYBOARD 1
#define KEYS 256

/* Relative paths. */
#define CONTENTPATH "../../../Content/"
#define SOURCEPATH "../../../Source/"
#define INTERMEDIATEPATH "../../../Intermidiate/"
#define LOGPATH "../../../Content/Logs/"

/* Absolute paths, don't use in shipping. */
#define PATH_ABS "C:/MadnessEngine/MadnessEngine/"
#define CONTENTPATH_ABS PATH_ABS"Content/"
#define SOURCEPATH_ABS PATH_ABS"Source/"
#define INTERMEDIATEPATH_ABS PATH_ABS"Intermediate/"
#define LOGPATH_ABS INTERMEDIATEPATH_ABS"Logs/"

/* Math helpers */
#define RotToRad 0.0174532925f

/* Enum that describes shutdown reasons. */
namespace EShutdownReason
{
	enum Type
	{
		Quit,
		FailedToInit,
		FailedToRender,
		Unknown,
	};
}

#endif