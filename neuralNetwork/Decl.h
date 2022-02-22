#pragma once

#ifdef EXPORT_DECL
#define DECL __declspec(dllexport)
#else
#define DECL __declspec(dllimport)
#endif