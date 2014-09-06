#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport) __stdcall
#else
    #define DLL_EXPORT __declspec(dllimport) __stdcall
#endif


#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool _PluginLoad( const char* path );
DLL_EXPORT bool _PluginUnload( const char* path );
DLL_EXPORT void _PluginUnloadAll();
DLL_EXPORT const char* _PluginListAll();
DLL_EXPORT bool _PluginSetCurrent( const char* path );
DLL_EXPORT const char* _PluginGetCurrent();
DLL_EXPORT const char* _PluginQuery( const char* query );
DLL_EXPORT const char* _PluginErrors();

#ifdef __cplusplus
}
#endif

#endif
