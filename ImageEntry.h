#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class ImageEntry
{
private:
	sf::Text text;
	sf::RectangleShape base;
	sf::Font font;
	sf::Vector2f textPosition;
	sf::Texture imageTexture;
	sf::Sprite imageSprite;
	int targetWidth, targetHeight;
	bool imageReady = false;
public:

	ImageEntry()
	{
		targetHeight = 0;
		targetWidth = 0;
	}

	ImageEntry(sf::Vector2f offset, sf::Text& text, int targetWidth, int targetHeight)
	{
		this->targetWidth = targetWidth;
		this->targetHeight = targetHeight;
		this->text = text;
		this->text.setPosition(offset);
		this->font.loadFromFile("Arial.ttf");
		base.setSize(sf::Vector2f(10, 10));
		base.setFillColor(sf::Color::Blue);
		base.setPosition(offset);
		textPosition = offset;
		imageSprite.setPosition(offset.x, offset.y + 50);
	}

	void LoadImageFromFile(const std::string& filename)
	{
		try
		{
			sf::Image tmp;
			tmp.loadFromFile(filename);
			imageTexture.loadFromImage(tmp);
			imageSprite.setTexture(imageTexture);
			imageSprite.scale(targetWidth / imageSprite.getLocalBounds().width, targetHeight / imageSprite.getLocalBounds().height);
			imageReady = true;
		}
		catch (const std::exception& e)
		{
			std::cout << "Something went wrong with the image: " << e.what() << "\n";
		}
	}

	void AfterLoading(const std::string& message)
	{
		text.setString(message);
	}

	void Draw(sf::RenderTarget& target)
	{
		this->text.setFont(this->font);
		target.draw(this->text);
		target.draw(imageSprite);
	}

	bool ImageReady() const noexcept
	{

		return imageReady;
	}

	void ImageNotRead()
	{
		imageReady = false;
	}

	~ImageEntry()
	{
	}
};