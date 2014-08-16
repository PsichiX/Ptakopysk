#include "PtakopyskInterface.h"
#include <json/json.h>

typedef bool (CALLBACK* _UnregisterComponents)();
typedef bool (CALLBACK* _RegisterComponents)( int );

RTTI_CLASS_DERIVATIONS( PtakopyskInterface,
                        RTTI_DERIVATIONS_END
                        )

PtakopyskInterface::PtakopyskInterface()
: RTTI_CLASS_DEFINE( PtakopyskInterface )
, m_renderWindow( 0 )
, m_gameManager( 0 )
, m_queriedGameObject( 0 )
, m_gameObjectIsIterating( false )
, m_gridSize( sf::Vector2f( 64.0f, 64.0f ) )
, m_cameraSize( sf::Vector2f( 1.0f, 1.0f ) )
, m_cameraZoom( 1.0f )
{
}

PtakopyskInterface::~PtakopyskInterface()
{
    release();
}

bool PtakopyskInterface::initialize( int windowHandle, bool editMode )
{
    if( !windowHandle )
    {
        m_errors << "Cannot initialize PtakopyskInterface: windowHandle cannot be null!\n";
        return false;
    }

    release();

    LOG_SETUP( "PtakopyskInterface.log" );
    GameManager::initialize();
    GameManager::setEditMode( editMode );
    m_renderWindow = xnew sf::RenderWindow( (sf::WindowHandle)windowHandle );
    m_gameManager = xnew GameManager();
    m_gameManager->RenderWindow = m_renderWindow;

    return true;
}

void PtakopyskInterface::release()
{
    DELETE_OBJECT( m_gameManager );
    if( m_renderWindow )
        m_renderWindow->close();
    DELETE_OBJECT( m_renderWindow );
    Assets::destroy();
    pluginUnloadAll();
    GameManager::cleanup();
}

void PtakopyskInterface::setAssetsFileSystemRoot( const std::string& path )
{
    Assets::use().setFileSystemRoot( path );
}

bool PtakopyskInterface::processEvents()
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

bool PtakopyskInterface::processPhysics( float deltaTime, int velocityIterations, int positionIterations )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
    {
        m_errors << "Window is null or window is not open or game manager is null!\n";
        return false;
    }

    m_gameManager->processPhysics( deltaTime, velocityIterations, positionIterations );

    return true;
}

bool PtakopyskInterface::processUpdate( float deltaTime, bool sortInstances )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() || !m_gameManager )
    {
        m_errors << "Window is null or window is not open or game manager is null!\n";
        return false;
    }

    m_gameManager->processUpdate( deltaTime, sortInstances );

    return true;
}

bool PtakopyskInterface::processRender()
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

bool PtakopyskInterface::setVerticalSyncEnabled( bool enabled )
{
    if( !m_renderWindow || !m_renderWindow->isOpen() )
    {
        m_errors << "Window is null or window is not open.\n";
        return false;
    }

    m_renderWindow->setVerticalSyncEnabled( enabled );

    return true;
}

sf::Vector2f PtakopyskInterface::convertPointFromScreenToWorldSpace( sf::Vector2i point )
{
    if( !m_renderWindow )
    {
        m_errors << "Render window cannot be null!\n";
        return sf::Vector2f();
    }

    return m_renderWindow->mapPixelToCoords( point, m_sceneView );
}

bool PtakopyskInterface::clearSceneGameObjects( bool isPrefab )
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

bool PtakopyskInterface::clearScene()
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

bool PtakopyskInterface::applyJsonToScene( const std::string& json )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    Json::Value root;
    Json::Reader reader;
    if( reader.parse( json, root ) )
    {
        m_gameManager->jsonToScene( root );
        m_gameManager->processAdding();
        m_gameManager->processRemoving();
        return true;
    }
    else
    {
        m_errors << "Canot parse json:\n" << json.c_str() << "\n";
        return false;
    }
}

std::string PtakopyskInterface::convertSceneToJson()
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return "";
    }

    Json::Value root = m_gameManager->sceneToJson( GameManager::All, true );
    Json::StyledWriter writer;
    return writer.write( root );
}

int PtakopyskInterface::createGameObject( bool isPrefab, int parent, const std::string& prefabSource, const std::string& id )
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
                m_gameManager->addGameObject( go, isPrefab );
                m_gameManager->processAdding();
                return (int)go;
            }
        }
    }

    return 0;
}

bool PtakopyskInterface::destroyGameObject( int handle, bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    GameObject* go = findGameObject( handle, isPrefab );

    if( go )
    {
        if( go->getParent() )
        {
            go->getParent()->removeGameObject( go );
            go->getParent()->processRemoving();
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

bool PtakopyskInterface::clearGameObject( int handle, bool isPrefab )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( go )
    {
        go->removeAllComponents();
        return true;
    }
    return false;
}

bool PtakopyskInterface::duplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo )
{
    GameObject* goFrom = findGameObject( handleFrom, isPrefabFrom );
    GameObject* goTo = findGameObject( handleTo, isPrefabTo );
    if( goFrom && goTo )
    {
        goTo->duplicate( goFrom );
        return true;
    }
    return false;
}

bool PtakopyskInterface::triggerGameObjectComponentFunctionality( int handle, bool isPrefab, const std::string& compId, const std::string& funcName )
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

bool PtakopyskInterface::applyJsonToGameObject( int handle, bool isPrefab, const std::string& json )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( go )
    {
        Json::Value root;
        Json::Reader reader;
        reader.parse( json, root );
        go->fromJson( root );
        return true;
    }
    return false;
}

std::string PtakopyskInterface::convertGameObjectToJson( int handle, bool isPrefab )
{
    GameObject* go = findGameObject( handle, isPrefab );
    if( go )
    {
        Json::Value root = go->toJson();
        Json::StyledWriter writer;
        return writer.write( root );
    }
    return "";
}

bool PtakopyskInterface::startQueryGameObject( int handle, bool isPrefab )
{
    m_queriedGameObject = findGameObject( handle, isPrefab );
    m_queriedGameObjectResult.clear();
    m_queriedGameObjectCurrentIterator = m_queriedGameObjectResult.end();
    if( !m_queriedGameObject )
    {
        m_errors << "Cannot find queried game object!\n";
        return false;
    }
    return true;
}

bool PtakopyskInterface::queryGameObject( const std::string& query )
{
    if( !m_queriedGameObject )
    {
        m_errors << "Queried game object is null!\n";
        return false;
    }

    m_queriedGameObjectResult.clear();
    std::stringstream ss;
    Json::Value root;
    Json::Reader reader;
    if( reader.parse( query, root ) && root.isObject() )
    {
        if( root.isMember( "set" ) )
        {
            Json::Value set = root[ "set" ];
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
                                m_queriedGameObject->setId( item.asString() );
                            else if( *it == "Active" && item.isBool() )
                                m_queriedGameObject->setActive( item.asBool() );
                            else if( *it == "Order" && item.isNumeric() )
                                m_queriedGameObject->setOrder( item.asInt() );
                            else if( *it == "MetaData" )
                                m_queriedGameObject->setMetaData( item );
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
                            s = m_queriedGameObject->getOrCreateComponent( type );
                            if( s )
                            {
                                item = components[ *it ];
                                if( item.isNull() )
                                    m_queriedGameObject->removeComponent( type );
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
        if( root.isMember( "get" ) )
        {
            Json::Value get = root[ "get" ];
            if( get.isNull() )
            {
                GameObject* pref = m_queriedGameObject->getPrefab();
                m_queriedGameObjectResult[ "prefab" ] = pref ? Json::Value( pref->getId() ) : "";
                m_queriedGameObjectResult[ "properties/Id" ] = Json::Value( m_queriedGameObject->getId() );
                m_queriedGameObjectResult[ "properties/Active" ] = Json::Value( m_queriedGameObject->isActive() );
                m_queriedGameObjectResult[ "properties/Order" ] = Json::Value( m_queriedGameObject->getOrder() );
                m_queriedGameObjectResult[ "properties/MetaData" ] = m_queriedGameObject->getMetaData();
                Serialized* s;
                std::string n;
                for( GameObject::Components::iterator it = m_queriedGameObject->componentAtBegin(); it != m_queriedGameObject->componentAtEnd(); it++ )
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
                            ss.str( "" );
                            ss.clear();
                            ss << "components/" << n << "/" << (*_it);
                            m_queriedGameObjectResult[ ss.str() ] = v;
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
                        GameObject* pref = m_queriedGameObject->getPrefab();
                        m_queriedGameObjectResult[ "prefab" ] = pref ? Json::Value( pref->getId() ) : "";
                    }
                }
                if( get.isMember( "properties" ) )
                {
                    Json::Value properties = get[ "properties" ];
                    if( properties.isNull() )
                    {
                        m_queriedGameObjectResult[ "properties/Id" ] = Json::Value( m_queriedGameObject->getId() );
                        m_queriedGameObjectResult[ "properties/Active" ] = Json::Value( m_queriedGameObject->isActive() );
                        m_queriedGameObjectResult[ "properties/Order" ] = Json::Value( m_queriedGameObject->getOrder() );
                        m_queriedGameObjectResult[ "properties/MetaData" ] = m_queriedGameObject->getMetaData();
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
                                ss.str( "" );
                                ss.clear();
                                ss << "properties/" << property;
                                if( property == "Id" )
                                    m_queriedGameObjectResult[ ss.str() ] = Json::Value( m_queriedGameObject->getId() );
                                else if( property == "Active" )
                                    m_queriedGameObjectResult[ ss.str() ] = Json::Value( m_queriedGameObject->isActive() );
                                else if( property == "Order" )
                                    m_queriedGameObjectResult[ ss.str() ] = Json::Value( m_queriedGameObject->getOrder() );
                                else if( property == "MetaData" )
                                    m_queriedGameObjectResult[ ss.str() ] = m_queriedGameObject->getMetaData();
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
                        for( GameObject::Components::iterator it = m_queriedGameObject->componentAtBegin(); it != m_queriedGameObject->componentAtEnd(); it++ )
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
                                    ss.str( "" );
                                    ss.clear();
                                    ss << "components/" << n << "/" << (*_it);
                                    m_queriedGameObjectResult[ ss.str() ] = v;
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
                            s = m_queriedGameObject->getComponent( type );
                            if( s )
                            {
                                if( item.isNull() )
                                {
                                    std::vector< std::string >& props = s->accessPropertiesNames();
                                    for( std::vector< std::string >::iterator _it = props.begin(); _it != props.end(); _it++ )
                                    {
                                        Json::Value v;
                                        s->serializeProperty( *_it, v );
                                        ss.str( "" );
                                        ss.clear();
                                        ss << "components/" << *it << "/" << (*_it);
                                        m_queriedGameObjectResult[ ss.str() ] = v;
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
                                            ss.str( "" );
                                            ss.clear();
                                            ss << "components/" << *it << "/" << _item.asString();
                                            m_queriedGameObjectResult[ ss.str() ] = v;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    m_queriedGameObjectCurrentIterator = m_queriedGameObjectResult.begin();
    return true;
}

int PtakopyskInterface::queriedGameObjectHandle()
{
    return (int)m_queriedGameObject;
}

unsigned int PtakopyskInterface::queriedGameObjectResultsCount()
{
    return m_queriedGameObjectResult.size();
}

bool PtakopyskInterface::queriedGameObjectResultNext()
{
    m_queriedGameObjectCurrentIterator++;
    return m_queriedGameObjectCurrentIterator == m_queriedGameObjectResult.end();
}

std::string PtakopyskInterface::queriedGameObjectResultKey()
{
    if( m_queriedGameObjectCurrentIterator == m_queriedGameObjectResult.end() )
        return "";
    else
        return m_queriedGameObjectCurrentIterator->first;
}

std::string PtakopyskInterface::queriedGameObjectResultValue()
{
    if( m_queriedGameObjectCurrentIterator == m_queriedGameObjectResult.end() )
        return "";
    else
    {
        Json::FastWriter writer;
        std::string result = writer.write( m_queriedGameObjectCurrentIterator->second );
        return result.substr( 0, result.find_last_not_of( "\f\n\r\t\v" ) + 1 );
    }
}

void PtakopyskInterface::endQueryGameObject()
{
    m_queriedGameObject = 0;
    m_queriedGameObjectResult.clear();
    m_queriedGameObjectCurrentIterator = m_queriedGameObjectResult.end();
}

bool PtakopyskInterface::startIterateGameObjects( bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    if( m_gameObjectIsIterating )
    {
        if( m_gameObjectIteratorStack.empty() )
        {
            if( m_gameObjectCurrentIterator == m_gameManager->gameObjectAtEnd() )
                return false;
            GameObject* go = *m_gameObjectCurrentIterator;
            if( !go )
                return false;
            GameObject::List::iterator it = go->gameObjectAtBegin();
            if( it == go->gameObjectAtEnd() )
                return false;
            m_gameObjectIteratorStack.push( m_gameObjectCurrentIterator );
            m_gameObjectCurrentIterator = it;
            return true;
        }
        else
        {
            GameObject::List::iterator p = m_gameObjectIteratorStack.top();
            GameObject* pgo = *p;
            if( !pgo || m_gameObjectCurrentIterator == pgo->gameObjectAtEnd() )
                return false;
            GameObject* go = *m_gameObjectCurrentIterator;
            if( !go )
                return false;
            GameObject::List::iterator it = go->gameObjectAtBegin();
            if( it == go->gameObjectAtEnd() )
                return false;
            m_gameObjectIteratorStack.push( m_gameObjectCurrentIterator );
            m_gameObjectCurrentIterator = it;
            return true;
        }
    }
    else
    {
        m_gameObjectCurrentIterator = m_gameManager->gameObjectAtBegin( isPrefab );
        if( m_gameObjectCurrentIterator != m_gameManager->gameObjectAtEnd( isPrefab ) )
        {
            m_gameObjectIsIterating = true;
            return true;
        }
        else
            return false;
    }
}

bool PtakopyskInterface::canIterateGameObjectsNext( bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    bool status = false;
    if( m_gameObjectIsIterating )
    {
        if( m_gameObjectIteratorStack.empty() )
            status = m_gameObjectCurrentIterator != m_gameManager->gameObjectAtEnd( isPrefab );
        else
        {
            GameObject::List::iterator p = m_gameObjectIteratorStack.top();
            GameObject* pgo = *p;
            status = pgo && m_gameObjectCurrentIterator != pgo->gameObjectAtEnd();
        }
    }
    return status;
}

bool PtakopyskInterface::iterateGameObjectsNext( bool isPrefab )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return false;
    }

    if( !m_gameObjectIsIterating )
        return false;
    if( m_gameObjectIteratorStack.empty() )
    {
        if( m_gameObjectCurrentIterator != m_gameManager->gameObjectAtEnd( isPrefab ) )
        {
            m_gameObjectCurrentIterator++;
            return true;
        }
    }
    else
    {
        GameObject::List::iterator p = m_gameObjectIteratorStack.top();
        GameObject* pgo = *p;
        if( pgo && m_gameObjectCurrentIterator != pgo->gameObjectAtEnd() )
        {
            m_gameObjectCurrentIterator++;
            return true;
        }
    }
    return false;
}

bool PtakopyskInterface::startQueryIteratedGameObject()
{
    GameObject::List::iterator it = m_gameObjectCurrentIterator;
    m_queriedGameObject = m_gameObjectIsIterating ? *it : 0;
    m_queriedGameObjectResult.clear();
    m_queriedGameObjectCurrentIterator = m_queriedGameObjectResult.end();
    if( !m_queriedGameObject )
    {
        m_errors << "Cannot find iterated queried game object!\n";
        return false;
    }
    return true;
}

bool PtakopyskInterface::endIterateGameObjects()
{
    if( !m_gameObjectIsIterating )
        return false;
    if( m_gameObjectIteratorStack.empty() )
    {
        m_gameObjectIsIterating = false;
        return true;
    }
    else
    {
        m_gameObjectCurrentIterator = m_gameObjectIteratorStack.top();
        m_gameObjectIteratorStack.pop();
        return true;
    }
}

int PtakopyskInterface::findGameObjectHandleById( const std::string& id, bool isPrefab, int parent )
{
    GameObject* p = parent ? findGameObject( parent, isPrefab ) : 0;
    return (int)findGameObjectById( id, isPrefab, p );
}

int PtakopyskInterface::findGameObjectHandleAtPosition( float x, float y, int parent )
{
    return (int)findGameObjectAtPosition( sf::Vector2f( x, y ), (GameObject*)parent );
}

int PtakopyskInterface::findGameObjectHandleAtScreenPosition( int x, int y, int parent )
{
    if( !m_renderWindow )
    {
        m_errors << "Render window cannot be null!\n";
        return 0;
    }

    sf::Vector2f worldPos = m_renderWindow->mapPixelToCoords( sf::Vector2i( x, y ), m_sceneView );
    return (int)findGameObjectAtPosition( worldPos, (GameObject*)parent );
}

void PtakopyskInterface::startIterateAssets( AssetType type )
{
    if( type == atTexture )
        m_assetTextureIterator = Assets::use().getTextureAtBegin();
    else if( type == atShader )
        m_assetShaderIterator = Assets::use().getShaderAtBegin();
    else if( type == atSound )
        m_assetSoundIterator = Assets::use().getSoundAtBegin();
    else if( type == atMusic )
        m_assetMusicIterator = Assets::use().getMusicAtBegin();
    else if( type == atFont )
        m_assetFontIterator = Assets::use().getFontAtBegin();
}

bool PtakopyskInterface::canIterateAssetsNext( AssetType type )
{
    if( type == atTexture )
        return m_assetTextureIterator != Assets::use().getTextureAtEnd();
    else if( type == atShader )
        return m_assetShaderIterator != Assets::use().getShaderAtEnd();
    else if( type == atSound )
        return m_assetSoundIterator != Assets::use().getSoundAtEnd();
    else if( type == atMusic )
        return m_assetMusicIterator != Assets::use().getMusicAtEnd();
    else if( type == atFont )
        return m_assetFontIterator != Assets::use().getFontAtEnd();
    else
        return false;
}

bool PtakopyskInterface::iterateAssetsNext( AssetType type )
{
    if( type == atTexture )
    {
        if( m_assetTextureIterator != Assets::use().getTextureAtEnd() )
        {
            m_assetTextureIterator++;
            return true;
        }
    }
    else if( type == atShader )
    {
        if( m_assetShaderIterator != Assets::use().getShaderAtEnd() )
        {
            m_assetShaderIterator++;
            return true;
        }
    }
    else if( type == atSound )
    {
        if( m_assetSoundIterator != Assets::use().getSoundAtEnd() )
        {
            m_assetSoundIterator++;
            return true;
        }
    }
    else if( type == atMusic )
    {
        if( m_assetMusicIterator != Assets::use().getMusicAtEnd() )
        {
            m_assetMusicIterator++;
            return true;
        }
    }
    else if( type == atFont )
    {
        if( m_assetFontIterator != Assets::use().getFontAtEnd() )
        {
            m_assetFontIterator++;
            return true;
        }
    }
    return false;
}

std::string PtakopyskInterface::getIteratedAssetId( AssetType type )
{
    if( type == atTexture )
    {
        if( m_assetTextureIterator != Assets::use().getTextureAtEnd() )
            return m_assetTextureIterator->first;
    }
    else if( type == atShader )
    {
        if( m_assetShaderIterator != Assets::use().getShaderAtEnd() )
            return m_assetShaderIterator->first;
    }
    else if( type == atSound )
    {
        if( m_assetSoundIterator != Assets::use().getSoundAtEnd() )
            return m_assetSoundIterator->first;
    }
    else if( type == atMusic )
    {
        if( m_assetMusicIterator != Assets::use().getMusicAtEnd() )
            return m_assetMusicIterator->first;
    }
    else if( type == atFont )
    {
        if( m_assetFontIterator != Assets::use().getFontAtEnd() )
            return m_assetFontIterator->first;
    }
    return "";
}

std::string PtakopyskInterface::getIteratedAssetMeta( AssetType type )
{
    if( type == atTexture )
    {
        if( m_assetTextureIterator != Assets::use().getTextureAtEnd() )
            return Assets::use().getTextureMeta( m_assetTextureIterator->first );
    }
    else if( type == atShader )
    {
        if( m_assetShaderIterator != Assets::use().getShaderAtEnd() )
            return Assets::use().getShaderMeta( m_assetShaderIterator->first );
    }
    else if( type == atSound )
    {
        if( m_assetSoundIterator != Assets::use().getSoundAtEnd() )
            return Assets::use().getSoundMeta( m_assetSoundIterator->first );
    }
    else if( type == atMusic )
    {
        if( m_assetMusicIterator != Assets::use().getMusicAtEnd() )
            return Assets::use().getMusicMeta( m_assetMusicIterator->first );
    }
    else if( type == atFont )
    {
        if( m_assetFontIterator != Assets::use().getFontAtEnd() )
            return Assets::use().getFontMeta( m_assetFontIterator->first );
    }
    return "";
}

std::string PtakopyskInterface::getIteratedAssetTags( AssetType type )
{
    std::vector< std::string >* tags = 0;
    if( type == atTexture )
    {
        if( m_assetTextureIterator != Assets::use().getTextureAtEnd() )
            tags = Assets::use().accessTextureTags( m_assetTextureIterator->first );
    }
    else if( type == atShader )
    {
        if( m_assetShaderIterator != Assets::use().getShaderAtEnd() )
            tags = Assets::use().accessShaderTags( m_assetShaderIterator->first );
    }
    else if( type == atSound )
    {
        if( m_assetSoundIterator != Assets::use().getSoundAtEnd() )
            tags = Assets::use().accessSoundTags( m_assetSoundIterator->first );
    }
    else if( type == atMusic )
    {
        if( m_assetMusicIterator != Assets::use().getMusicAtEnd() )
            tags = Assets::use().accessMusicTags( m_assetMusicIterator->first );
    }
    else if( type == atFont )
    {
        if( m_assetFontIterator != Assets::use().getFontAtEnd() )
            tags = Assets::use().accessFontTags( m_assetFontIterator->first );
    }

    if( tags && !tags->empty() )
    {
        std::stringstream ss;
        unsigned int i = 0;
        for( std::vector< std::string >::iterator it = tags->begin(); it != tags->end(); it++ )
        {
            ss << *it;
            if( i < tags->size() - 1 )
                ss << "|";
            i++;
        }
        return ss.str();
    }
    else
        return "";
}

void PtakopyskInterface::endIterateAssets( AssetType type )
{
    if( type == atTexture )
        m_assetTextureIterator = Assets::use().getTextureAtEnd();
    else if( type == atShader )
        m_assetShaderIterator = Assets::use().getShaderAtEnd();
    else if( type == atSound )
        m_assetSoundIterator = Assets::use().getSoundAtEnd();
    else if( type == atMusic )
        m_assetMusicIterator = Assets::use().getMusicAtEnd();
    else if( type == atFont )
        m_assetFontIterator = Assets::use().getFontAtEnd();
}

bool PtakopyskInterface::queryAssets( AssetType type, const std::string& query )
{
    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( query, root ) )
    {
        m_errors << "Cannot parse asset query: " << query.c_str() << "!\n";
        return false;
    }
    if( root.isObject() )
    {
        if( root.isMember( "free" ) )
        {
            Json::Value free = root[ "free" ];
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
                    }
                }
            }
        }
        if( root.isMember( "load" ) )
        {
            Json::Value load = root[ "load" ];
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
                                Assets::use().jsonToTexture( item );
                                if( Assets::use().getTexture( sid ) )
                                    m_errors << "Texture loaded: " << sid << "!\n";
                                else
                                    m_errors << "Cannot load texture: " << sid << "!\n";
                            }
                            else if( type == atShader )
                            {
                                Assets::use().jsonToShader( item );
                                if( Assets::use().getShader( sid ) )
                                    m_errors << "Shader loaded: " << sid << "!\n";
                                else
                                    m_errors << "Cannot load shader: " << sid << "!\n";
                            }
                            else if( type == atSound )
                            {
                                Assets::use().jsonToSound( item );
                                if( Assets::use().getSound( sid ) )
                                    m_errors << "Sound loaded: " << sid << "!\n";
                                else
                                    m_errors << "Cannot load sound: " << sid << "!\n";
                            }
                            else if( type == atMusic )
                            {
                                Assets::use().jsonToMusic( item );
                                if( Assets::use().getMusic( sid ) )
                                    m_errors << "Music loaded: " << sid << "!\n";
                                else
                                    m_errors << "Cannot load music: " << sid << "!\n";
                            }
                            else if( type == atFont )
                            {
                                Assets::use().jsonToFont( item );
                                if( Assets::use().getFont( sid ) )
                                    m_errors << "Font loaded: " << sid << "!\n";
                                else
                                    m_errors << "Cannot load font: " << sid << "!\n";
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    return false;
}

int PtakopyskInterface::pluginLoadComponents( const std::string& path )
{
    if( m_plugins.count( path ) )
    {
        m_errors << "Plugin already loaded: " << path.c_str() << "!\n";
        return 0;
    }

    HINSTANCE plugin = LoadLibrary( path.c_str() );
    if( !plugin )
    {
        m_errors << "Cannot load plugin: " << path.c_str() << "!\n";
        return 0;
    }

    _RegisterComponents registerComponents = (_RegisterComponents)GetProcAddress( plugin, "_RegisterComponents" );
    if( !registerComponents )
    {
        FreeLibrary( plugin );
        m_errors << "Cannot find _RegisterComponents function in plugin: " << path.c_str() << "!\n";
        return 0;
    }

    m_componentsPending.clear();
    if( registerComponents( (int)this ) )
    {
        for( std::vector< ComponentData >::iterator it = m_componentsPending.begin(); it != m_componentsPending.end(); it++ )
        {
            m_errors << "Try to register pending component - id: " << it->id << "; type: " << it->type << "; builder: " << (void*)it->builder << ".\n";
            if( it->id.empty() )
                m_errors << "Cannot register undefined component!\n";
            else
            {
                if( it->type && it->builder )
                {
                    if( GameManager::registerComponentFactory( it->id, it->type, it->builder ) )
                        m_errors << "Registered!\n";
                    else
                        m_errors << "Cannot register!\n";
                }
                else
                    m_errors << "Incomplete component information!\n";
            }
        }
        m_componentsPending.clear();
        m_plugins[ path ] = plugin;
        return (int)plugin;
    }
    else
    {
        m_componentsPending.clear();
        FreeLibrary( plugin );
        m_errors << "Cannot register components from plugin: " << path.c_str() << "!\n";
        return 0;
    }
}

bool PtakopyskInterface::pluginUnloadComponents( int handle )
{
    for( std::map< std::string, HINSTANCE >::iterator it = m_plugins.begin(); it != m_plugins.end(); it++ )
    {
        if( it->second == (HINSTANCE)handle )
        {
            _UnregisterComponents unregisterComponents = (_UnregisterComponents)GetProcAddress( it->second, "_UnregisterComponents" );
            if( !unregisterComponents )
            {
                m_errors << "Cannot find _UnregisterComponents function in plugin: " << it->first.c_str() << "!\n";
                return false;
            }

            m_componentsPending.clear();
            if( unregisterComponents() )
            {
                for( std::vector< ComponentData >::iterator _it = m_componentsPending.begin(); _it != m_componentsPending.end(); _it++ )
                {
                    m_errors << "Try to unregister pending component - id: " << _it->id << ".\n";
                    if( _it->id.empty() )
                        m_errors << "Cannot unregister undefined component!\n";
                    else
                    {
                        if( GameManager::unregisterComponentFactory( _it->id ) )
                            m_errors << "Unregistered!\n";
                        else
                            m_errors << "Cannot unregister!\n";
                    }
                }
                m_componentsPending.clear();
                FreeLibrary( it->second );
                m_plugins.erase( it );
                return true;
            }
            else
            {
                m_componentsPending.clear();
                m_errors << "Cannot unregister components from plugin: " << it->first.c_str() << "!\n";
                return false;
            }
        }
    }
    m_errors << "Cannot unload plugin - there is no loaded plugin handle: " << handle << "!\n";
    return false;
}

bool PtakopyskInterface::pluginUnloadComponentsByPath( const std::string& path )
{
    for( std::map< std::string, HINSTANCE >::iterator it = m_plugins.begin(); it != m_plugins.end(); it++ )
    {
        if( it->first == path )
        {
            _UnregisterComponents unregisterComponents = (_UnregisterComponents)GetProcAddress( it->second, "_UnregisterComponents" );
            if( !unregisterComponents )
            {
                m_errors << "Cannot find _UnregisterComponents function in plugin: " << path.c_str() << "!\n";
                return false;
            }

            m_componentsPending.clear();
            if( unregisterComponents() )
            {
                for( std::vector< ComponentData >::iterator _it = m_componentsPending.begin(); _it != m_componentsPending.end(); _it++ )
                {
                    m_errors << "Try to unregister pending component - id: " << _it->id << ".\n";
                    if( _it->id.empty() )
                        m_errors << "Cannot unregister undefined component!\n";
                    else
                    {
                        if( GameManager::unregisterComponentFactory( _it->id ) )
                            m_errors << "Unregistered!\n";
                        else
                            m_errors << "Cannot unregister!\n";
                    }
                }
                m_componentsPending.clear();
                FreeLibrary( it->second );
                m_plugins.erase( it );
                return true;
            }
            else
            {
                m_componentsPending.clear();
                m_errors << "Cannot unregister components from plugin: " << path.c_str() << "!\n";
                return false;
            }
        }
    }
    m_errors << "Cannot unload plugin - there is no loaded plugin: " << path.c_str() << "!\n";
    return false;
}

void PtakopyskInterface::pluginUnloadAll()
{
    m_componentsPending.clear();
    for( std::map< std::string, HINSTANCE >::iterator it = m_plugins.begin(); it != m_plugins.end(); it++ )
    {
        _UnregisterComponents unregisterComponents = (_UnregisterComponents)GetProcAddress( it->second, "_UnregisterComponents" );
        if( unregisterComponents )
        {
            if( unregisterComponents() )
            {
                for( std::vector< ComponentData >::iterator _it = m_componentsPending.begin(); _it != m_componentsPending.end(); _it++ )
                {
                    m_errors << "Try to unregister pending component - id: " << _it->id << ".\n";
                    if( _it->id.empty() )
                        m_errors << "Cannot unregister undefined component!\n";
                    else
                    {
                        if( GameManager::unregisterComponentFactory( _it->id ) )
                            m_errors << "Unregistered!\n";
                        else
                            m_errors << "Cannot unregister!\n";
                    }
                }
            }
        }
        m_componentsPending.clear();
        FreeLibrary( it->second );
    }
    m_componentsPending.clear();
    m_plugins.clear();
}

void PtakopyskInterface::pluginRegisterComponent( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback creator )
{
    m_componentsPending.push_back( ComponentData( id, type, creator ) );
}

void PtakopyskInterface::pluginUnregisterComponent( const std::string& id )
{
    m_componentsPending.push_back( ComponentData( id, 0, 0 ) );
}

void PtakopyskInterface::startIterateComponents()
{
    GameManager::getComponentsIds( m_queruedComponentsIds );
    m_queruedComponentsIdsIterator = m_queruedComponentsIds.begin();
}

bool PtakopyskInterface::canIterateComponentsNext()
{
    return m_queruedComponentsIdsIterator != m_queruedComponentsIds.end();
}

bool PtakopyskInterface::iterateComponentsNext()
{
    if( m_queruedComponentsIdsIterator != m_queruedComponentsIds.end() )
    {
        m_queruedComponentsIdsIterator++;
        return true;
    }
    return false;
}

std::string PtakopyskInterface::getIteratedComponentId()
{
    if( m_queruedComponentsIdsIterator != m_queruedComponentsIds.end() )
        return *m_queruedComponentsIdsIterator;
    else
        return std::string();
}

void PtakopyskInterface::endIterateComponents()
{
    m_queruedComponentsIds.clear();
    m_queruedComponentsIdsIterator = m_queruedComponentsIds.end();
}

void PtakopyskInterface::renderGrid( sf::RenderWindow* target, sf::Vector2f gridSize )
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

GameObject* PtakopyskInterface::findGameObject( int handle, bool isPrefab, GameObject* parent )
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

GameObject* PtakopyskInterface::findGameObjectById( const std::string& id, bool isPrefab, GameObject* parent )
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

GameObject* PtakopyskInterface::findGameObjectAtPosition( const sf::Vector2f& pos, GameObject* parent )
{
    if( !m_gameManager )
    {
        m_errors << "Game manager is null!\n";
        return 0;
    }

    if( parent )
    {
        GameObject* go = 0;
        for( GameObject::List::iterator it = parent->gameObjectAtBegin(); it != parent->gameObjectAtEnd(); it++ )
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
        for( GameObject::List::iterator it = m_gameManager->gameObjectAtBegin(); it != m_gameManager->gameObjectAtEnd(); it++ )
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
