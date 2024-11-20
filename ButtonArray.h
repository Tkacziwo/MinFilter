#pragma once
#include "Button.h"
#include "FileEntry.h"

class ButtonArray
{
private:
	std::vector<Button> buttons;
	sf::Vector2f arrayOffset;
	std::vector<sf::Texture> textures;
	sf::Font font;
	int characterSize;
	FileEntry label;
	std::string originalStr;
	int threadsNumber;
public:
	ButtonArray()
	{
		characterSize = 30;
		font.loadFromFile("Arial.ttf");
	}

	ButtonArray(sf::Vector2f offset, std::vector<sf::Texture>& textures)
	{
		this->textures = textures;
		arrayOffset = offset;
		characterSize = 40;
		font.loadFromFile("Arial.ttf");
		originalStr = "Number or threads: ";
		this->label = FileEntry(sf::Vector2f(arrayOffset.x, arrayOffset.y + 45), originalStr, font, characterSize);
		int factorOf2 = 1;
		for (int i = 1; i <= 7; i++)
		{
			buttons.push_back(CreateButton(factorOf2));
			arrayOffset.x += 55;
			factorOf2 *= 2;
		}
	}

	Button CreateButton(int i)
	{
		Button btn(arrayOffset, sf::Vector2f(50, 50), i, textures, 50, 50);
		btn.SetText(std::to_string(i), font, characterSize);
		return btn;
	}

	void OnClick(const sf::Vector2i& mousePos)
	{
		for (auto& b : buttons)
		{
			if (b.IsMouseInside(mousePos))
			{
				std::cout << "Number of threads: " << b.Action(mousePos) << "\n";
				threadsNumber = b.Action(mousePos);
				SetLabel(std::to_string(b.Action(mousePos)));
				break;
			}
		}
	}

	void SetLabel(std::string str)
	{
		label.SetEntry(originalStr);
		std::string temp = label.GetText();
		temp += " " + str;
		label.SetStr(temp, characterSize);

	}

	void OnHover(const sf::Vector2i& mousePos)
	{
		for (auto& b : buttons)
		{
			b.OnHover(b.IsMouseInside(mousePos));
		}
	}

	void Draw(sf::RenderTarget& target)
	{
		for (auto& b : buttons)
		{
			b.Draw(target);
		}
		label.Draw(target);
	}

	int GetThreadsNumber() const noexcept
	{
		return threadsNumber;
	}

	~ButtonArray()
	{

	}
};