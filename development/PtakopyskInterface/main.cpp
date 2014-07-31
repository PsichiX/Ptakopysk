#include "main.h"
#include "PtakopyskInterface.h"

void DLL_EXPORT Initialize( void* handle )
{
    PtakopyskInterface::use().initialize( (sf::WindowHandle)handle );
}

void DLL_EXPORT Release()
{
    PtakopyskInterface::use().release();
}

void DLL_EXPORT ProcessEvents()
{
    PtakopyskInterface::use().processEvents();
}

void DLL_EXPORT ProcessPhysics( float deltaTime, int velocityIterations, int positionIterations )
{
    PtakopyskInterface::use().processPhysics( deltaTime, velocityIterations, positionIterations );
}

void DLL_EXPORT ProcessUpdate( float deltaTime, bool sortInstances )
{
    PtakopyskInterface::use().processUpdate( deltaTime, sortInstances );
}

void DLL_EXPORT ProcessRender()
{
    PtakopyskInterface::use().processRender();
}

void DLL_EXPORT SetVerticalSyncEnabled( bool enabled )
{
    PtakopyskInterface::use().setVerticalSyncEnabled( enabled );
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            PtakopyskInterface::use();
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            PtakopyskInterface::destroy();
            break;
    }
    return TRUE; // succesful
}
