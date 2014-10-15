#include "SceneViewInterface.h"

#include <Ptakopysk/System/Assets.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <XeCore/Common/Timer.h>

RTTI_CLASS_DERIVATIONS( SceneViewInterface,
                        RTTI_DERIVATIONS_END
                        )

SceneViewInterface::SceneViewInterface()
: RTTI_CLASS_DEFINE( SceneViewInterface )
, m_renderWindow( 0 )
, m_gameManager( 0 )
, m_gridSize( sf::Vector2f( 64.0f, 64.0f ) )
, m_cameraSize( sf::Vector2f( 1.0f, 1.0f ) )
, m_cameraZoom( 1.0f )
{
}

SceneViewInterface::~SceneViewInterface()
{
    release();
}

void SceneViewInterface::query( const Json::Value& root, Json::Value& result )
{
    if( root.isMember( "initialize" ) )
    {
        Json::Value func = root[ "initialize" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value windowHandle = func[ 0u ];
            if( windowHandle.isNumeric() )
                result = makeResultJson( initialize( (int64_t)windowHandle.asDouble() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "release" ) )
    {
        Json::Value func = root[ "release" ];
        if( func.isArray() && func.size() == 0 )
        {
            release();
            result = makeResultJson( Json::Value::null );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "processEvents" ) )
    {
        Json::Value func = root[ "processEvents" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( processEvents() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "processUpdate" ) )
    {
        Json::Value func = root[ "processUpdate" ];
        if( func.isArray() && func.size() == 2 )
        {
            Json::Value deltaTime = func[ 0u ];
            Json::Value sortInstances = func[ 1u ];
            if( deltaTime.isNumeric() && sortInstances.isBool() )
                result = makeResultJson( processUpdate( (float)deltaTime.asDouble(), sortInstances.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "processRender" ) )
    {
        Json::Value func = root[ "processRender" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( processRender() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "setAssetsFileSystemRoot" ) )
    {
        Json::Value func = root[ "setAssetsFileSystemRoot" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value path = func[ 0u ];
            if( path.isString() )
            {
                setAssetsFileSystemRoot( path.asString() );
                result = makeResultJson( Json::Value::null );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "getGridSize" ) )
    {
        Json::Value func = root[ "getGridSize" ];
        if( func.isArray() && func.size() == 0 )
        {
            Json::Value v;
            v.append( getGridSize().x );
            v.append( getGridSize().y );
            result = makeResultJson( v );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "setGridSize" ) )
    {
        Json::Value func = root[ "setGridSize" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value v = func[ 0u ];
            if( v.isArray() && v.size() == 2 )
            {
                setGridSize( sf::Vector2f( (float)v[ 0u ].asDouble(), (float)v[ 1u ].asDouble() ) );
                result = makeResultJson( Json::Value::null );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "getSceneViewSize" ) )
    {
        Json::Value func = root[ "getSceneViewSize" ];
        if( func.isArray() && func.size() == 0 )
        {
            Json::Value v;
            v.append( getSceneViewSize().x );
            v.append( getSceneViewSize().y );
            result = makeResultJson( v );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "setSceneViewSize" ) )
    {
        Json::Value func = root[ "setSceneViewSize" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value v = func[ 0u ];
            if( v.isArray() && v.size() == 2 )
            {
                setSceneViewSize( sf::Vector2f( (float)v[ 0u ].asDouble(), (float)v[ 1u ].asDouble() ) );
                result = makeResultJson( Json::Value::null );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "getSceneViewCenter" ) )
    {
        Json::Value func = root[ "getSceneViewCenter" ];
        if( func.isArray() && func.size() == 0 )
        {
            Json::Value v;
            v.append( getSceneViewCenter().x );
            v.append( getSceneViewCenter().y );
            result = makeResultJson( v );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "setSceneViewCenter" ) )
    {
        Json::Value func = root[ "setSceneViewCenter" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value v = func[ 0u ];
            if( v.isArray() && v.size() == 2 )
            {
                setSceneViewCenter( sf::Vector2f( (float)v[ 0u ].asDouble(), (float)v[ 1u ].asDouble() ) );
                result = makeResultJson( Json::Value::null );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "getSceneViewZoom" ) )
    {
        Json::Value func = root[ "getSceneViewZoom" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( getSceneViewZoom() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "setSceneViewZoom" ) )
    {
        Json::Value func = root[ "setSceneViewZoom" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value v = func[ 0u ];
            if( v.isNumeric() )
            {
                setSceneViewZoom( (float)v.asDouble() );
                result = makeResultJson( Json::Value::null );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "convertPointFromScreenToWorldSpace" ) )
    {
        Json::Value func = root[ "convertPointFromScreenToWorldSpace" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value p = func[ 0u ];
            if( p.isArray() && p.size() == 2 )
            {
                sf::Vector2f r = convertPointFromScreenToWorldSpace( sf::Vector2i( p[ 0u ].asInt(), p[ 1u ].asInt() ) );
                Json::Value v;
                v.append( r.x );
                v.append( r.y );
                result = makeResultJson( v );
            }
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "clearScene" ) )
    {
        Json::Value func = root[ "clearScene" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( clearScene() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "clearSceneGameObjects" ) )
    {
        Json::Value func = root[ "clearSceneGameObjects" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value isPrefab = func[ 0u ];
            if( isPrefab.isBool() )
                result = makeResultJson( clearSceneGameObjects( isPrefab.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "applyJsonToScene" ) )
    {
        Json::Value func = root[ "applyJsonToScene" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value json = func[ 0u ];
            result = makeResultJson( applyJsonToScene( json ) );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "convertSceneToJson" ) )
    {
        Json::Value func = root[ "convertSceneToJson" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( convertSceneToJson() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "createGameObject" ) )
    {
        Json::Value func = root[ "createGameObject" ];
        if( func.isArray() && func.size() == 4 )
        {
            Json::Value isPrefab = func[ 0u ];
            Json::Value parent = func[ 1u ];
            Json::Value prefabSource = func[ 2u ];
            Json::Value id = func[ 3u ];
            if( isPrefab.isBool() && parent.isNumeric() && prefabSource.isString() && id.isString() )
                result = makeResultJson( createGameObject( isPrefab.asBool(), parent.asInt(), prefabSource.asString(), id.asString() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "destroyGameObject" ) )
    {
        Json::Value func = root[ "destroyGameObject" ];
        if( func.isArray() && func.size() == 2 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            if( handle.isNumeric() && isPrefab.isBool() )
                result = makeResultJson( destroyGameObject( handle.asInt(), isPrefab.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "clearGameObject" ) )
    {
        Json::Value func = root[ "clearGameObject" ];
        if( func.isArray() && func.size() == 2 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            if( handle.isNumeric() && isPrefab.isBool() )
                result = makeResultJson( clearGameObject( handle.asInt(), isPrefab.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "duplicateGameObject" ) )
    {
        Json::Value func = root[ "duplicateGameObject" ];
        if( func.isArray() && func.size() == 4 )
        {
            Json::Value handleFrom = func[ 0u ];
            Json::Value isPrefabFrom = func[ 1u ];
            Json::Value handleTo = func[ 2u ];
            Json::Value isPrefabTo = func[ 3u ];
            if( handleFrom.isNumeric() && isPrefabFrom.isBool() && handleTo.isNumeric() && isPrefabTo.isBool() )
                result = makeResultJson( duplicateGameObject( handleFrom.asInt(), isPrefabFrom.asBool(), handleTo.asInt(), isPrefabTo.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "triggerGameObjectComponentFunctionality" ) )
    {
        Json::Value func = root[ "triggerGameObjectComponentFunctionality" ];
        if( func.isArray() && func.size() == 4 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            Json::Value compId = func[ 2u ];
            Json::Value funcName = func[ 3u ];
            if( handle.isNumeric() && isPrefab.isBool() && compId.isString() && funcName.isString() )
                result = makeResultJson( triggerGameObjectComponentFunctionality( handle.asInt(), isPrefab.asBool(), compId.asString(), funcName.asString() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "applyJsonToGameObject" ) )
    {
        Json::Value func = root[ "applyJsonToGameObject" ];
        if( func.isArray() && func.size() == 3 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            Json::Value json = func[ 2u ];
            if( handle.isNumeric() && isPrefab.isBool() )
                result = makeResultJson( applyJsonToGameObject( handle.asInt(), isPrefab.asBool(), json ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "convertGameObjectToJson" ) )
    {
        Json::Value func = root[ "convertGameObjectToJson" ];
        if( func.isArray() && func.size() == 2 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            if( handle.isNumeric() && isPrefab.isBool() )
                result = makeResultJson( convertGameObjectToJson( handle.asInt(), isPrefab.asBool() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "findGameObjectHandleById" ) )
    {
        Json::Value func = root[ "findGameObjectHandleById" ];
        if( func.isArray() && func.size() == 3 )
        {
            Json::Value id = func[ 0u ];
            Json::Value handle = func[ 1u ];
            Json::Value parent = func[ 2u ];
            if( id.isString() && handle.isNumeric() && parent.isNumeric() )
                result = makeResultJson( findGameObjectHandleById( id.asString(), handle.asInt(), parent.asInt() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "findGameObjectHandleAtScreenPosition" ) )
    {
        Json::Value func = root[ "findGameObjectHandleAtScreenPosition" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value p = func[ 0u ];
            if( p.isArray() && p.size() == 2 )
                result = makeResultJson( findGameObjectHandleAtScreenPosition( sf::Vector2i( p[ 0u ].asInt(), p[ 1u ].asInt() ) ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "listGameObjects" ) )
    {
        Json::Value func = root[ "listGameObjects" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value isPrefab = func[ 0u ];
            if( isPrefab.isBool() )
                result = makeResultJson( listGameObjects( isPrefab.asInt(), true ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "queryGameObject" ) )
    {
        Json::Value func = root[ "queryGameObject" ];
        if( func.isArray() && func.size() == 3 )
        {
            Json::Value handle = func[ 0u ];
            Json::Value isPrefab = func[ 1u ];
            Json::Value json = func[ 2u ];
            if( handle.isNumeric() && isPrefab.isBool() )
                result = makeResultJson( queryGameObject( handle.asInt(), isPrefab.asBool(), json ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "listAssets" ) )
    {
        Json::Value func = root[ "listAssets" ];
        if( func.isArray() && func.size() == 1 )
        {
            Json::Value type = func[ 0u ];
            if( type.isNumeric() )
                result = makeResultJson( listAssets( (AssetType)type.asInt() ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "queryAssets" ) )
    {
        Json::Value func = root[ "queryAssets" ];
        if( func.isArray() && func.size() == 2 )
        {
            Json::Value type = func[ 0u ];
            Json::Value json = func[ 1u ];
            if( type.isNumeric() )
                result = makeResultJson( queryAssets( (AssetType)type.asInt(), json ) );
            else
                result = makeErrorJson( "Some argument have wrong type!" );
        }
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "listComponents" ) )
    {
        Json::Value func = root[ "listComponents" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( listComponents() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else if( root.isMember( "listCustomAssets" ) )
    {
        Json::Value func = root[ "listCustomAssets" ];
        if( func.isArray() && func.size() == 0 )
            result = makeResultJson( listCustomAssets() );
        else
            result = makeErrorJson( "Wrong number of arguments!" );
    }
    else
        makeErrorJson( "Query function not found!" );
}

Json::Value SceneViewInterface::makeErrorJson( const std::string& text )
{
    Json::Value root;
    root[ "error" ] = text;
    return root;
}

Json::Value SceneViewInterface::makeResultJson( const Json::Value& value )
{
    Json::Value root;
    root[ "error" ] = popErrors();
    root[ "result" ] = value;
    return root;
}

bool SceneViewInterface::initialize( int64_t windowHandle )
{
    if( !windowHandle )
    {
        m_errors << "Cannot initialize PtakopyskInterface: windowHandle cannot be null!\n";
        return false;
    }

    release();
    LOG_SETUP( "SceneViewInterface.log" );
    GameManager::initialize();
    GameManager::setEditMode( true );
    m_renderWindow = xnew sf::RenderWindow( (sf::WindowHandle)windowHandle );
    m_renderWindow->setVerticalSyncEnabled( false );
    m_gameManager = xnew GameManager();
    m_gameManager->RenderWindow = m_renderWindow;
    Assets::use().setAssetsLoadingMode( Assets::LoadIfFilesExists );
    for( std::map< std::string, ComponentData >::iterator it = m_components.begin(); it != m_components.end(); it++ )
        GameManager::registerComponentFactory( it->first, it->second.type, it->second.builder );
    for( std::map< std::string, CustomAssetData >::iterator it = m_customAssets.begin(); it != m_customAssets.end(); it++ )
        Assets::use().registerCustomAssetFactory( it->first, it->second.type, it->second.builder );
    return true;
}

void SceneViewInterface::release()
{
    DELETE_OBJECT( m_gameManager );
    m_renderWindow = 0;
    GameManager::cleanup();
    Assets::destroy();
}

bool SceneViewInterface::processEvents()
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
    {
        m_errors << "Window is null or window is not open or game manager is null!\n";
        return false;
    }

    sf::Event event;
    while( m_renderWindow->pollEvent( event ) )
        m_gameManager->processEvents( event );

    return true;
}

bool SceneViewInterface::processUpdate( float deltaTime, bool sortInstances )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
    {
        m_errors << "Window is null or window is not open or game manager is null!\n";
        return false;
    }

    m_gameManager->processUpdate( deltaTime, sortInstances );
    return true;
}

bool SceneViewInterface::processRender()
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
    {
        m_errors << "Window is null or window is not open or game manager is null!\n";
        return false;
    }

    m_renderWindow->clear( sf::Color( 64, 64, 64, 255 ) );
    m_renderWindow->setView( m_sceneView );
    m_gameManager->processRenderEditor( m_sceneView, m_renderWindow );
    float zf = 1.0f;
    float zt = 1.0f;
    if( m_cameraZoom > 1.0f )
    {
        zt *= 2.0f;
        while( zt <= m_cameraZoom )
        {
            zf = zt;
            zt *= 2.0f;
        }
    }
    else if( m_cameraZoom < 1.0f )
    {
        zf *= 0.5f;
        while( zf >= m_cameraZoom )
        {
            zt = zf;
            zf *= 0.5f;
        }
    }
    renderGrid( m_renderWindow, m_gridSize * zf );
    renderGrid( m_renderWindow, m_gridSize * zt );
    m_renderWindow->display();
    return true;
}

sf::Vector2f SceneViewInterface::convertPointFromScreenToWorldSpace( const sf::Vector2i& point )
{
    if( !m_renderWindow )
    {
        m_errors << "Render window cannot be null!\n";
        return sf::Vector2f();
    }

    return m_renderWindow->mapPixelToCoords( point, m_sceneView );
}

bool SceneViewInterface::clearSceneGameObjects( bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    m_gameManager->removeAllGameObjects( isPrefab );
    m_gameManager->processRemoving();
    return true;
}

bool SceneViewInterface::clearScene()
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    m_gameManager->removeScene();
    m_gameManager->processRemoving();
    return true;
}

bool SceneViewInterface::applyJsonToScene( const Json::Value& root )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    m_gameManager->jsonToScene( root );
    m_gameManager->processAdding();
    m_gameManager->processRemoving();
    return true;
}

Json::Value SceneViewInterface::convertSceneToJson()
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return Json::Value::null;
    }

    return m_gameManager->sceneToJson( GameManager::All, true );
}

int SceneViewInterface::createGameObject( bool isPrefab, int parent, const std::string& prefabSource, const std::string& id )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return 0;
    }

    GameObject* p = parent ? findGameObject( parent, isPrefab ) : 0;
    if( prefabSource.empty() )
    {
        GameObject* go = xnew GameObject( id );
        if( p )
        {
            p->addGameObject( go );
            p->processAdding();
        }
        else
        {
            m_gameManager->addGameObject( go, isPrefab );
            m_gameManager->processAdding();
        }
        return (int)go;
    }
    else
    {
        if( p )
        {
            GameObject* go = m_gameManager->instantiatePrefab( prefabSource );
            if( go )
            {
                go->setId( id );
                p->addGameObject( go );
                p->processAdding();
                return (int)go;
            }
        }
        else
        {
            GameObject* go = m_gameManager->instantiatePrefab( prefabSource );
            if( go )
            {
                go->setId( id );
                m_gameManager->addGameObject( go, isPrefab );
                m_gameManager->processAdding();
                return (int)go;
            }
        }
    }

    return 0;
}

bool SceneViewInterface::destroyGameObject( int handle, bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    GameObject* go = findGameObject( handle, isPrefab );

    if( go )
    {
        GameObject* parent = go->getParent();
        if( parent )
        {
            parent->removeGameObject( go );
            parent->processRemoving();
            return true;
        }
        else
        {
            m_gameManager->removeGameObject( go, isPrefab );
            m_gameManager->processRemoving();
            return true;
        }
    }

    return false;
}

bool SceneViewInterface::clearGameObject( int handle, bool isPrefab )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( go )
    {
        go->removeAllComponents();
        return true;
    }
    return false;
}

bool SceneViewInterface::duplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo )
{
    GameObject* goFrom = findGameObject( handleFrom, isPrefabFrom );
    GameObject* goTo = findGameObject( handleTo, isPrefabTo );
    if( goFrom && goTo )
    {
        std::string toId = goTo->getId();
        goTo->duplicate( goFrom );
        if( isPrefabTo )
            goTo->setId( toId );
        return true;
    }
    return false;
}

bool SceneViewInterface::triggerGameObjectComponentFunctionality( int handle, bool isPrefab, const std::string& compId, const std::string& funcName )
{
    GameObject* go = findGameObject( handle, isPrefab );
    XeCore::Common::IRtti::Derivation ct = GameManager::findComponentFactoryTypeById( compId );
    if( go && ct )
    {
        Component* c = go->getComponent( ct );
        if( c )
        {
            if( c->triggerFunctionality( funcName ) )
                return true;
            else
            {
                m_errors << "Failed triggering functionality: " << funcName.c_str() << "!\n";
                return false;
            }
        }
        else
        {
            m_errors << "Cannot find component by id: " << compId.c_str() << "!\n";
            return false;
        }
    }
    else
    {
        m_errors << "Cannot find GameObject or Component id!\n";
        return false;
    }
}

bool SceneViewInterface::applyJsonToGameObject( int handle, bool isPrefab, const Json::Value& root )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( go )
    {
        go->fromJson( root );
        return true;
    }
    return false;
}

Json::Value SceneViewInterface::convertGameObjectToJson( int handle, bool isPrefab )
{
    GameObject* go = findGameObject( handle, isPrefab );
    return go ? go->toJson() : Json::Value::null;
}

int SceneViewInterface::findGameObjectHandleById( const std::string& id, bool isPrefab, int parent )
{
    GameObject* p = parent == 0 ? 0 : findGameObject( parent, isPrefab );
    return (int)findGameObjectById( id, isPrefab, p );
}

int SceneViewInterface::findGameObjectHandleAtScreenPosition( const sf::Vector2i& p )
{
    if( !m_renderWindow )
    {
        m_errors << "Render window cannot be null!\n";
        return 0;
    }
    sf::Vector2f worldPos = m_renderWindow->mapPixelToCoords( p, m_sceneView );
    return (int)findGameObjectAtPosition( worldPos );
}

Json::Value SceneViewInterface::listGameObjects( bool isPrefab, bool includeChilds, GameObject* parent )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return Json::Value::null;
    }

    Json::Value result( Json::arrayValue );
    Json::Value item;
    if( parent )
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = parent->gameObjectAtBegin(); it != parent->gameObjectAtEnd(); it++ )
        {
            go = *it;
            item = Json::Value::null;
            item[ "handle" ] = (int)go;
            item[ "id" ] = go->getId();
            if( includeChilds )
                item[ "childs" ] = listGameObjects( isPrefab, includeChilds, go );
            result.append( item );
        }
    }
    else
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = m_gameManager->gameObjectAtBegin( isPrefab ); it != m_gameManager->gameObjectAtEnd( isPrefab ); it++ )
        {
            go = *it;
            item = Json::Value::null;
            item[ "handle" ] = (int)go;
            item[ "id" ] = go->getId();
            if( includeChilds )
                item[ "childs" ] = listGameObjects( isPrefab, includeChilds, go );
            result.append( item );
        }
    }
    return result;
}

Json::Value SceneViewInterface::queryGameObject( int handle, bool isPrefab, const Json::Value& query )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( !go )
    {
        m_errors << "Cannot find Game Object!\n";
        return Json::Value::null;
    }
    if( !query.isObject() )
    {
        m_errors << "Query is not an object!\n";
        return Json::Value::null;
    }

    Json::Value result;
    if( query.isMember( "set" ) )
    {
        Json::Value set = query[ "set" ];
        if( set.isObject() )
        {
            if( set.isMember( "properties" ) )
            {
                Json::Value properties = set[ "properties" ];
                if( properties.isObject() )
                {
                    Json::Value::Members m = properties.getMemberNames();
                    Json::Value item;
                    for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
                    {
                        item = properties[ *it ];
                        if( *it == "Id" && item.isString() )
                            go->setId( item.asString() );
                        else if( *it == "Active" && item.isBool() )
                            go->setActive( item.asBool() );
                        else if( *it == "Order" && item.isNumeric() )
                            go->setOrder( item.asInt() );
                        else if( *it == "MetaData" )
                            go->setMetaData( item );
                    }
                }
            }
            if( set.isMember( "components" ) )
            {
                Json::Value components = set[ "components" ];
                if( components.isObject() )
                {
                    Serialized* s;
                    Json::Value::Members m = components.getMemberNames();
                    Json::Value item;
                    Json::Value _item;
                    for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
                    {
                        XeCore::Common::IRtti::Derivation type = GameManager::findComponentFactoryTypeById( *it );
                        s = go->getOrCreateComponent( type );
                        if( s )
                        {
                            item = components[ *it ];
                            if( item.isNull() )
                                go->removeComponent( type );
                            else if( item.isObject() )
                            {
                                Json::Value::Members _m = item.getMemberNames();
                                for( Json::Value::Members::iterator _it = _m.begin(); _it != _m.end(); _it++ )
                                {
                                    _item = item[ *_it ];
                                    s->deserializeProperty( *_it, _item );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::stringstream ss;
    if( query.isMember( "get" ) )
    {
        Json::Value get = query[ "get" ];
        if( get.isNull() )
        {
            GameObject* pref = go->getPrefab();
            result[ "prefab" ] = Json::Value( pref ? pref->getId() : "" ).toStyledString();
            result[ "properties/Id" ] = Json::Value( go->getId() ).toStyledString();
            result[ "properties/Active" ] = Json::Value( go->isActive() ).toStyledString();
            result[ "properties/Order" ] = Json::Value( go->getOrder() ).toStyledString();
            result[ "properties/MetaData" ] = Json::Value( go->getMetaData() ).toStyledString();
            Serialized* s;
            std::string n;
            for( GameObject::Components::iterator it = go->componentAtBegin(); it != go->componentAtEnd(); it++ )
            {
                s = it->second;
                n = GameManager::findComponentFactoryIdByType( it->first );
                if( s )
                {
                    std::vector< std::string >& props = s->accessPropertiesNames();
                    for( std::vector< std::string >::iterator _it = props.begin(); _it != props.end(); _it++ )
                    {
                        Json::Value v;
                        s->serializeProperty( *_it, v );
                        ss.clear();
                        ss.str( "" );
                        ss << "components/" << n << "/" << *_it;
                        result[ ss.str() ] = v.toStyledString();
                    }
                }
            }
        }
        else if( get.isObject() )
        {
            if( get.isMember( "prefab" ) )
            {
                Json::Value prefab = get[ "prefab" ];
                if( prefab.isNull() )
                {
                    GameObject* pref = go->getPrefab();
                    result[ "prefab" ] = Json::Value( pref ? pref->getId() : "" ).toStyledString();
                }
            }
            if( get.isMember( "properties" ) )
            {
                Json::Value properties = get[ "properties" ];
                if( properties.isNull() )
                {
                    result[ "properties/Id" ] = Json::Value( go->getId() ).toStyledString();
                    result[ "properties/Active" ] = Json::Value( go->isActive() ).toStyledString();
                    result[ "properties/Order" ] = Json::Value( go->getOrder() ).toStyledString();
                    result[ "properties/MetaData" ] = go->getMetaData().toStyledString();
                }
                else if( properties.isArray() )
                {
                    Json::Value item;
                    std::string property;
                    for( unsigned int i = 0; i < properties.size(); i++ )
                    {
                        item = properties[ i ];
                        if( item.isString() )
                        {
                            property = item.asString();
                            ss.clear();
                            ss.str( "" );
                            ss << "properties/" << property;
                            if( property == "Id" )
                                result[ ss.str() ] = Json::Value( go->getId() ).toStyledString();
                            else if( property == "Active" )
                                result[ ss.str() ] = Json::Value( go->isActive() ).toStyledString();
                            else if( property == "Order" )
                                result[ ss.str() ] = Json::Value( go->getOrder() ).toStyledString();
                            else if( property == "MetaData" )
                                result[ ss.str() ] = go->getMetaData().toStyledString();
                        }
                    }
                }
            }
            if( get.isMember( "components" ) )
            {
                Json::Value components = get[ "components" ];
                if( components.isNull() )
                {
                    Serialized* s;
                    std::string n;
                    for( GameObject::Components::iterator it = go->componentAtBegin(); it != go->componentAtEnd(); it++ )
                    {
                        s = it->second;
                        n = GameManager::findComponentFactoryIdByType( it->first );
                        if( s )
                        {
                            std::vector< std::string >& props = s->accessPropertiesNames();
                            for( std::vector< std::string >::iterator _it = props.begin(); _it != props.end(); _it++ )
                            {
                                Json::Value v;
                                s->serializeProperty( *_it, v );
                                ss.clear();
                                ss.str( "" );
                                ss << "components/" << n << "/" << *_it;
                                result[ ss.str() ] = v.toStyledString();
                            }
                        }
                    }
                }
                else if( components.isObject() )
                {
                    Serialized* s;
                    Json::Value::Members m = components.getMemberNames();
                    Json::Value item;
                    for( Json::Value::Members::iterator it = m.begin(); it != m.end(); it++ )
                    {
                        item = components[ *it ];
                        XeCore::Common::IRtti::Derivation type = GameManager::findComponentFactoryTypeById( *it );
                        s = go->getComponent( type );
                        if( s )
                        {
                            if( item.isNull() )
                            {
                                std::vector< std::string >& props = s->accessPropertiesNames();
                                for( std::vector< std::string >::iterator _it = props.begin(); _it != props.end(); _it++ )
                                {
                                    Json::Value v;
                                    s->serializeProperty( *_it, v );
                                    ss.clear();
                                    ss.str( "" );
                                    ss << "components/" << *it << "/" << *_it;
                                    result[ ss.str() ] = v.toStyledString();
                                }
                            }
                            else if( item.isArray() )
                            {
                                Json::Value _item;
                                for( unsigned int i = 0; i < item.size(); i++ )
                                {
                                    _item = item[ i ];
                                    if( _item.isString() )
                                    {
                                        Json::Value v;
                                        s->serializeProperty( _item.asString(), v );
                                        ss.clear();
                                        ss.str( "" );
                                        ss << "components/" << *it << "/" << _item.asString();
                                        result[ ss.str() ] = v.toStyledString();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

Json::Value SceneViewInterface::listAssets( AssetType type )
{
    Json::Value result( Json::arrayValue );
    if( type == atTexture )
    {
        for( std::map< std::string, sf::Texture* >::iterator it = Assets::use().getTextureAtBegin(); it != Assets::use().getTextureAtEnd(); it++ )
            result.append( it->first );
    }
    if( type == atShader )
    {
        for( std::map< std::string, sf::Shader* >::iterator it = Assets::use().getShaderAtBegin(); it != Assets::use().getShaderAtEnd(); it++ )
            result.append( it->first );
    }
    if( type == atSound )
    {
        for( std::map< std::string, sf::Sound* >::iterator it = Assets::use().getSoundAtBegin(); it != Assets::use().getSoundAtEnd(); it++ )
            result.append( it->first );
    }
    if( type == atMusic )
    {
        for( std::map< std::string, sf::Music* >::iterator it = Assets::use().getMusicAtBegin(); it != Assets::use().getMusicAtEnd(); it++ )
            result.append( it->first );
    }
    if( type == atFont )
    {
        for( std::map< std::string, sf::Font* >::iterator it = Assets::use().getFontAtBegin(); it != Assets::use().getFontAtEnd(); it++ )
            result.append( it->first );
    }
    if( type == atCustom )
    {
        for( std::map< std::string, ICustomAsset* >::iterator it = Assets::use().getCustomAssetAtBegin(); it != Assets::use().getCustomAssetAtEnd(); it++ )
            result.append( it->first );
    }
    return result;
}

Json::Value SceneViewInterface::queryAssets( AssetType type, const Json::Value& query )
{
    if( !query.isObject() )
    {
        m_errors << "Query is not an object!\n";
        return Json::Value::null;
    }

    Json::Value result( Json::arrayValue );
    if( query.isMember( "free" ) )
    {
        Json::Value free = query[ "free" ];
        if( free.isArray() && !free.empty() )
        {
            Json::Value item;
            for( unsigned int i = 0; i < free.size(); i++ )
            {
                item = free[ i ];
                if( item.isString() )
                {
                    if( type == atTexture )
                    {
                        Assets::use().freeTexture( item.asString() );
                        if( Assets::use().getTexture( item.asString() ) )
                            m_errors << "Cannot release texture: " << item.asString() << "!\n";
                        else
                            m_errors << "Texture released: " << item.asString() << "!\n";
                    }
                    else if( type == atShader )
                    {
                        Assets::use().freeShader( item.asString() );
                        if( Assets::use().getShader( item.asString() ) )
                            m_errors << "Cannot release shader: " << item.asString() << "!\n";
                        else
                            m_errors << "Shader released: " << item.asString() << "!\n";
                    }
                    else if( type == atSound )
                    {
                        Assets::use().freeSound( item.asString() );
                        if( Assets::use().getSound( item.asString() ) )
                            m_errors << "Cannot release sound: " << item.asString() << "!\n";
                        else
                            m_errors << "Sound released: " << item.asString() << "!\n";
                    }
                    else if( type == atMusic )
                    {
                        Assets::use().freeMusic( item.asString() );
                        if( Assets::use().getMusic( item.asString() ) )
                            m_errors << "Cannot release music: " << item.asString() << "!\n";
                        else
                            m_errors << "Music released: " << item.asString() << "!\n";
                    }
                    else if( type == atFont )
                    {
                        Assets::use().freeFont( item.asString() );
                        if( Assets::use().getFont( item.asString() ) )
                            m_errors << "Cannot release font: " << item.asString() << "!\n";
                        else
                            m_errors << "Font released: " << item.asString() << "!\n";
                    }
                    else if( type == atCustom )
                    {
                        Assets::use().freeCustomAsset( item.asString() );
                        if( Assets::use().getCustomAsset( item.asString() ) )
                            m_errors << "Cannot release custom asset: " << item.asString() << "!\n";
                        else
                            m_errors << "Custom asset released: " << item.asString() << "!\n";
                    }
                }
            }
        }
    }
    if( query.isMember( "load" ) )
    {
        Json::Value load = query[ "load" ];
        if( load.isArray() && !load.empty() )
        {
            Json::Value item;
            Json::Value id;
            std::string sid;
            for( unsigned int i = 0; i < load.size(); i++ )
            {
                item = load[ i ];
                if( item.isObject() && item.isMember( "id" ) )
                {
                    id = item[ "id" ];
                    if( id.isString() )
                    {
                        sid = id.asString();
                        if( type == atTexture )
                        {
                            if( Assets::use().jsonToTexture( item ) )
                                m_errors << "Texture loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load texture: " << sid << "!\n";
                        }
                        else if( type == atShader )
                        {
                            if( !Assets::use().shadersAvailable() )
                                m_errors << "Shaders are not supported!\n";
                            if( Assets::use().jsonToShader( item ) )
                                m_errors << "Shader loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load shader: " << sid << "!\n";
                        }
                        else if( type == atSound )
                        {
                            if( Assets::use().jsonToSound( item ) )
                                m_errors << "Sound loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load sound: " << sid << "!\n";
                        }
                        else if( type == atMusic )
                        {
                            if( Assets::use().jsonToMusic( item ) )
                                m_errors << "Music loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load music: " << sid << "!\n";
                        }
                        else if( type == atFont )
                        {
                            if( Assets::use().jsonToFont( item ) )
                                m_errors << "Font loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load font: " << sid << "!\n";
                        }
                        else if( type == atCustom )
                        {
                            if( Assets::use().jsonToCustomAsset( item ) )
                                m_errors << "Custom asset loaded: " << sid << "!\n";
                            else
                                m_errors << "Cannot load custom asset: " << sid << "!\n";
                        }
                    }
                }
            }
        }
    }
    if( query.isMember( "get" ) )
    {
        Json::Value get = query[ "get" ];
        if( get.isArray() && !get.empty() )
        {
            Json::Value item;
            for( unsigned int i = 0; i < get.size(); i++ )
            {
                item = get[ i ];
                if( item.isString() )
                {
                    if( type == atTexture )
                        result.append( Assets::use().textureToJson( item.asString() ) );
                    else if( type == atShader )
                        result.append( Assets::use().shaderToJson( item.asString() ) );
                    else if( type == atSound )
                        result.append( Assets::use().soundToJson( item.asString() ) );
                    else if( type == atMusic )
                        result.append( Assets::use().musicToJson( item.asString() ) );
                    else if( type == atFont )
                        result.append( Assets::use().fontToJson( item.asString() ) );
                    else if( type == atCustom )
                        result.append( Assets::use().customAssetToJson( item.asString() ) );
                }
            }
        }
    }
    if( query.isMember( "info" ) )
    {
        Json::Value get = query[ "info" ];
        if( get.isArray() && !get.empty() )
        {
            Json::Value item;
            Json::Value info;
            std::vector< std::string >* tags;
            for( unsigned int i = 0; i < get.size(); i++ )
            {
                item = get[ i ];
                if( item.isString() )
                {
                    if( type == atTexture )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getTextureMeta( item.asString() );
                        tags = Assets::use().accessTextureTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                    else if( type == atShader )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getShaderMeta( item.asString() );
                        tags = Assets::use().accessShaderTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                    else if( type == atSound )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getSoundMeta( item.asString() );
                        tags = Assets::use().accessSoundTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                    else if( type == atMusic )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getMusicMeta( item.asString() );
                        tags = Assets::use().accessMusicTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                    else if( type == atFont )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getFontMeta( item.asString() );
                        tags = Assets::use().accessFontTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                    else if( type == atCustom )
                    {
                        info[ "id" ] = item.asString();
                        info[ "meta" ] = Assets::use().getCustomAssetMeta( item.asString() );
                        tags = Assets::use().accessCustomAssetTags( item.asString() );
                        if( tags && !tags->empty() )
                            for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
                                info[ "tags" ].append( *it );
                        result.append( info );
                    }
                }
            }
        }
    }
    return result;
}

Json::Value SceneViewInterface::listComponents()
{
    Json::Value result( Json::arrayValue );
    std::vector< std::string > ids;
    GameManager::getComponentsIds( ids );
    for( std::vector< std::string >::iterator it = ids.begin(); it != ids.end(); it++ )
        result.append( *it );
    return result;
}

Json::Value SceneViewInterface::listCustomAssets()
{
    Json::Value result( Json::arrayValue );
    std::vector< std::string > ids;
    Assets::use().getCustomAssetsIds( ids );
    for( std::vector< std::string >::iterator it = ids.begin(); it != ids.end(); it++ )
        result.append( *it );
    return result;
}

void SceneViewInterface::renderGrid( sf::RenderWindow* target, sf::Vector2f gridSize )
{
    if( !target )
    {
        m_errors << "Target is null.\n";
        return;
    }

    float hw = m_sceneView.getSize().x * 0.5f;
    float hh = m_sceneView.getSize().y * 0.5f;
    float xc = m_sceneView.getCenter().x;
    float yc = m_sceneView.getCenter().y;
    float xf = std::floor( ( xc - hw ) / gridSize.x ) * gridSize.x;
    float yf = std::floor( ( yc - hh ) / gridSize.y ) * gridSize.y;
    float xt = std::ceil( ( xc + hw ) / gridSize.x ) * gridSize.x;
    float yt = std::ceil( ( yc + hh ) / gridSize.y ) * gridSize.y;
    sf::Vertex v[ 2 ];
    v[ 0 ].color = sf::Color( 255, 255, 255, 16 );
    v[ 1 ].color = sf::Color( 255, 255, 255, 16 );
    v[ 0 ].position.y = yf;
    v[ 1 ].position.y = yt;
    for( float x = xf; x <= xt; x += gridSize.x )
    {
        v[ 0 ].position.x = x;
        v[ 1 ].position.x = x;
        target->draw( v, 2, sf::Lines );
    }
    v[ 0 ].position.x = xf;
    v[ 1 ].position.x = xt;
    for( float y = yf; y <= yt; y += gridSize.y )
    {
        v[ 0 ].position.y = y;
        v[ 1 ].position.y = y;
        target->draw( v, 2, sf::Lines );
    }
}

GameObject* SceneViewInterface::findGameObject( int handle, bool isPrefab, GameObject* parent )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return 0;
    }

    if( !handle )
        return 0;

    if( parent )
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = parent->gameObjectAtBegin(); it != parent->gameObjectAtEnd(); it++ )
        {
            go = *it;
            if( (int)go == handle )
                return go;
            else
            {
                go = findGameObject( handle, isPrefab, go );
                if( (int)go == handle )
                    return go;
            }
        }
    }
    else
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = m_gameManager->gameObjectAtBegin( isPrefab ); it != m_gameManager->gameObjectAtEnd( isPrefab ); it++ )
        {
            go = *it;
            if( (int)go == handle )
                return go;
            else
            {
                go = findGameObject( handle, isPrefab, go );
                if( (int)go == handle )
                    return go;
            }
        }
    }

    return 0;
}

GameObject* SceneViewInterface::findGameObjectById( const std::string& id, bool isPrefab, GameObject* parent )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return 0;
    }

    if( id.empty() )
        return 0;

    if( parent )
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = parent->gameObjectAtBegin(); it != parent->gameObjectAtEnd(); it++ )
        {
            go = *it;
            if( go->getId() == id )
                return go;
            else
            {
                go = findGameObjectById( id, isPrefab, go );
                if( go )
                    return go;
            }
        }
    }
    else
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = m_gameManager->gameObjectAtBegin( isPrefab ); it != m_gameManager->gameObjectAtEnd( isPrefab ); it++ )
        {
            go = *it;
            if( go->getId() == id )
                return go;
            else
            {
                go = findGameObjectById( id, isPrefab, go );
                if( go )
                    return go;
            }
        }
    }

    return 0;
}

GameObject* SceneViewInterface::findGameObjectAtPosition( const sf::Vector2f& pos, GameObject* parent )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return 0;
    }

    if( parent )
    {
        GameObject* go = 0;
        for( GameObject::List::reverse_iterator it = parent->gameObjectAtReversedBegin(); it != parent->gameObjectAtReversedEnd(); it++ )
        {
            go = *it;
            if( !go->isActive() )
                continue;
            if( go->componentsContainsPoint( pos ) )
                return go;
            else
            {
                go = findGameObjectAtPosition( pos, go );
                if( go )
                    return go;
            }
        }
    }
    else
    {
        GameObject* go = 0;
        for( GameObject::List::reverse_iterator it = m_gameManager->gameObjectAtReversedBegin(); it != m_gameManager->gameObjectAtReversedEnd(); it++ )
        {
            go = *it;
            if( !go->isActive() )
                continue;
            if( go->componentsContainsPoint( pos ) )
                return go;
            else
            {
                go = findGameObjectAtPosition( pos, go );
                if( go )
                    return go;
            }
        }
    }

    return 0;
}
