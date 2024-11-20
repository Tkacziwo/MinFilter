#pragma once
#include "Hitbox.h"
#include <iostream>
class Button
{
protected:
	sf::RectangleShape Base;
	sf::Vector2f Offset;
	hit::Hitbox hitbox;
	sf::Texture texture;
	sf::Texture hoverTexture;
	sf::Text text;
	sf::Font font;
	int ActionId;
public:
	Button()
	{
		ActionId = 0;
	}

	Button(sf::Vector2f offset, int actionId) : ActionId(actionId)
	{
		Offset = offset;
		Base.setSize(sf::Vector2f(100, 100));
		Base.setPosition(Offset);
		Base.setFillColor(sf::Color::Red);
		hitbox = Base;
		this->text = sf::Text();
	}

	Button(sf::Vector2f offset, sf::Vector2f size, int actionId, std::vector<sf::Texture>& texture, int targetWidth, int targetHeight) : ActionId(actionId)
	{
		Offset = offset;
		Base.setPosition(Offset);
		Base.setSize(size);
		sf::Sprite spr;
		this->texture = texture[0];
		spr.setTexture(texture[0]);
		this->hoverTexture = texture[1];
		spr.scale(targetWidth / spr.getLocalBounds().width, targetHeight / spr.getLocalBounds().height);
		this->texture = *spr.getTexture();
		Base.setTexture(&this->texture);
		hitbox = Base;
		this->text = sf::Text();
	}

	void SetText(std::string str, sf::Font font, int characterSize)
	{
		this->font = font;
		this->text = sf::Text(str, font, characterSize);
		this->text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		this->text.setPosition(Base.getPosition() + (Base.getSize() / 2.f));
		this->text.setFillColor(sf::Color::Black);
	}

	Button(sf::Vector2f offset, int actionId, sf::Vector2f size) : ActionId(actionId)
	{
		Offset = offset;
		Base.setSize(size);
		Base.setPosition(Offset);
		Base.setFillColor(sf::Color::Red);
		hitbox = Base;
	}

	void SetPosition(const sf::Vector2f pos)
	{
		Base.setPosition(pos);
		hitbox = Base;
	}

	bool IsMouseInside(const sf::Vector2i& mousePos)
	{
		return hitbox.CheckHitbox(mousePos);
	}

	int Action(const sf::Vector2i& mousePos) const
	{
		return ActionId;
	}

	void OnHover(bool inside)
	{
		if (inside)
		{
			Base.setTexture(&hoverTexture);
		}
		else
		{
			Base.setTexture(&texture);
		}
	}

	virtual void Draw(sf::RenderTarget& target)
	{
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(Base.getPosition() + (Base.getSize() / 2.f));
		target.draw(Base);
		target.draw(text);
	}
};