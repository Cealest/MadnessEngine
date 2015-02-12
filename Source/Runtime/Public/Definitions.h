/* Copyright 2015 Myles Salholm */

#ifndef GLOBAL_DEFINITIONS
#define GLOBAL_DEFINITIONS

/* Engine information. */
#define TITLE "Madness Engine v0.01"
#define SHORT_TITLE "MAD"
#define VERSION 0.01f
#define WITH_EDITOR 1
/* Target platform. */
#define WINDOWS 1
#if WINDOWS
#define WIN32_LEAN_AND_MEAN
#endif
/* Which graphics API we're using. */
#define DIRECTX 1
#define OPENGL 0
/* What networking structure we're using. */
#define SERVER 0
#define CLIENT 0
#define PEER_TO_PEER 0

#endif