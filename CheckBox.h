#pragma once
#include "Button.h"
#include "FileEntry.h"

class CheckBox : public Button
{
private:
	bool state = false;
	FileEntry label;
public:
	CheckBox()
	{}

	CheckBox(sf::Vector2f offset, sf::Vector2f size, std::vector<sf::Texture>& texture, int targetWidth, int targetHeight, const std::string label)
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
		this->label = FileEntry(sf::Vector2f(Offset.x + 55, Offset.y), label, font, 40);
		ActionId = 100;
	}

	void SetFont(sf::Font& font)
	{
		this->font = font;
	}

	void OnClick()
	{
		if (text.getString() == "X")
		{
			SetText("", font, 50);
			state = false;
		}
		else
		{
			SetText("X", font, 50);
			state = true;
		}
	}

	void ChangeState()
	{
		if (state)
		{
			state = false;
		}
		else
		{
			state = true;
		}
	}

	bool GetState() const noexcept
	{
		return state;
	}

	void SetAction(int id)
	{
		ActionId = id;
	}

	int GetAction() const noexcept
	{
		return ActionId;
	}

	void Draw(sf::RenderTarget& target) override
	{
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getGlobalBounds().getSize() / 2.f + text.getLocalBounds().getPosition());
		text.setPosition(Base.getPosition() + (Base.getSize() / 2.f));
		target.draw(Base);
		target.draw(text);
		label.SetFont(font);
		label.Draw(target);
	}

	~CheckBox()
	{

	}


};