#pragma once
#include "FileEntry.h"
class ProgramStatus : public FileEntry
{
private:
	std::string originalStr;

public:
	ProgramStatus()
	{
	};

	ProgramStatus(sf::Vector2f offset, const std::string str, sf::Font& font, int characterSize)
	{
		this->font = font;
		this->text = sf::Text(str, font, characterSize);
		this->text.setPosition(offset);
		base.setSize(sf::Vector2f(10, 10));
		base.setFillColor(sf::Color::Blue);
		base.setPosition(offset);
		textPosition = offset;
		originalStr = str;
	}

	void AppendText(const std::string& str)
	{
		std::string textStr = text.getString();
		originalStr = textStr;
		textStr.append(" " + str);
		text.setString(textStr);
	}

	void RestoreOriginalText()
	{
		text.setString(originalStr);
	}
	void HandleProgramStatusChange(int statusCode)
	{
		RestoreOriginalText();
		switch (statusCode)
		{
		case 10:
			AppendText("File not selected");
			break;
		case 20:
			AppendText("Successfully selected file");
			break;
		case 30:
			AppendText("Filtering...");
			break;
		case 40:
			AppendText("Filtering finished!");
			break;
		case 50:
			AppendText("Chosen Cpp library");
			break;
		case 60:
			AppendText("Chosen Asm library");
			break;
		default:
			break;
		}
	}

	~ProgramStatus()
	{

	}
};