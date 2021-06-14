#pragma once

#if defined(_MSC_VER)

#ifdef NativePlugin_EXPORTS
  #define NATIVEPLUGIN_API extern "C" __declspec(dllexport)
#else
  #define NATIVEPLUGIN_API extern "C" __declspec(dllimport)
#endif
#else
#define NATIVEPLUGIN_API extern "C"
#endif

NATIVEPLUGIN_API int get_number();

NATIVEPLUGIN_API bool is_windows();
