#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include "PtakopyskInterface.h"

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport) __stdcall
#else
    #define DLL_EXPORT __declspec(dllimport) __stdcall
#endif


#ifdef __cplusplus
extern "C"
{
#endif

const char* DLL_EXPORT _PopErrors();
bool DLL_EXPORT _Initialize( int64_t windowHandle, bool editMode );
void DLL_EXPORT _Release();
void DLL_EXPORT _SetAssetsFileSystemRoot( const char* path );
bool DLL_EXPORT _ProcessEvents();
bool DLL_EXPORT _ProcessPhysics( float deltaTime, int velocityIterations, int positionIterations );
bool DLL_EXPORT _ProcessUpdate( float deltaTime, bool sortInstances );
bool DLL_EXPORT _ProcessRender();
bool DLL_EXPORT _SetVerticalSyncEnabled( bool enabled );
float DLL_EXPORT _GetGridSizeX();
float DLL_EXPORT _GetGridSizeY();
void DLL_EXPORT _SetGridSize( float x, float y );
float DLL_EXPORT _GetSceneViewSizeX();
float DLL_EXPORT _GetSceneViewSizeY();
void DLL_EXPORT _SetSceneViewSize( float x, float y );
float DLL_EXPORT _GetSceneViewCenterX();
float DLL_EXPORT _GetSceneViewCenterY();
void DLL_EXPORT _SetSceneViewCenter( float x, float y );
float DLL_EXPORT _GetSceneViewZoom();
void DLL_EXPORT _SetSceneViewZoom( float zoom );
void DLL_EXPORT _ConvertPointFromScreenToWorldSpace( int x, int y );
float DLL_EXPORT _GetConvertedPointX();
float DLL_EXPORT _GetConvertedPointY();
bool DLL_EXPORT _ClearScene();
bool DLL_EXPORT _ClearSceneGameObjects( bool isPrefab );
bool DLL_EXPORT _ApplyJsonToScene( const char* json );
const char* DLL_EXPORT _ConvertSceneToJson();
int DLL_EXPORT _CreateGameObject( bool isPrefab, int parent, const char* prefabSource, const char* id );
bool DLL_EXPORT _DestroyGameObject( int handle, bool isPrefab );
bool DLL_EXPORT _ClearGameObject( int handle, bool isPrefab );
bool DLL_EXPORT _DuplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo );
bool DLL_EXPORT _TriggerGameObjectComponentFunctionality( int handle, bool isPrefab, const char* compId, const char* funcName );
bool DLL_EXPORT _ApplyJsonToGameObject( int handle, bool isPrefab, const char* json );
const char* DLL_EXPORT _ConvertGameObjectToJson( int handle, bool isPrefab );
bool DLL_EXPORT _StartQueryGameObject( int handle, bool isPrefab );
bool DLL_EXPORT _QueryGameObject( const char* query );
int DLL_EXPORT _QueriedGameObjectHandle();
unsigned int DLL_EXPORT _QueriedGameObjectResultsCount();
bool DLL_EXPORT _QueriedGameObjectResultNext();
const char* DLL_EXPORT _QueriedGameObjectResultKey();
const char* DLL_EXPORT _QueriedGameObjectResultValue();
void DLL_EXPORT _EndQueryGameObject();
bool DLL_EXPORT _StartIterateGameObjects( bool isPrefab );
bool DLL_EXPORT _CanIterateGameObjectsNext( bool isPrefab );
bool DLL_EXPORT _IterateGameObjectsNext( bool isPrefab );
bool DLL_EXPORT _StartQueryIteratedGameObject();
bool DLL_EXPORT _EndIterateGameObjects();
int DLL_EXPORT _FindGameObjectHandleById( const char* id, bool isPrefab, int parent );
int DLL_EXPORT _FindGameObjectHandleAtPosition( float x, float y, int parent );
int DLL_EXPORT _FindGameObjectHandleAtScreenPosition( int x, int y, int parent );
void DLL_EXPORT _StartIterateAssets( int type );
bool DLL_EXPORT _CanIterateAssetsNext( int type );
bool DLL_EXPORT _IterateAssetsNext( int type );
const char* DLL_EXPORT _GetIteratedAssetId( int type );
const char* DLL_EXPORT _GetIteratedAssetMeta( int type );
const char* DLL_EXPORT _GetIteratedAssetTags( int type );
void DLL_EXPORT _EndIterateAssets( int type );
bool DLL_EXPORT _QueryAssets( int type, const char* query );
int DLL_EXPORT _PluginLoad( const char* path );
bool DLL_EXPORT _PluginUnload( int handle );
bool DLL_EXPORT _PluginUnloadByPath( const char* path );
void DLL_EXPORT _StartIterateComponents();
bool DLL_EXPORT _CanIterateComponentsNext();
bool DLL_EXPORT _IterateComponentsNext();
const char* DLL_EXPORT _GetIteratedComponentId();
void DLL_EXPORT _EndIterateComponents();
void DLL_EXPORT _StartIterateCustomAssets();
bool DLL_EXPORT _CanIterateCustomAssetsNext();
bool DLL_EXPORT _IterateCustomAssetsNext();
const char* DLL_EXPORT _GetIteratedCustomAssetId();
void DLL_EXPORT _EndIterateCustomAssets();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
