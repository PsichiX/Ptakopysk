#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT Initialize( void* handle );
void DLL_EXPORT Release();
void DLL_EXPORT ProcessEvents();
void DLL_EXPORT ProcessPhysics( float deltaTime, int velocityIterations, int positionIterations );
void DLL_EXPORT ProcessUpdate( float deltaTime, bool sortInstances );
void DLL_EXPORT ProcessRender();
void DLL_EXPORT SetVerticalSyncEnabled( bool enabled );

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
