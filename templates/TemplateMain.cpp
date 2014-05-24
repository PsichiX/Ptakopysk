#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Events.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include <XeCore/Common/Timer.h>
//<TEMPLATE
#include "TemplateComponent.h"
//TEMPLATE>

using namespace Ptakopysk;

const std::string APP_NAME = "Template Application";
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 574;
const sf::Color WINDOW_COLOR = sf::Color( 255, 255, 255, 255 );

void onEvent( Events::Event* ev )
{
}

int main()
{
    /// initialization
    LOG_SETUP( "log.log" );
    Events::use().setCallback( &onEvent );
    GameManager::initialize();
    //<TEMPLATE
	GameManager::registerComponentFactory( "TemplateComponent", RTTI_CLASS_TYPE( TemplateComponent ), TemplateComponent::onBuildComponent );
	//TEMPLATE>

    /// scene
    sf::RenderWindow* window = xnew sf::RenderWindow(
        sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ),
        APP_NAME,
        sf::Style::Titlebar | sf::Style::Close
    );

    /// game manager
    GameManager* gameManager = xnew GameManager();
	gameManager->RenderWindow = window;
    /// deserialize JSON to scene
    gameManager->jsonToScene( GameManager::loadJson( "template_game.json" ) );
    //GameManager::saveJson( "template_game.bin", GameManager::loadJson( "template_game.json" ), true, MAXDWORD );
    //gameManager->jsonToScene( GameManager::loadJson( "template_game.bin", true, MAXDWORD ) );

    /// main loop
    srand( time( 0 ) );
    XeCore::Common::Timer timer;
    timer.start();
    const float fixedStep = 1.0f / 30.0f;
    float fixedStepAccum = 0.0f;
    while( window->isOpen() )
    {
        sf::Event event;
        while( window->pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
            {
                window->close();
            }
            else if( event.type == sf::Event::KeyPressed )
            {
                if( event.key.code == sf::Keyboard::Escape )
                    window->close();
            }
        }
        /// timers update
        timer.update();
        float dt = timer.deltaSeconds();
        fixedStepAccum += dt;
        bool processFixedStep = false;
        while( fixedStepAccum > fixedStep )
        {
            processFixedStep = true;
            fixedStepAccum -= fixedStep;
        }

        /// process frame
        Events::use().dispatch();
        if( processFixedStep )
        {
            gameManager->processPhysics( fixedStep );
            gameManager->processUpdate( fixedStep );
        }
        window->clear( WINDOW_COLOR );
        gameManager->processRender( window );
        window->display();
        XeCore::Common::Concurrent::Thread::sleep( 1000 / 60 );
    }
    timer.stop();

    /// serialize scene to JSON
    GameManager::saveJson( "_template_game.json", gameManager->sceneToJson() );

    DELETE_OBJECT( window );
    DELETE_OBJECT( gameManager );
    Assets::destroy();
    Events::destroy();

    return 0;
}
