#ifndef __SCENE_VIEW_INTERFACE__
#define __SCENE_VIEW_INTERFACE__

#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Singleton.h>
#include <windef.h>
#include <sstream>

using namespace Ptakopysk;

class SceneViewInterface
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
{
    RTTI_CLASS_DECLARE( SceneViewInterface );

public:
    enum AssetType
    {
        atTexture = 0,
        atShader,
        atSound,
        atMusic,
        atFont,
        atCustom
    };

    struct ComponentData
    {
        ComponentData() : type( 0 ), builder( 0 ) {};
        ComponentData( XeCore::Common::IRtti::Derivation t, Component::OnBuildComponentCallback b ) : type( t ), builder( b ) {};

        XeCore::Common::IRtti::Derivation type;
        Component::OnBuildComponentCallback builder;
    };

    struct CustomAssetData
    {
        CustomAssetData() : type( 0 ), builder( 0 ) {};
        CustomAssetData( XeCore::Common::IRtti::Derivation t, ICustomAsset::OnBuildCustomAssetCallback b ) : type( t ), builder( b ) {};

        XeCore::Common::IRtti::Derivation type;
        ICustomAsset::OnBuildCustomAssetCallback builder;
    };

    SceneViewInterface();
    virtual ~SceneViewInterface();

    FORCEINLINE void registerComponentFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback builder ) { m_components[ id ] = ComponentData( type, builder ); };
    FORCEINLINE void unregisterComponentFactory( const std::string& id ) { if( m_components.count( id ) ) m_components.erase( id ); };
    FORCEINLINE void registerCustomAssetFactory( const std::string& id, XeCore::Common::IRtti::Derivation type, ICustomAsset::OnBuildCustomAssetCallback builder ) { m_customAssets[ id ] = CustomAssetData( type, builder ); };
    FORCEINLINE void unregisterCustomAssetFactory( const std::string& id ) { if( m_customAssets.count( id ) ) m_customAssets.erase( id ); };
    void query( const Json::Value& root, Json::Value& result );

protected:
    Json::Value makeErrorJson( const std::string& text );
    Json::Value makeResultJson( const Json::Value& value );
    FORCEINLINE std::string popErrors() { std::string err = m_errors.str(); m_errors.str( "" ); return err; };
    bool initialize( int64_t windowHandle );
    void release();
    bool processEvents();
    bool processUpdate( float deltaTime, bool sortInstances );
    bool processRender();
    FORCEINLINE void setAssetsFileSystemRoot( const std::string& path ) { Assets::use().setFileSystemRoot( path ); };
    FORCEINLINE const sf::Vector2f& getGridSize() { return m_gridSize; };
    FORCEINLINE void setGridSize( const sf::Vector2f& v ) { m_gridSize = v; };
    FORCEINLINE const sf::Vector2f& getSceneViewSize() { return m_cameraSize; };
    FORCEINLINE void setSceneViewSize( const sf::Vector2f& v ) { m_cameraSize = v; m_sceneView.setSize( m_cameraSize ); m_sceneView.zoom( m_cameraZoom ); };
    FORCEINLINE const sf::Vector2f& getSceneViewCenter() { return m_sceneView.getCenter(); };
    FORCEINLINE void setSceneViewCenter( const sf::Vector2f& v ) { m_sceneView.setCenter( v ); };
    FORCEINLINE float getSceneViewZoom() { return m_cameraZoom; };
    FORCEINLINE void setSceneViewZoom( float v ) { m_cameraZoom = v; m_sceneView.setSize( m_cameraSize ); m_sceneView.zoom( m_cameraZoom ); };
    sf::Vector2f convertPointFromScreenToWorldSpace( const sf::Vector2i& p );
    bool clearScene();
    bool clearSceneGameObjects( bool isPrefab );
    bool applyJsonToScene( const Json::Value& root );
    Json::Value convertSceneToJson();
    int createGameObject( bool isPrefab, int parent, const std::string& prefabSource, const std::string& id );
    bool destroyGameObject( int handle, bool isPrefab );
    bool clearGameObject( int handle, bool isPrefab );
    bool duplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo );
    bool triggerGameObjectComponentFunctionality( int handle, bool isPrefab, const std::string& compId, const std::string& funcName );
    bool applyJsonToGameObject( int handle, bool isPrefab, const Json::Value& root );
    Json::Value convertGameObjectToJson( int handle, bool isPrefab );
    int findGameObjectHandleById( const std::string& id, bool isPrefab, int parent );
    int findGameObjectHandleAtScreenPosition( const sf::Vector2i& p );
    Json::Value listGameObjects( bool isPrefab, bool includeChilds = true, GameObject* parent = 0 );
    Json::Value queryGameObject( int handle, bool isPrefab, const Json::Value& query );
    Json::Value listAssets( AssetType type );
    Json::Value queryAssets( AssetType type, const Json::Value& query );
    Json::Value listComponents();
    Json::Value listCustomAssets();

private:
    void renderGrid( sf::RenderWindow* target, sf::Vector2f gridSize );
    GameObject* findGameObject( int handle, bool isPrefab, GameObject* parent = 0 );
    GameObject* findGameObjectById( const std::string& id, bool isPrefab, GameObject* parent = 0 );
    GameObject* findGameObjectAtPosition( const sf::Vector2f& pos, GameObject* parent = 0 );

    std::stringstream                                   m_errors;
    sf::RenderWindow*                                   m_renderWindow;
    GameManager*                                        m_gameManager;
    sf::Vector2f                                        m_gridSize;
    sf::View                                            m_sceneView;
    sf::Vector2f                                        m_cameraSize;
    float                                               m_cameraZoom;
    std::map< std::string, ComponentData >              m_components;
    std::map< std::string, CustomAssetData >            m_customAssets;
};

#endif
