#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "zombie.hpp"
#include "player.hpp"

using std::atan;
using std::cos;
using std::sin;

const float pi = 4 * atan( 1 );

inline float toDegree( float radians )
{
    return radians * (180 / pi);
}

inline float toRadian( float degree )
{
    return degree * (pi / 180);
}

Zombie::Zombie( float speed, float health ) :
    speed( speed ),
    health( health )
{
    texture.loadFromFile( "media/zombie.png" );

    groan_buffer.loadFromFile( "media/zombie groan.wav" );
    groan_sound.setBuffer( groan_buffer );

    groan_time.restart( sf::seconds( 10 ) );
}

void Zombie::follow( Player& player)
{
    const sf::Vector2f& player_pos = player.getSprite().getPosition();
    sf::Vector2f delta;
    sf::Vector2f direction;
    float rotation;
    for ( auto& a : zombies )
    {
        delta.x = player_pos.x - a.getPosition().x;
        delta.y = player_pos.y - a.getPosition().y;
        rotation = toDegree( std::atan2( delta.y, delta.x ) );
        a.setRotation( rotation );

        direction.x = speed * cos( toRadian( rotation ) );
        direction.y = speed * sin( toRadian( rotation ) );
        a.move( direction );

        if ( a.getGlobalBounds().intersects( player.getSprite().getGlobalBounds() ) ) player.decHealth();
    }
}

void Zombie::addOne()
{
    std::default_random_engine generator;
    std::uniform_real_distribution< float > distribution_x{ 0.f, 800.f };
    std::uniform_real_distribution< float > distribution_y{ 0.f, 600.f };
    float x = distribution_x( generator );
    float y = distribution_y( generator );
    zombies.emplace_back( sf::Sprite{} );
    zombies.back().setPosition( x, y );
    zombies.back().setOrigin( zombies.back().getTextureRect().width / 2, zombies.back().getTextureRect().height / 2);
    zombies.back().setTexture( texture );
    zombies.back().scale( 2.f, 2.f );
    health.push_back( 5 );
}

int Zombie::checkHealth()
{
    int nDead = 0;
    std::chrono::system_clock::time_point current = std::chrono::system_clock::now();
    long long int time = std::chrono::system_clock::to_time_t( current );
    std::default_random_engine generator( time );
    std::uniform_real_distribution< float > distribution_x{ 0.f, 800.f };
    std::uniform_real_distribution< float > distribution_y{ 0.f, 600.f };
    int i = -1;
    for ( auto& a : health )
    {
        ++i;
        if ( a <= 0 )
        {
            nDead++;
            a = 5;
            zombies.at( i ).setPosition( distribution_x( generator ), distribution_y( generator ) );
            addOne();
        }
    }
    return nDead;
}

void Zombie::makeSound()
{
    if ( groan_time.isExpired() )
    {
        groan_time.restart( sf::seconds( 10 ) );
        std::default_random_engine generator;
        std::uniform_int_distribution< int > distribution( 1, 2 );
        if ( distribution( generator )  == 1) groan_sound.play();
    }
}

void Zombie::decHealth( unsigned int i )
{
    --health.at( i );
}

std::vector< sf::Sprite >& Zombie::getZombies()
{
    return zombies;
}
