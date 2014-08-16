#include "main.h"
#include "PtakopyskInterface.h"

static std::string s_lastErrors;
static std::string s_lastString;
static sf::Vector2f s_convertedPoint;

const char* DLL_EXPORT _PopErrors()
{
    s_lastErrors = PtakopyskInterface::use().popErrors();
    return s_lastErrors.c_str();
}

bool DLL_EXPORT _Initialize( int windowHandle, bool editMode )
{
    return PtakopyskInterface::use().initialize( windowHandle, editMode );
}

void DLL_EXPORT _Release()
{
    PtakopyskInterface::use().release();
}

void DLL_EXPORT _SetAssetsFileSystemRoot( const char* path )
{
    std::string p = path;
    PtakopyskInterface::use().setAssetsFileSystemRoot( p );
}

bool DLL_EXPORT _ProcessEvents()
{
    return PtakopyskInterface::use().processEvents();
}

bool DLL_EXPORT _ProcessPhysics( float deltaTime, int velocityIterations, int positionIterations )
{
    return PtakopyskInterface::use().processPhysics( deltaTime, velocityIterations, positionIterations );
}

bool DLL_EXPORT _ProcessUpdate( float deltaTime, bool sortInstances )
{
    return PtakopyskInterface::use().processUpdate( deltaTime, sortInstances );
}

bool DLL_EXPORT _ProcessRender()
{
    return PtakopyskInterface::use().processRender();
}

bool DLL_EXPORT _SetVerticalSyncEnabled( bool enabled )
{
    return PtakopyskInterface::use().setVerticalSyncEnabled( enabled );
}

float DLL_EXPORT _GetGridSizeX()
{
    return PtakopyskInterface::use().getGridSize().x;
}

float DLL_EXPORT _GetGridSizeY()
{
    return PtakopyskInterface::use().getGridSize().y;
}

void DLL_EXPORT _SetGridSize( float x, float y )
{
    PtakopyskInterface::use().setGridSize( sf::Vector2f( x, y ) );
}

float DLL_EXPORT _GetSceneViewSizeX()
{
    return PtakopyskInterface::use().getSceneViewSize().x;
}

float DLL_EXPORT _GetSceneViewSizeY()
{
    return PtakopyskInterface::use().getSceneViewSize().y;
}

void DLL_EXPORT _SetSceneViewSize( float x, float y )
{
    PtakopyskInterface::use().setSceneViewSize( sf::Vector2f( x, y ) );
}

float DLL_EXPORT _GetSceneViewCenterX()
{
    return PtakopyskInterface::use().getSceneViewCenter().x;
}

float DLL_EXPORT _GetSceneViewCenterY()
{
    return PtakopyskInterface::use().getSceneViewCenter().y;
}

void DLL_EXPORT _SetSceneViewCenter( float x, float y )
{
    PtakopyskInterface::use().setSceneViewCenter( sf::Vector2f( x, y ) );
}

float DLL_EXPORT _GetSceneViewZoom()
{
    return PtakopyskInterface::use().getSceneViewZoom();
}

void DLL_EXPORT _SetSceneViewZoom( float zoom )
{
    PtakopyskInterface::use().setSceneViewZoom( zoom );
}

void DLL_EXPORT _ConvertPointFromScreenToWorldSpace( int x, int y )
{
    s_convertedPoint = PtakopyskInterface::use().convertPointFromScreenToWorldSpace( sf::Vector2i( x, y ) );
}

float DLL_EXPORT _GetConvertedPointX()
{
    return s_convertedPoint.x;
}

float DLL_EXPORT _GetConvertedPointY()
{
    return s_convertedPoint.y;
}

bool DLL_EXPORT _ClearScene()
{
    return PtakopyskInterface::use().clearScene();
}

bool DLL_EXPORT _ClearSceneGameObjects( bool isPrefab )
{
    return PtakopyskInterface::use().clearSceneGameObjects( isPrefab );
}

bool DLL_EXPORT _ApplyJsonToScene( const char* json )
{
    std::string j = json;
    return PtakopyskInterface::use().applyJsonToScene( j );
}

const char* DLL_EXPORT _ConvertSceneToJson()
{
    s_lastString = PtakopyskInterface::use().convertSceneToJson();
    return s_lastString.c_str();
}

int DLL_EXPORT _CreateGameObject( bool isPrefab, int parent, const char* prefabSource, const char* id )
{
    std::string ps = prefabSource;
    std::string i = id;
    return PtakopyskInterface::use().createGameObject( isPrefab, parent, ps, i );
}

bool DLL_EXPORT _DestroyGameObject( int handle, bool isPrefab )
{
    return PtakopyskInterface::use().destroyGameObject( handle, isPrefab );
}

bool DLL_EXPORT _ClearGameObject( int handle, bool isPrefab )
{
    return PtakopyskInterface::use().clearGameObject( handle, isPrefab );
}

bool DLL_EXPORT _DuplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo )
{
    return PtakopyskInterface::use().duplicateGameObject( handleFrom, isPrefabFrom, handleTo, isPrefabTo );
}

bool DLL_EXPORT _TriggerGameObjectComponentFunctionality( int handle, bool isPrefab, const char* compId, const char* funcName )
{
    std::string c = compId;
    std::string f = funcName;
    return PtakopyskInterface::use().triggerGameObjectComponentFunctionality( handle, isPrefab, c, f );
}

bool DLL_EXPORT _ApplyJsonToGameObject( int handle, bool isPrefab, const char* json )
{
    std::string j = json;
    return PtakopyskInterface::use().applyJsonToGameObject( handle, isPrefab, json );
}

const char* DLL_EXPORT _ConvertGameObjectToJson( int handle, bool isPrefab )
{
    s_lastString = PtakopyskInterface::use().convertGameObjectToJson( handle, isPrefab );
    return s_lastString.c_str();
}

bool DLL_EXPORT _StartQueryGameObject( int handle, bool isPrefab )
{
    return PtakopyskInterface::use().startQueryGameObject( handle, isPrefab );
}

bool DLL_EXPORT _QueryGameObject( const char* query )
{
    std::string q = query;
    return PtakopyskInterface::use().queryGameObject( q );
}

int DLL_EXPORT _QueriedGameObjectHandle()
{
    return PtakopyskInterface::use().queriedGameObjectHandle();
}

unsigned int DLL_EXPORT _QueriedGameObjectResultsCount()
{
    return PtakopyskInterface::use().queriedGameObjectResultsCount();
}

bool DLL_EXPORT _QueriedGameObjectResultNext()
{
    return PtakopyskInterface::use().queriedGameObjectResultNext();
}

const char* DLL_EXPORT _QueriedGameObjectResultKey()
{
    s_lastString = PtakopyskInterface::use().queriedGameObjectResultKey();
    return s_lastString.c_str();
}

const char* DLL_EXPORT _QueriedGameObjectResultValue()
{
    s_lastString = PtakopyskInterface::use().queriedGameObjectResultValue();
    return s_lastString.c_str();
}

void DLL_EXPORT _EndQueryGameObject()
{
    PtakopyskInterface::use().endQueryGameObject();
}

bool DLL_EXPORT _StartIterateGameObjects( bool isPrefab )
{
    return PtakopyskInterface::use().startIterateGameObjects( isPrefab );
}

bool DLL_EXPORT _CanIterateGameObjectsNext( bool isPrefab )
{
    return PtakopyskInterface::use().canIterateGameObjectsNext( isPrefab );
}

bool DLL_EXPORT _IterateGameObjectsNext( bool isPrefab )
{
    return PtakopyskInterface::use().iterateGameObjectsNext( isPrefab );
}

bool DLL_EXPORT _StartQueryIteratedGameObject()
{
    return PtakopyskInterface::use().startQueryIteratedGameObject();
}

bool DLL_EXPORT _EndIterateGameObjects()
{
    return PtakopyskInterface::use().endIterateGameObjects();
}

int DLL_EXPORT _FindGameObjectHandleById( const char* id, bool isPrefab, int parent )
{
    std::string i = id;
    return PtakopyskInterface::use().findGameObjectHandleById( i, isPrefab, parent );
}

int DLL_EXPORT _FindGameObjectHandleAtPosition( float x, float y, int parent )
{
    return PtakopyskInterface::use().findGameObjectHandleAtPosition( x, y, parent );
}

int DLL_EXPORT _FindGameObjectHandleAtScreenPosition( int x, int y, int parent )
{
    return PtakopyskInterface::use().findGameObjectHandleAtScreenPosition( x, y, parent );
}

void DLL_EXPORT _StartIterateAssets( int type )
{
    PtakopyskInterface::use().startIterateAssets( (PtakopyskInterface::AssetType)type );
}

bool DLL_EXPORT _CanIterateAssetsNext( int type )
{
    return PtakopyskInterface::use().canIterateAssetsNext( (PtakopyskInterface::AssetType)type );
}

bool DLL_EXPORT _IterateAssetsNext( int type )
{
    return PtakopyskInterface::use().iterateAssetsNext( (PtakopyskInterface::AssetType)type );
}

const char* DLL_EXPORT _GetIteratedAssetId( int type )
{
    s_lastString = PtakopyskInterface::use().getIteratedAssetId( (PtakopyskInterface::AssetType)type );
    return s_lastString.c_str();
}

const char* DLL_EXPORT _GetIteratedAssetMeta( int type )
{
    s_lastString = PtakopyskInterface::use().getIteratedAssetMeta( (PtakopyskInterface::AssetType)type );
    return s_lastString.c_str();
}

const char* DLL_EXPORT _GetIteratedAssetTags( int type )
{
    s_lastString = PtakopyskInterface::use().getIteratedAssetTags( (PtakopyskInterface::AssetType)type );
    return s_lastString.c_str();
}

void DLL_EXPORT _EndIterateAssets( int type )
{
    PtakopyskInterface::use().endIterateAssets( (PtakopyskInterface::AssetType)type );
}

bool DLL_EXPORT _QueryAssets( int type, const char* query )
{
    std::string q = query;
    return PtakopyskInterface::use().queryAssets( (PtakopyskInterface::AssetType)type, q );
}

int DLL_EXPORT _PluginLoadComponents( const char* path )
{
    std::string p = path;
    return PtakopyskInterface::use().pluginLoadComponents( p );
}

bool DLL_EXPORT _PluginUnloadComponents( int handle )
{
    return PtakopyskInterface::use().pluginUnloadComponents( handle );
}

bool DLL_EXPORT _PluginUnloadComponentsByPath( const char* path )
{
    std::string p = path;
    return PtakopyskInterface::use().pluginUnloadComponentsByPath( p );
}

void DLL_EXPORT _StartIterateComponents()
{
    PtakopyskInterface::use().startIterateComponents();
}

bool DLL_EXPORT _CanIterateComponentsNext()
{
    return PtakopyskInterface::use().canIterateComponentsNext();
}

bool DLL_EXPORT _IterateComponentsNext()
{
    return PtakopyskInterface::use().iterateComponentsNext();
}

const char* DLL_EXPORT _GetIteratedComponentId()
{
    s_lastString = PtakopyskInterface::use().getIteratedComponentId();
    return s_lastString.c_str();
}

void DLL_EXPORT _EndIterateComponents()
{
    PtakopyskInterface::use().endIterateComponents();
}

///
///
///

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            PtakopyskInterface::use();
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            PtakopyskInterface::destroy();
            s_lastErrors.clear();
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
