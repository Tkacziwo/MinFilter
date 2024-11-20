#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace hit {
	class Hitbox
	{
	private:
		sf::FloatRect Bounds;
	public:
		Hitbox() {};

		bool CheckHitbox(const sf::Vector2i& mousePos)
		{
			if (Bounds.contains(sf::Vector2f(mousePos)))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void operator = (sf::RectangleShape shape)
		{
			Bounds = sf::FloatRect(shape.getGlobalBounds());
		}
	};
}