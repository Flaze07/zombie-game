#include <iostream>
#include <cmath>
#include <vector>
#include <THOR/Input.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.hpp"

using std::atan;
using std::cos;
using std::sin;

const float pi = 4 * std::atan( 1 );

inline float toDegree( float radians )
{
    return radians * (180 / pi);
}

inline float toRadian( float degree )
{
    return degree * (pi / 180);
}


Player::Player( float speed, float bullet_speed ) :
    speed( speed ),
    health( 500 ),
    bullet_speed( bullet_speed ),
    shot( false )
{
    texture.loadFromFile( "media/human.png" );

    sprite.setTexture( texture );
    sprite.setOrigin( sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2 );
    sprite.move( sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2 );
    sprite.scale( 2.f, 2.f );

    sound_buffer.loadFromFile( "media/gun sound.wav");
    gun_sound.setBuffer( sound_buffer );

    //set actions
    action_map[ "walk forward" ] = thor::Action{ sf::Keyboard::W, thor::Action::Hold };
    action_map[ "walk backward" ] = thor::Action{ sf::Keyboard::S, thor::Action::Hold };
    action_map[ "walk left" ] = thor::Action{ sf::Keyboard::A, thor::Action::Hold };
    action_map[ "walk right" ] = thor::Action{ sf::Keyboard::D, thor::Action::Hold };
    action_map[ "shoot" ] = thor::Action{ sf::Mouse::Left, thor::Action::PressOnce };

    shoot_countdown.restart( sf::milliseconds( 1 ) );
}

void Player::faceMouse( const sf::Vector2f& mouse_pos )
{
    float delta_x = mouse_pos.x - sprite.getPosition().x;
    float delta_y = mouse_pos.y - sprite.getPosition().y;
    float rotation = toDegree( std::atan2( delta_y, delta_x ) );
    sprite.setRotation( rotation );
}

void Player::action( sf::RenderWindow& win, sf::Event& event )
{
    sf::Rect< float > border{ 0, 0, static_cast<float>( win.getSize().x ), static_cast<float>( win.getSize().y ) };

    action_map.pushEvent( event );
    float radians = toRadian( sprite.getRotation() );
    sf::Vector2f direction;
    if ( action_map.isActive( "walk forward" ) )
    {
        direction.x = speed * cos( radians );
        direction.y = speed * sin( radians );
    }
    if ( action_map.isActive( "walk backward" ) )
    {
        direction.x = -speed * cos( radians );
        direction.y = -speed * sin( radians );
    }
    if ( action_map.isActive( "walk left" ) )
    {
        direction.x = speed * cos( radians + toRadian( 90 ) );
        direction.y = speed * sin( radians + toRadian( 90 ) );
    }
    if ( action_map.isActive( "walk right" ) )
    {
        direction.x = -speed * cos( radians + toRadian( 90 ) );
        direction.y = -speed * sin( radians + toRadian( 90 ) );
    }
    if ( action_map.isActive( "shoot" ) )
    {
        if ( shoot_countdown.isExpired() )
        {
            if ( !shot )
            {
                bullet.emplace_back( sf::Vector2f{ 10, 2} );
                bullet.back().setRotation( sprite.getRotation() );
                bullet.back().setPosition( sprite.getPosition() );

                bullet_line.emplace_back( sf::Vector2f{ ( sqrt( 800 * 600 ) ), 5 } );
                bullet_line.back().setRotation( sprite.getRotation() );
                bullet_line.back().setOrigin( 0, 5 );
                bullet_line.back().setPosition( sprite.getPosition() );
                bullet_line.back().setFillColor( sf::Color::Yellow );
                gun_sound.play();
                shot = true;
                shoot_countdown.restart( sf::milliseconds( 200 ) );
            }
            shot = false;
        }
    }
    sprite.move( direction );

    if ( !border.contains( sprite.getPosition() ) )
    {
        direction.x = -direction.x;
        direction.y = -direction.y;
        sprite.move( direction );
    }
    action_map.clearEvents();
}

void Player::updateBullet( sf::RenderWindow& win )
{
    sf::Rect< unsigned int > win_box{ 0, 0, win.getSize().x, win.getSize().y };
    if ( bullet.size() == 0 ) return;
    int i = -1;
    for ( auto& a : bullet )
    {
        ++i;
        float radian = toRadian( a.getRotation() );
        sf::Vector2f direction;
        direction.x = bullet_speed * cos( radian );
        direction.y = bullet_speed * sin( radian );
        a.move( direction );

        if ( !win_box.contains( static_cast< sf::Vector2u >( a.getPosition() ) ) )
        {
            bullet.erase( bullet.begin() + i );
            bullet_line.erase( bullet_line.begin() + i );
        }
    }
}

void Player::checkHitZombie( Zombie& zombie )
{
    int i_zombies = 0;
    for ( auto& a : zombie.getZombies() )
    {
        int i_bullet = 0;
        if ( bullet.size() == 0 ) return;
        for ( auto& i : bullet_line )
        {
            if ( bullet.size() == 0) break;
            if ( i.getGlobalBounds().intersects( a.getGlobalBounds() ) )
            {
                zombie.decHealth( i_zombies );
            }
            ++i_bullet;
        }
        ++i_zombies;
    }
}

void Player::decHealth()
{
    --health;
}

sf::Sprite& Player::getSprite()
{
    return sprite;
}

std::vector< sf::RectangleShape >& Player::getBullet()
{
    return bullet_line;
}

float& Player::getHealth()
{
    return health;
}
