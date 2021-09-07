//----------------------------------include section-----------------------------------
#pragma once
#include <SFML/Graphics.hpp>

//----------------------------------enum class definition-----------------------------
enum class Direction
{
    Up,
    Down,
    Right,
    Left,
    Stay,
    Max,
};

enum class EnemyAction
{
	Idle,					// 0
    Walk,                   // 1
	Attack,                 // 2
    Hit                     // 3
};

enum class WeaponAction
{
	Fly,					// 0
	Explode,                // 1
};

enum class rubyAction
{
	Shine,                  // 0
};

//----------------------------------functions definition------------------------------
Direction opposite(Direction dir);
sf::Vector2f toVector(Direction dir);