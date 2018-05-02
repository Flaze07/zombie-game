#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "header/player.hpp"
#include "header/zombie.hpp"

int main()
{
    sf::RenderWindow win{ sf::VideoMode{ 800, 600 }, "win" };
    win.setFramerateLimit( 60 );

    Player player( 1.5f, 200.f );

    Zombie zombie{ 0.5f, 5 };
    zombie.addOne();

    sf::Font font;
    font.loadFromFile( "media/font.otf" );

    sf::Text player_health;
    player_health.setFont( font );
    player_health.setOrigin( player_health.getLocalBounds().width / 2, player_health.getLocalBounds().height / 2 );
    player_health.setPosition( 300, 0 );

    unsigned int highscore, temp = 0;

    std::fstream file{ "highscore.txt", std::ios::in | std::ios::out };

    if ( file.is_open() )
    {
        file >> highscore;
    }

    while ( win.isOpen() )
    {
        sf::Event event;
        player.checkHitZombie( zombie );
        while ( win.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed ) win.close();
            if ( ( event.type == sf::Event::KeyPressed ) && ( event.key.code == sf::Keyboard::Escape ) ) win.close();
            if ( !win.isOpen() ) break;
            player.action( win, event );
        }

        player.faceMouse( static_cast< sf::Vector2f >( sf::Mouse::getPosition( win ) ) );
        player.updateBullet( win );

        zombie.follow( player );
        zombie.makeSound();
        temp += zombie.checkHealth();

        player_health.setString( std::to_string( static_cast< int >( player.getHealth() ) ) );

        if ( player.getHealth() <= 0 )
        {
            win.close();
            break;
        }

        win.clear();
        win.draw( player_health );
        win.draw(  player.getSprite() );
        for ( auto& a : player.getBullet() )
        {
            win.draw( a );
        }
        for ( auto& a : zombie.getZombies() )
        {
            win.draw( a );
        }
        win.display();
    }
    if ( temp - 4 > highscore )
    {
        highscore = temp - 4;
    }
    file << highscore;
    std::cout << "highscore : " << highscore << "\n";
    std::cout << "your score : " << temp - 4 << "\n";
    file.flush();
    std::cin.ignore();
}
