#include "main.h"
#include <map>
#include <sstream>

///==========///

typedef bool (CALLBACK* _RegisterPlugin)();
typedef void (CALLBACK* _UnregisterPlugin)();
typedef const char* (CALLBACK* _QueryPlugin)( const char* );

struct PluginData
{
    HINSTANCE instance;
    _RegisterPlugin registerPlugin;
    _UnregisterPlugin unregisterPlugin;
    _QueryPlugin queryPlugin;

    FORCEINLINE PluginData() { clear(); };

    FORCEINLINE bool isValid() { return instance && registerPlugin && unregisterPlugin && queryPlugin; };

    FORCEINLINE void clear()
    {
        instance = 0;
        registerPlugin = 0;
        unregisterPlugin = 0;
        queryPlugin = 0;
    }
};

PluginData g_currentData;
std::string g_currentPath;
std::map< std::string, PluginData > g_plugins;
std::string g_stringCache;

///==========///

DLL_EXPORT bool _PluginLoad( const char* path )
{
    std::string p = path;
    if( g_plugins.count( p ) )
        return false;

    PluginData plugin;
    plugin.instance = LoadLibrary( p.c_str() );
    if( !plugin.instance )
        return false;

    plugin.registerPlugin = (_RegisterPlugin)GetProcAddress( plugin.instance, "_RegisterPlugin" );
    plugin.unregisterPlugin = (_UnregisterPlugin)GetProcAddress( plugin.instance, "_UnregisterPlugin" );
    plugin.queryPlugin = (_QueryPlugin)GetProcAddress( plugin.instance, "_QueryPlugin" );

    if( !plugin.isValid() )
    {
        FreeLibrary( plugin.instance );
        return false;
    }

    if( plugin.registerPlugin() )
    {
        g_plugins[ p ] = plugin;
        return true;
    }
    else
    {
        FreeLibrary( plugin.instance );
        return false;
    }
}

DLL_EXPORT bool _PluginUnload( const char* path )
{
    for( std::map< std::string, PluginData >::iterator it = g_plugins.begin(); it != g_plugins.end(); it++ )
    {
        if( it->first == path )
        {
            PluginData plugin = it->second;
            plugin.unregisterPlugin();
            FreeLibrary( plugin.instance );
            g_plugins.erase( it );
            g_currentData.clear();
            g_currentPath.clear();
            return true;
        }
    }
    return false;
}

DLL_EXPORT void _PluginUnloadAll()
{
    g_currentData.clear();
    g_currentPath.clear();
    for( std::map< std::string, PluginData >::iterator it = g_plugins.begin(); it != g_plugins.end(); it++ )
    {
        PluginData plugin = it->second;
        plugin.unregisterPlugin();
        FreeLibrary( plugin.instance );
    }
    g_plugins.clear();
}

DLL_EXPORT const char* _PluginListAll()
{
    std::stringstream ss;
    int i = 0;
    int c = (int)g_plugins.size();
    for( std::map< std::string, PluginData >::iterator it = g_plugins.begin(); it != g_plugins.end(); it++, i++ )
    {
        ss << it->first.c_str();
        if( i < c - 1 )
            ss << ";";
    }
    g_stringCache = ss.str();
    return g_stringCache.c_str();
}

DLL_EXPORT bool _PluginSetCurrent( const char* path )
{
    if( g_currentPath == path )
        return true;
    std::string p = path;
    if( g_plugins.count( p ) )
    {
        g_currentData = g_plugins[ p ];
        g_currentPath = p;
        return g_currentData.isValid();
    }
    return false;
}

DLL_EXPORT const char* _PluginGetCurrent()
{
    return g_currentData.isValid() ? g_currentPath.c_str() : "";
}

DLL_EXPORT const char* _PluginQuery( const char* query )
{
    if( g_currentData.isValid() )
    {
        g_stringCache = g_currentData.queryPlugin( query );
        return g_stringCache.c_str();
    }
    return "";
}

///==========///

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            _PluginUnloadAll();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
