#ifndef __PTAKOPYSK_INTERFACE__
#define __PTAKOPYSK_INTERFACE__

#include <sstream>
#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Singleton.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <XeCore/Common/Timer.h>
#include <windef.h>

using namespace Ptakopysk;

class PtakopyskInterface
: public virtual XeCore::Common::IRtti
, public virtual XeCore::Common::MemoryManager::Manageable
, public XeCore::Common::Singleton< PtakopyskInterface >
{
    RTTI_CLASS_DECLARE( PtakopyskInterface );

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
        ComponentData(
            const std::string& _id,
            XeCore::Common::IRtti::Derivation _type,
            Component::OnBuildComponentCallback _builder
            )
        {
            id = _id;
            type = _type;
            builder = _builder;
        }

        std::string id;
        XeCore::Common::IRtti::Derivation type;
        Component::OnBuildComponentCallback builder;
    };

    struct AssetData
    {
        AssetData(
            const std::string& _id,
            XeCore::Common::IRtti::Derivation _type,
            ICustomAsset::OnBuildCustomAssetCallback _builder
            )
        {
            id = _id;
            type = _type;
            builder = _builder;
        }

        std::string id;
        XeCore::Common::IRtti::Derivation type;
        ICustomAsset::OnBuildCustomAssetCallback builder;
    };

    PtakopyskInterface();
    virtual ~PtakopyskInterface();

    bool initialize( int64_t windowHandle, bool editMode );
    void release();
    Assets* getAssetsInstance();
    void setAssetsFileSystemRoot( const std::string& path );
    bool processEvents();
    bool processPhysics( float deltaTime, int velocityIterations, int positionIterations );
    bool processUpdate( float deltaTime, bool sortInstances );
    bool processRender();
    bool setVerticalSyncEnabled( bool enabled );
    FORCEINLINE sf::Vector2f getGridSize() { return m_gridSize; };
    FORCEINLINE void setGridSize( sf::Vector2f v ) { m_gridSize = v; };
    FORCEINLINE sf::Vector2f getSceneViewSize() { return m_cameraSize; };
    FORCEINLINE void setSceneViewSize( sf::Vector2f v ) { m_cameraSize = v; m_sceneView.setSize( m_cameraSize ); m_sceneView.zoom( m_cameraZoom ); };
    FORCEINLINE sf::Vector2f getSceneViewCenter() { return m_sceneView.getCenter(); };
    FORCEINLINE void setSceneViewCenter( sf::Vector2f v ) { m_sceneView.setCenter( v ); };
    FORCEINLINE float getSceneViewZoom() { return m_cameraZoom; };
    FORCEINLINE void setSceneViewZoom( float v ) { m_cameraZoom = v; m_sceneView.setSize( m_cameraSize ); m_sceneView.zoom( m_cameraZoom ); };
    FORCEINLINE std::string popErrors() { std::string err = m_errors.str(); m_errors.str( "" ); return err; };
    sf::Vector2f convertPointFromScreenToWorldSpace( sf::Vector2i p );
    bool clearScene();
    bool clearSceneGameObjects( bool isPrefab );
    bool applyJsonToScene( const std::string& json );
    std::string convertSceneToJson();
    int createGameObject( bool isPrefab, int parent, const std::string& prefabSource, const std::string& id );
    bool destroyGameObject( int handle, bool isPrefab );
    bool clearGameObject( int handle, bool isPrefab );
    bool duplicateGameObject( int handleFrom, bool isPrefabFrom, int handleTo, bool isPrefabTo );
    bool triggerGameObjectComponentFunctionality( int handle, bool isPrefab, const std::string& compId, const std::string& funcName );
    bool applyJsonToGameObject( int handle, bool isPrefab, const std::string& json );
    std::string convertGameObjectToJson( int handle, bool isPrefab );
    bool startQueryGameObject( int handle, bool isPrefab );
    bool queryGameObject( const std::string& query );
    int queriedGameObjectHandle();
    unsigned int queriedGameObjectResultsCount();
    bool queriedGameObjectResultNext();
    std::string queriedGameObjectResultKey();
    std::string queriedGameObjectResultValue();
    void endQueryGameObject();
    bool startIterateGameObjects( bool isPrefab );
    bool canIterateGameObjectsNext( bool isPrefab );
    bool iterateGameObjectsNext( bool isPrefab );
    bool startQueryIteratedGameObject();
    bool endIterateGameObjects();
    int findGameObjectHandleById( const std::string& id, bool isPrefab, int parent );
    int findGameObjectHandleAtPosition( float x, float y, int parent );
    int findGameObjectHandleAtScreenPosition( int x, int y, int parent );
    void startIterateAssets( AssetType type );
    bool canIterateAssetsNext( AssetType type );
    bool iterateAssetsNext( AssetType type );
    std::string getIteratedAssetId( AssetType type );
    std::string getIteratedAssetMeta( AssetType type );
    std::string getIteratedAssetTags( AssetType type );
    void endIterateAssets( AssetType type );
    bool queryAssets( AssetType type, const std::string& query );
    int pluginLoad( const std::string& path );
    bool pluginUnload( int handle );
    bool pluginUnloadByPath( const std::string& path );
    void pluginUnloadAll();
    void pluginRegisterComponent( const std::string& id, XeCore::Common::IRtti::Derivation type, Component::OnBuildComponentCallback creator );
    void pluginUnregisterComponent( const std::string& id );
    void pluginRegisterAsset( const std::string& id, XeCore::Common::IRtti::Derivation type, ICustomAsset::OnBuildCustomAssetCallback creator );
    void pluginUnregisterAsset( const std::string& id );
    void startIterateComponents();
    bool canIterateComponentsNext();
    bool iterateComponentsNext();
    std::string getIteratedComponentId();
    void endIterateComponents();
    void startIterateCustomAssets();
    bool canIterateCustomAssetsNext();
    bool iterateCustomAssetsNext();
    std::string getIteratedCustomAssetId();
    void endIterateCustomAssets();

private:
    void renderGrid( sf::RenderWindow* target, sf::Vector2f gridSize );
    GameObject* findGameObject( int handle, bool isPrefab, GameObject* parent = 0 );
    GameObject* findGameObjectById( const std::string& id, bool isPrefab, GameObject* parent = 0 );
    GameObject* findGameObjectAtPosition( const sf::Vector2f& pos, GameObject* parent );

    std::stringstream                                   m_errors;
    sf::RenderWindow*                                   m_renderWindow;
    GameManager*                                        m_gameManager;
    Assets*                                             m_assets;
    GameObject*                                         m_queriedGameObject;
    std::map< std::string, Json::Value >                m_queriedGameObjectResult;
    std::map< std::string, Json::Value >::iterator      m_queriedGameObjectCurrentIterator;
    std::stack< GameObject::List::iterator >            m_gameObjectIteratorStack;
    GameObject::List::iterator                          m_gameObjectCurrentIterator;
    bool                                                m_gameObjectIsIterating;
    std::map< std::string, sf::Texture* >::iterator     m_assetTextureIterator;
    std::map< std::string, sf::Shader* >::iterator      m_assetShaderIterator;
    std::map< std::string, sf::Sound* >::iterator       m_assetSoundIterator;
    std::map< std::string, sf::Music* >::iterator       m_assetMusicIterator;
    std::map< std::string, sf::Font* >::iterator        m_assetFontIterator;
    std::map< std::string, ICustomAsset* >::iterator    m_assetCustomIterator;
    sf::Vector2f                                        m_gridSize;
    sf::View                                            m_sceneView;
    sf::Vector2f                                        m_cameraSize;
    float                                               m_cameraZoom;
    std::map< std::string, HINSTANCE >                  m_plugins;
    std::vector< ComponentData >                        m_componentsPending;
    std::vector< AssetData >                            m_assetsPending;
    std::vector< std::string >                          m_queriedComponentsIds;
    std::vector< std::string >::iterator                m_queriedComponentsIdsIterator;
    std::vector< std::string >                          m_queriedCustomAssetsIds;
    std::vector< std::string >::iterator                m_queriedCustomAssetsIdsIterator;
};

#endif
