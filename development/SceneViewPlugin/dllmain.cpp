#include <windows.h>
#include <SceneViewInterface/SceneViewInterface.h>
#include "__components_headers_list__generated__.h"
#include "__assets_headers_list__generated__.h"

#define DLL_EXPORT __declspec(dllexport) __stdcall
#define REGISTER_COMPONENT(id, typename) g_interface->registerComponentFactory( id, RTTI_CLASS_TYPE( typename ), typename::onBuildComponent )
#define UNREGISTER_COMPONENT(id) g_interface->unregisterComponentFactory( id )
#define REGISTER_ASSET(id, typename) g_interface->registerCustomAssetFactory( id, RTTI_CLASS_TYPE( typename ), typename::onBuildCustomAsset )
#define UNREGISTER_ASSET(id) g_interface->unregisterCustomAssetFactory( id )

SceneViewInterface* g_interface = 0;
std::string g_stringCache;

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool _RegisterPlugin()
{
    DELETE_OBJECT( g_interface );
    g_interface = xnew SceneViewInterface();

#include "__register_components__generated__.inl"
#include "__register_assets__generated__.inl"

    return true;
}

DLL_EXPORT void _UnregisterPlugin()
{
    if( !g_interface )
        return;

#include "__unregister_components__generated__.inl"
#include "__unregister_assets__generated__.inl"

    DELETE_OBJECT( g_interface );
}

DLL_EXPORT const char* _QueryPlugin( const char* query )
{
    if( !g_interface )
        return "";

    Json::Value root;
    Json::Reader reader;
    Json::Value result;
    if( reader.parse( query, root ) && root.isObject() )
    {
        g_interface->query( root, result );
        Json::FastWriter writer;
        g_stringCache = writer.write( result );
        return g_stringCache.c_str();
    }
    else
        return "";
}

#ifdef __cplusplus
}
#endif

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            _UnregisterPlugin();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
