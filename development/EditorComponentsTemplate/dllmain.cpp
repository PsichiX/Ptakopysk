#include <windows.h>
#include <PtakopyskInterface/PtakopyskInterface.h>
#include "__components_headers_list__generated__.h"
#include "__assets_headers_list__generated__.h"

#define DLL_EXPORT __declspec(dllexport) __stdcall
#define REGISTER_COMPONENT(id, typename) g_interface->pluginRegisterComponent( id, RTTI_CLASS_TYPE( typename ), typename::onBuildComponent )
#define UNREGISTER_COMPONENT(id) g_interface->pluginUnregisterComponent( id )
#define REGISTER_ASSET(id, typename) g_interface->pluginRegisterAsset( id, RTTI_CLASS_TYPE( typename ), typename::onBuildCustomAsset )
#define UNREGISTER_ASSET(id) g_interface->pluginUnregisterAsset( id )

PtakopyskInterface* g_interface;

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT bool _UnregisterPlugin()
{
    if( !g_interface )
        return false;

#include "__unregister_components__generated__.inl"
#include "__unregister_assets__generated__.inl"

    g_interface = 0;
    return true;
}

DLL_EXPORT bool _RegisterPlugin( int interfacePtr )
{
    g_interface = (PtakopyskInterface*)interfacePtr;
    if( !g_interface )
        return false;

#include "__register_components__generated__.inl"
#include "__register_assets__generated__.inl"

    Ptakopysk::Assets::makeSharedFrom( g_interface->getAssetsInstance() );
    return true;
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
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
