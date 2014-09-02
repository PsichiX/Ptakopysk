#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Events.h>
#include <Ptakopysk/System/Tween.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Logger.h>
#include "_include_components.h"
#include "_include_assets.h"

using namespace Ptakopysk;

void onEvent( Events::Event* ev )
{
}

int main()
{
    LOG_SETUP( "log.log" );
    Events::use().setCallback( &onEvent );

    GameManager::initialize();
#include "_register_components.inl"
#include "_register_assets.inl"

    Json::Value config = Assets::loadJson( "config.json" );
    GameManager* gameManager = xnew GameManager( config );
    gameManager->runScene( "main" );
    gameManager->processLifeCycle();
    DELETE_OBJECT( gameManager );
    GameManager::cleanup();
    Assets::destroy();
    Events::destroy();
    Tweener::destroy();

    return 0;
}
