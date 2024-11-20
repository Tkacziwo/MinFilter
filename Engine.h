#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Button.h"
#include <iostream>
#include "BmpFunctions.h"
#include <chrono>
#include <thread>
#include <windows.h>
#include <string>
#include "Filters.h"
#include "Filemanager.h"
#include "ImageEntry.h"
#include <fstream>
#include "ProgramStatus.h"
#include "CheckBox.h"
#include "ButtonArray.h"

class Engine
{
private:
	sf::RenderWindow* window;
	sf::Vector2i MousePosition;
	Button btn;
	Button btn2;
	FileManager* fileMng = nullptr;
	std::vector<Button> Buttons;
	std::string bmpFileName;
	int targetWidth = 600, targetHeight = 450;
	bool imagesPrepared = false;
	std::vector<sf::Texture> assetTextures;
	sf::Font font;
	ImageEntry* srcImg = nullptr;
	ImageEntry* fImg = nullptr; //filtered image
	ProgramStatus programStatus;
	int programStatusCode;
	std::vector<CheckBox> checkboxes;
	int checkboxChosenId = 100;
	int checkboxChosenAction;
	ButtonArray btnArray;
	int threadsNumber;

	void GetMousePosition()
	{
		MousePosition = sf::Mouse::getPosition(*window);
	}

	void PollEvents()
	{
		sf::Event e{};

		while (window->pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
			{
				window->close();
				break;
			}

			case sf::Event::Resized:
			{
				delete window;
				window = new sf::RenderWindow(sf::VideoMode(e.size.width, e.size.height), "Minimal Filter ver. 0.1");
				Buttons[0].SetPosition(sf::Vector2f(e.size.width - 350, 50));
				Buttons[1].SetPosition(sf::Vector2f(50, 50));
				break;
			}

			case sf::Event::MouseMoved:
			{
				for (int i = 0; i < Buttons.size(); i++)
				{
					Buttons[i].OnHover(Buttons[i].IsMouseInside(MousePosition));
				}
				for (int i = 0; i < checkboxes.size(); i++)
				{
					checkboxes[i].OnHover(checkboxes[i].IsMouseInside(MousePosition));
				}

				btnArray.OnHover(MousePosition);
				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				for (int i = 0; i < Buttons.size(); i++)
				{
					if (Buttons[i].IsMouseInside(MousePosition))
					{
						int actionId = Buttons[i].Action(MousePosition);
						switch (actionId)
						{
						case 0:
						{
							std::cout << "Invalid action id\n";
							break;
						}
						case 1:
						{
							if (bmpFileName == "")
							{
								std::cout << "File not chosen!\n";
							}
							else
							{
								std::cout << "Starting filter...\n";
								programStatus.HandleProgramStatusChange(30);
								Filter(bmpFileName, checkboxChosenAction, threadsNumber);
								programStatus.HandleProgramStatusChange(40);
								std::cout << "Filter finished!\n";
							}
							break;
						}
						case 2:
						{
							std::cout << "Opening file manager...\n";
							fileMng = new FileManager();
							if (fileMng->IsOpen())
							{
								std::cout << "Window already opened!\n";
							}
							else
							{
								std::cout << "Opening...\n";
								fileMng->OpenWindow();
								while (fileMng->IsOpen())
								{

									fileMng->Update();
									fileMng->Render();
									if (fileMng->goodFileFound)
									{
										bmpFileName = fileMng->GetBmpFileName();
										fileMng->CloseWindow();
										delete fImg;
										fImg = nullptr;
										programStatus.HandleProgramStatusChange(20);
										PrepareSrcImg();
									}
								}
							}
							delete fileMng;
							std::cout << "Finished\n";
							break;
						}

						default:
						{
							break;
						}
						}
					}
				}

				for (int i = 0; i < checkboxes.size(); i++)
				{
					if (checkboxes[i].IsMouseInside(MousePosition))
					{
						if (checkboxChosenId == 100)
						{
							checkboxes[i].OnClick();
							checkboxChosenId = i;
							if (checkboxes[i].GetState())
							{
								programStatus.HandleProgramStatusChange(checkboxes[i].GetAction());
								checkboxChosenAction = checkboxes[i].GetAction();
								break;
							}
							else
							{
								programStatus.RestoreOriginalText();
								checkboxChosenAction = 0;
								checkboxChosenId = 100;
							}
						}
						else if (checkboxChosenId == i)
						{
							checkboxes[i].OnClick();
							checkboxChosenId = i;
							if (checkboxes[i].GetState())
							{
								programStatus.HandleProgramStatusChange(checkboxes[i].GetAction());
								checkboxChosenAction = checkboxes[i].GetAction();
								break;
							}
							else
							{
								programStatus.RestoreOriginalText();
								checkboxChosenAction = 0;
								checkboxChosenId = 100;
							}
						}
					}
				}

				btnArray.OnClick(MousePosition);

				threadsNumber = btnArray.GetThreadsNumber();

				break;
			}
			default:
				break;
			}
		}
	}

	void PrepareSrcImg()
	{
		sf::Text text("Loading file...", font, 20);
		srcImg = new ImageEntry(sf::Vector2f(50, 350), text, targetWidth, targetHeight);
		srcImg->LoadImageFromFile(bmpFileName);
		srcImg->AfterLoading("Source image: ");
	}

	void PrepareImages(const std::string& resBmpName)
	{
		sf::Text text("Loading file...", font, 20);
		fImg = new ImageEntry(sf::Vector2f(800, 350), text, targetWidth, targetHeight);
		fImg->LoadImageFromFile(resBmpName);
		fImg->AfterLoading("Filtered image: ");

	}

	void LoadTextures()
	{
		sf::Image img;
		img.loadFromFile("Assets\\ButtonSquare.png");
		sf::Texture texture;
		texture.loadFromImage(img);
		assetTextures.push_back(texture);
		img.loadFromFile("Assets\\ButtonSquareHover.png");
		texture.loadFromImage(img);
		assetTextures.push_back(texture);
	}

public:
	Engine()
	{
		InitWindow();
		InitVariables();
	}

	void InitWindow()
	{
		window = new sf::RenderWindow(sf::VideoMode(1600, 900), "Minimal filter");
		window->setFramerateLimit(60);
	}

	void InitVariables()
	{
		font.loadFromFile("Arial.ttf");
		LoadTextures();
		auto windowSize = window->getSize();
		Button btn1(sf::Vector2f(windowSize.x - 350, 50), sf::Vector2f(300, 150), 1, assetTextures, 300, 150);
		btn1.SetText("Filter", font, 30);
		Buttons.push_back(btn1);
		Button btn2(sf::Vector2f(50, 50), sf::Vector2f(300, 150), 2, assetTextures, 300, 150);
		btn2.SetText("Choose file", font, 30);
		Buttons.push_back(btn2);
		programStatus = ProgramStatus(sf::Vector2f(50, 300), "Program status: ", font, 20);
		programStatusCode = 10; //file not selected yet
		programStatus.HandleProgramStatusChange(programStatusCode);


		CheckBox chk1(sf::Vector2f(400, 50), sf::Vector2f(50, 50), assetTextures, 50, 50, "Cpp lib");
		chk1.SetFont(font);
		chk1.SetAction(50);
		checkboxes.push_back(chk1);
		CheckBox chk2(sf::Vector2f(400, 150), sf::Vector2f(50, 50), assetTextures, 50, 50, "Asm lib");
		chk2.SetFont(font);
		chk2.SetAction(60);
		checkboxes.push_back(chk2);

		btnArray = ButtonArray(sf::Vector2f(700, 50), assetTextures);
	}

	void Update()
	{
		this->GetMousePosition();

		this->PollEvents();
	}

	void Render()
	{
		window->clear();
		btnArray.Draw(*window);

		for (auto& b : Buttons)
		{
			b.Draw(*window);
		}
		programStatus.Draw(*window);
		for (auto& c : checkboxes)
		{
			c.Draw(*window);
		}
		if (srcImg != nullptr && srcImg->ImageReady())
		{
			srcImg->Draw(*window);
		}
		if (fImg != nullptr && fImg->ImageReady())
		{
			fImg->Draw(*window);
		}
		window->display();
	}

	void Filter(const std::string& filename, int option, int threadsNr = 1)
	{
		int width{}, height{};
		unsigned char* bmp = bmpfun::LoadBMPFromFile(filename, width, height);
		std::string output;
		switch (option)
		{
		case 50:
		{
			try
			{
				std::chrono::milliseconds timeResult;
				if (threadsNr == 1)
				{
					filter::OneThreadC(height, width, bmp, timeResult, output);
				}
				else
				{
					filter::ThreadedC(threadsNr, height, width, bmp, timeResult, output);
				}
				std::cout << "Execution time: " << timeResult << "\n";
			}
			catch (const std::exception& e)
			{
				std::cout << "Something went wrong with Cpp filter: " << e.what() << "\n";
			}
			break;
		}
		case 60:
		{
			try
			{
				std::chrono::milliseconds timeResult;
				if (threadsNr == 1)
				{
					filter::OneThreadAssembly(height, width, bmp, timeResult, output);
				}
				else
				{
					filter::ThreadedAssembly(threadsNr, height, width, bmp, timeResult, output);
				}
				std::cout << "Execution time: " << timeResult << "\n";
			}
			catch (const std::exception& e)
			{
				std::cout << "Something went wrong with asm filter: " << e.what();
			}
			break;
		}
		default:
			break;
		}

		PrepareImages(output);

		delete[] bmp;
	}

	bool WindowOpen() const noexcept
	{
		if (window->isOpen())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	~Engine()
	{
		delete srcImg, fImg;
	}
};