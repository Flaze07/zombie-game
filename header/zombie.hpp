#ifndef ZOMBIE_HPP_INCLUDED
#define ZOMBIE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <THOR/Time.hpp>
#include "player.hpp"

class Player;

class Zombie
{
private :
    sf::Texture texture;
    std::vector< sf::Sprite > zombies;
    const float speed;
    std::vector< unsigned int > health;
    sf::SoundBuffer groan_buffer;
    sf::Sound groan_sound;
    thor::Timer groan_time;
public :
    //constructor
    Zombie( float speed, float health );
    //other
    void follow( Player& player );
    void addOne();
    int checkHealth();
    void makeSound();
    void decHealth( unsigned int i );
    //getter
    std::vector< sf::Sprite >& getZombies();
};

#endif // ZOMBIE_HPP_INCLUDED
