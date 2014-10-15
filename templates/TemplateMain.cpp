#include <Ptakopysk/System/GameManager.h>
#include <Ptakopysk/System/Network.h>
#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Events.h>
#include <Ptakopysk/System/Tween.h>
#include <XeCore/Common/Logger.h>
#include "_include_components.h"
#include "_include_assets.h"

using namespace Ptakopysk;

void onEvent( Events::Event* ev )
{
}

int main( int argc, char* argv[] )
{
    LOG_SETUP( "log.log" );
    Events::use().setCallback( &onEvent );

    GameManager::initialize();
#include "_register_components.inl"
#include "_register_assets.inl"

    std::string runScene = "main";
    Json::Value config = Assets::loadJson( "config.json" );
    GameManager* gameManager = xnew GameManager( config );
    if( !gameManager->runScene( runScene ) )
        LOGNL( "Cannot run scene: %s", runScene.c_str() );
    gameManager->processLifeCycle();
    DELETE_OBJECT( gameManager );
    GameManager::cleanup();
    Network::destroy();
    Assets::destroy();
    Events::destroy();
    Tweener::destroy();

    return 0;
}
