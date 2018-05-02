#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <THOR/Input.hpp>
#include <THOR/Time.hpp>
#include "zombie.hpp"

class Zombie;

class Player
{
private :
    const float speed;
    float health;
    sf::Sprite sprite;
    sf::Texture texture;
    thor::ActionMap< std::string > action_map;
    std::vector< sf::RectangleShape > bullet;
    std::vector< sf::RectangleShape > bullet_line;
    const float bullet_speed;
    sf::SoundBuffer sound_buffer;
    sf::Sound gun_sound;
    thor::Timer shoot_countdown;
    bool shot;
public :
    //constructor
    Player( float speed, float bullet_speed );
    //other
    void faceMouse( const sf::Vector2f& mouse_pos);
    void action( sf::RenderWindow& win, sf::Event& event );
    void updateBullet( sf::RenderWindow& win );
    void checkHitZombie( Zombie& zombie );
    void decHealth();
    //getters
    sf::Sprite& getSprite();
    std::vector< sf::RectangleShape >& getBullet();
    float& getHealth();
};

#endif // PLAYER_H_INCLUDED
