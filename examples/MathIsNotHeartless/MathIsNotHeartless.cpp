#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Events.h>
#include <Ptakopysk/System/Tween.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>
#include "HeartControler.h"

using namespace Ptakopysk;

const std::string APP_NAME = "Math Is Not Heartless";
const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;
const sf::Color WINDOW_COLOR = sf::Color( 255, 255, 255, 255 );
//const sf::Color WINDOW_COLOR = sf::Color( 128, 128, 128, 255 );

void onEvent( Events::Event* ev )
{
}

int main()
{
    /// initialization
    //LOG_SETUP( "log.log" );
    Events::use().setCallback( &onEvent );
    GameManager::initialize();
    GameManager::registerComponentFactory( "HeartControler", RTTI_CLASS_TYPE( HeartControler ), HeartControler::onBuildComponent );

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
    gameManager->jsonToScene( GameManager::loadJson( "scene.json" ) );

    /// main loop
    srand( time( 0 ) );
    sf::Clock timer;
    sf::Clock deltaTimer;
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
        float dt = deltaTimer.getElapsedTime().asSeconds();
        deltaTimer.restart();

        /// process frame
        Events::use().dispatch();
        Tweener::use().processTweens( dt );
        gameManager->processPhysics( dt );
        gameManager->processUpdate( dt );
        window->clear( WINDOW_COLOR );
        gameManager->processRender();
        window->display();
        XeCore::Common::Concurrent::Thread::sleep( 1000 / 30 );
    }

    /// serialize scene to JSON
    //GameManager::saveJson( "_scene.json", gameManager->sceneToJson() );

    DELETE_OBJECT( window );
    DELETE_OBJECT( gameManager );
    Assets::destroy();
    Events::destroy();
    Tweener::destroy();

    return 0;
}
