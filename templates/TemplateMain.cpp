#include <Ptakopysk/System/Assets.h>
#include <Ptakopysk/System/Events.h>
#include <Ptakopysk/System/GameManager.h>
#include <XeCore/Common/Logger.h>
#include <XeCore/Common/Concurrent/Thread.h>

using namespace Ptakopysk;

const std::string APP_NAME = "Template Application";
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 574;
const sf::Color WINDOW_COLOR = sf::Color( 128, 128, 128, 255 );

void onEvent( Events::Event* ev )
{
}

int main()
{
    /// initialization
    LOG_SETUP( "log.log" );
    Events::use().setCallback( &onEvent );
    GameManager::initialize();

    /// scene
    sf::RenderWindow* window = xnew sf::RenderWindow(
        sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ),
        APP_NAME,
        sf::Style::Titlebar | sf::Style::Close
    );

    /// game manager
    GameManager* gameManager = xnew GameManager();
    /// deserialize JSON to scene
    gameManager->jsonToScene( gameManager->loadJson( "assets/scenes/game.json" ) );

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
        gameManager->processPhysics( dt );
        gameManager->processUpdate( dt );
        window->clear( WINDOW_COLOR );
        gameManager->processRender( window );
        window->display();
        XeCore::Common::Concurrent::Thread::sleep( 1000 / 30 );
    }

    /// serialize scene to JSON
    gameManager->saveJson( "assets/scenes/_game.json", gameManager->sceneToJson() );

    DELETE_OBJECT( window );
    DELETE_OBJECT( gameManager );
    Assets::destroy();
    Events::destroy();

    return 0;
}
