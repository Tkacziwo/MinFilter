#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Button.h"
#include <vector>
#include "FileEntry.h"
#include <string>;
#include <fstream>
#include <iostream>
#include <filesystem>
#include <regex>

class FileManager
{
private:
	sf::RenderWindow* managerWindow;
	Button* testBtn;
	std::vector<FileEntry> fileEntries;
	sf::Vector2i MousePosition;
	int page = 1;
	int maxElementPerPage = 10;
	std::string playerInput;
	sf::Text playerText;
	FileEntry playerEntry;
	std::filesystem::path path = "..\\";
	sf::Font font;
	int characterSize = 20;
	sf::Vector2f entriesOffset = sf::Vector2f(50, 50);
	sf::Vector2f originalEntriesOffset = sf::Vector2f(50, 50);
	int allPages;
	std::string bmpFileName;
public:
	bool goodFileFound = false;
	FileManager()
	{
		managerWindow = nullptr;
		testBtn = new Button(sf::Vector2f(0, 0), 0);
		font.loadFromFile("Arial.ttf");
		UpdateFileEntries();
		playerEntry.SetFont(font);
		playerEntry.SetOffset(entriesOffset);
	}

	void AddButton(sf::Vector2f offset, int actionId)
	{
		testBtn = new Button(offset, actionId);
	}

	void CalculateNumberOfAllPages(const std::vector<std::filesystem::directory_entry>& files)
	{
		if (files.size() % maxElementPerPage == 0)
		{
			allPages = files.size() / maxElementPerPage;
		}
		else
		{
			allPages = (files.size() / maxElementPerPage) + 1;
		}
	}

	void OpenWindow()
	{
		managerWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Open file");
		managerWindow->setFramerateLimit(30);
	}

	void CloseWindow()
	{
		managerWindow->close();
	}

	bool IsOpen()
	{
		if (managerWindow != nullptr && managerWindow->isOpen())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void UpdateFileEntries()
	{
		std::vector<std::filesystem::directory_entry> files;
		entriesOffset = originalEntriesOffset;
		fileEntries.erase(fileEntries.begin(), fileEntries.end());
		sf::Text text2("Current path: " + path.string(), font, 20);
		fileEntries.push_back(FileEntry(entriesOffset, text2, font));
		entriesOffset.y += 30;
		int i{ 0 };

		for (auto& f : std::filesystem::directory_iterator(path))
		{
			files.push_back(f);
		}
		CalculateNumberOfAllPages(files);
		for (int i = maxElementPerPage * (page - 1); i < maxElementPerPage * page; i++)
		{
			if (i < files.size())
			{
				std::string fileName = files[i].path().filename().string();
				sf::Text tempText(fileName, font, characterSize);
				FileEntry tempEntry(entriesOffset, tempText, font);
				fileEntries.push_back(tempEntry);
				entriesOffset.y += 30;
			}
		}
		std::string pageString = "Showing page: " + std::to_string(page) + "Of: " + std::to_string(allPages);
		sf::Text pageText(pageString, font, characterSize);
		fileEntries.push_back(FileEntry(entriesOffset, pageText, font));
		entriesOffset.y += 30;
	}

	void Draw()
	{
		for (int i = 0; i < fileEntries.size(); i++)
		{
			fileEntries[i].Draw(*managerWindow);
		}
		playerEntry.Draw(*managerWindow);
	}

	void HandleUserInput()
	{
		std::cout << "Handling user input";
		std::string userInput = playerEntry.GetText();

		if (userInput == "cd ..")
		{
			std::cout << "Going up\n";
			std::string newPath;
			newPath = path.string() + "..\\";
			path.assign(newPath);
			UpdateFileEntries();
		}

		else if (userInput == "exit")
		{
			std::cout << "Exiting\n";
			CloseWindow();
		}

		else if (userInput.substr(0, 4) == "open")
		{
			try
			{
				std::cout << "Opening...\n";
				std::regex openBmpFileRegex("open\\s{1}([a-zA-Z-_0-9]{1,}.bmp)");
				std::smatch match;
				std::regex_match(userInput, match, openBmpFileRegex);
				if (match.empty())
				{
					throw std::exception("Wrong file format!");
				}
				else
				{
					bmpFileName = path.string()+match[1].str();
					goodFileFound = true;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "Could not open file: " << e.what() << "\n";
			}
		}

		else
		{
			std::regex changeDirectoryRegex("cd\\s{1}([a-zA-Z-_0-9]{1,})");
			std::smatch match;

			std::regex_match(userInput, match, changeDirectoryRegex);

			std::filesystem::path path2;
			std::string newPath;
			newPath = path.string() + match[1].str() + "\\";
			path2.assign(newPath);

			if (std::filesystem::exists(path2))
			{
				std::cout << "Going to: " << path2 << "\n";
				path = path2;
			}
			else
			{
				std::cout << "Path does not exist\n";
			}
			UpdateFileEntries();
		}
	}

	void GetMousePosition()
	{
		MousePosition = sf::Mouse::getPosition(*managerWindow);
	}

	void PollEvents()
	{
		sf::Event e{};
		bool backspacePressed{ false };
		bool enterPressed{ false };
		while (managerWindow->pollEvent(e))
		{

			switch (e.type)
			{
			case sf::Event::MouseButtonPressed:
			{

			}

			case::sf::Event::Closed:
			{
				CloseWindow();
				break;
			}

			case sf::Event::KeyPressed:
			{
				if (e.key.scancode == sf::Keyboard::Scan::Enter)
				{
					HandleUserInput();
					playerInput = "";
					playerEntry.SetEntry(playerInput);
					playerEntry.Draw(*managerWindow);
					enterPressed = true;
				}
				else if (e.key.scancode == sf::Keyboard::Scan::Backspace)
				{
					if (playerInput.length() > 0)
					{
						playerInput.pop_back();
						playerEntry.SetEntry(playerInput);
						backspacePressed = true;
					}
					else if (playerInput.length() == 0)
					{
						backspacePressed = true;
					}
				}
				else if (e.key.scancode == sf::Keyboard::Scan::Right)
				{
					std::cout << "Changing page\n";
					if (page != allPages)
					{
						page++;
					}
					UpdateFileEntries();
				}
				else if (e.key.scancode == sf::Keyboard::Scan::Left)
				{
					std::cout << "Changing page\n";
					if (page != 1)
					{
						page--;
					}
					UpdateFileEntries();
				}

				break;
			}

			case::sf::Event::TextEntered:
			{
				if (enterPressed)
				{
					playerEntry.SetEntry(playerInput);
					enterPressed = false;
				}
				else if (!backspacePressed)
				{
					playerInput += e.text.unicode;
					playerEntry.SetEntry(playerInput);
				}
				else
				{
					backspacePressed = false;
					enterPressed = false;
				}
				break;
			}

			case::sf::Event::Resized:
			{
				delete managerWindow;
				managerWindow = new sf::RenderWindow(sf::VideoMode(e.size.width, e.size.height), "Open file");
				break;
			}
			default:
				break;
			}
		}
	}

	void Update()
	{
		GetMousePosition();
		PollEvents();
	}

	std::string GetBmpFileName()
	{
		if (bmpFileName.length() != 0)
		{
			return bmpFileName;
		}
		else
		{
			return "";
		}
	}

	void Render()
	{
		managerWindow->clear();

		Draw();

		managerWindow->display();
	}

	~FileManager()
	{
		if (managerWindow->isOpen())
		{
			managerWindow->close();
		}
		delete managerWindow;
		delete testBtn;
	}
};