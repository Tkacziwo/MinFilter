#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class FileEntry
{
protected:
	sf::Text text;
	sf::Font font;
	sf::RectangleShape base;
	sf::Vector2f textPosition;
public:
	FileEntry()
	{
		base.setSize(sf::Vector2f(10, 10));
		base.setFillColor(sf::Color::Blue);
	}

	FileEntry(sf::Vector2f& offset, sf::Text& text, sf::Font& font)
	{
		this->text = text;
		this->text.setPosition(offset);
		this->font = font;
		base.setSize(sf::Vector2f(10, 10));
		base.setFillColor(sf::Color::Blue);
		base.setPosition(offset);
		textPosition = offset;
	}

	FileEntry(sf::Vector2f offset, const std::string str, sf::Font& font, int characterSize)
	{
		this->font = font;
		this->text = sf::Text(str, font, characterSize);
		this->text.setPosition(offset);
		base.setSize(sf::Vector2f(10, 10));
		base.setFillColor(sf::Color::Blue);
		base.setPosition(offset);
		textPosition = offset;
	}

	void SetFont(sf::Font& font)
	{
		this->font = font;
	}

	void SetEntry(std::string& text)
	{
		this->text = sf::Text(text, font, 20);
		this->text.setPosition(textPosition);
	}

	void SetStr(std::string& str, int characterSize)
	{
		this->text = sf::Text(str, font, characterSize);
		this->text.setPosition(textPosition);
	}

	void SetOffset(sf::Vector2f& offset)
	{
		this->base.setPosition(offset);
		this->text.setPosition(offset);
		textPosition = text.getPosition();
	}

	void Draw(sf::RenderTarget& target)
	{
		text.setFont(font);
		target.draw(base);
		target.draw(text);
	}

	std::string GetText() const noexcept
	{
		return this->text.getString();
	}

	~FileEntry()
	{

	}
};