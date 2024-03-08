#include <iostream>
#include <SFML/Graphics.hpp>
#include "windows.h"
using namespace std;

const int LY = 23; // ������ ��������� �� ���������
const int LX = 28; // ������ ��������� �� �����������
const int sprSize = 24; // ������ ������� � ��������
const int dashboardSprSize = 48; // ������ �������� �������������� ���������
const int spritesCount = 9; //���������� ��������

int maze[LY][LX] = { // ��� ��� ��������, ��������� �� ��
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,1,1},
    {1,1,6,1,0,0,0,0,4,1,1,1,1,0,5,0,0,0,0,0,0,0,0,1,1,0,1,1},
    {1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1},
    {1,1,0,0,0,1,1,4,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1},
    {1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4,1,1,1,0,0,5,1,4,0,2},
    {1,1,4,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,7,0,0,0,0,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0,0,4,8,4,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0,1,1,1,1,1,1,6,1,1,4,0,0,0,0,0,4,0,0,1,1},
    {1,1,5,0,1,1,1,0,1,1,1,1,5,0,0,1,1,1,1,1,1,0,1,1,1,0,1,1},
    {1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,8,1,1,1,5,1,1},
    {1,1,1,4,0,0,7,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,1,4,0,0,0,0,0,0,0,0,0,0,4,1,1,1},
    {1,1,1,0,0,0,0,4,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,1,0,4,1,1,1,1,1,1,1,1,1,0,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,0,8,0,0,0,1,1,1,6,0,0,0,5,1,0,1,1,1},
    {1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1},
    {1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,7,1,1,1,0,1,1,1},
    {1,1,1,1,0,0,5,1,1,1,0,1,1,1,1,1,4,0,0,0,0,0,0,4,0,1,1,1},
    {1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1},
    {2,0,0,0,4,1,1,1,1,1,4,0,0,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// ���������� �� ���������� ������ ����
int score = 0; // ���� ����

int gameState = 0; // 0 - ���� ������������, 1 - �������, 2 - ����������� �����
sf::Time timeLimit = sf::milliseconds(90000); // ����� ���� � �������������
sf::Time gameTime; // ���������� �����
sf::Time elapsedTime; // ������� ���������� �������
sf::Clock gameClock; // ������
sf::Time addTime = sf::milliseconds(0); // �������������� �����

sf::Vector2f scoreTextPosition, timeTextPosition; // ������� ������ ��� ����� � ������ ��� �������

struct PlayerPosition // ���������, � ������� ������ ������� ������
{
    int x, y; // ���������� � � � ������
};

PlayerPosition playerPos; // ��������� ����������, ������� ����� ������� ������� ������

// ��������� ����, � ������� ������������ ����. ������ ������, ������� ������� ������� � ��������� ����
sf::RenderWindow window(sf::VideoMode(800, 688), "SFML maze game");

// ������� ����
sf::Texture textures[spritesCount];    // ��� ������ ��� �������� �������
sf::Sprite sprites[spritesCount];      // ��� ������ ��� ��������, ��� ���� ������� ������� ������� � ������� ������������� ��������� � ������� maze

std::string textureNames[spritesCount]{  // ��������� ������ ����� ��� �������� ���� ������, ������� ������ ��������
    "assets\\bitmaps\\empty.bmp",
    "assets\\bitmaps\\wall.bmp",
    "assets\\bitmaps\\door.bmp",
    "assets\\bitmaps\\man.png",
    "assets\\bitmaps\\money.png",
    "assets\\bitmaps\\diamond.png",
    "assets\\bitmaps\\time.png",
    "assets\\bitmaps\\mine.png",
    "assets\\bitmaps\\mine.png"
};

// ������� �������� �����������
sf::Texture backgroundTexture;
sf::Sprite backgroundSprite;
string backgroundImage = "assets\\background\\background.jpg";

// ������ "����"
sf::Texture scoreTexture;
sf::Sprite scoreSprite;
string scoreImage = "assets\\bitmaps\\score.png";

// ������ "������"
sf::Texture timerTexture;
sf::Sprite timerSprite;
string timerImage = "assets\\bitmaps\\timer.png";

// ����� �������� ����
sf::Text headerText;
sf::Font headerFont;
string headerTextFile = "assets\\fonts\\Novartis-Deco.ttf";

// ����� ����������� ����
sf::Text dashboardText;
sf::Font dashboardFont;
string dashboardTextFile = "assets\\fonts\\Grotesque-Bourgeoisie.ttf";

bool FindPlayerStartPosition(int maze[LY][LX], int width, int height)
{
    int countPlayer = 0; // ���������� �������
    int countExit = 0;   // ���������� �������

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            switch (maze[j][i])
            {
            case 2:                 // ���������, ���� ������� ������� ����� 2 (�����)
                countExit++;        // ��������� �����
                break;
            case 3:                 // ���������, ���� ������� ������� ����� 3 (�����) - ����� ��������� �������
                // ���������� ���������� ������
                playerPos.x = i;    
                playerPos.y = j;    
                countPlayer++;      // ��������� ������
                break;
            }
        }
    }

    if (countPlayer == 1 && countExit > 0) {    // ���� ����� � ������� ���� �� ���� �����
        return 1;   // �������� ��������
    }
    else return 0;  // �������� �� ��������
}

void UpdateScore(int score)
{
    dashboardText.setString(to_string(score)); // ������������� ����� ��� ������ to_string ��������� ����� � ��������� �������������
    dashboardText.setPosition(scoreTextPosition); // ������������� ������� ������ ��� �����
    window.draw(dashboardText); // ������������ ����� � ������ �����
}

// ������� ��������� ���������� ����� �� ��������� ����
void UpdateClock(sf::Time elapsed)
{
    gameTime = timeLimit - elapsed + addTime; /* ��������� ���������� ����� � ��������
                    timeLimit - ����� �������, elapsed - ��������� ����� � ������� ������ ����, addTime - ��������� �������������� �����
                    */
    if (gameTime.asSeconds() < 0) // ���������, ����������� �� �����
        // gameTime.asSeconds() - ���������� ����� �� ������� Time � �������
    {
        gameState = 2; // ���� ��, �� ��������� ������ ���� �� 2 - ����� ��������
    }
    else
    {   // ���� ����� ��������
        dashboardText.setPosition(timeTextPosition); // ������������� ������� ������ ��� �����
        // ������������� ����� ��� ������ to_string ��������� ����� � ��������� �������������
        // ��� ��������� ������� � �������� ���������� ������� asSeconds. ��� ���������� float
        // ������� ���� ����������� �� � int, ����� �������� ���������� ����� ��� ������ ����������� �������
        dashboardText.setString(to_string((int)gameTime.asSeconds()));
        window.draw(dashboardText); // ������������ ����� � ������ �����
    }
}

// ������� ������ ��������� � ��������� ��� ������
void ShowMessageWindow(string text,                                         // ������ ���������, ������������� � ����
                       sf::Color backgroundColor = sf::Color::Blue,         // ���� ���� ����
                       sf::Color textColor = sf::Color::White,              // ���� ������ ���������� ���������
                       sf::RectangleShape windowSize = sf::RectangleShape(sf::Vector2f(600.f, 300.f)), // ������� ����
                       sf::Color frameColor = sf::Color::White)             // ���� ����� ����
{
    sf::Vector2u wSize = window.getSize();          // �������� ������� ���� ����   
    sf::Vector2f mesSize = windowSize.getSize();    // �������� ������� ���� ���������

    // ���������, �� ��������� �� ������� ���� ��������� ������� ���� ����
    if (mesSize.x > wSize.x) mesSize.x = wSize.x - 10;
    if (mesSize.y > wSize.y) mesSize.y = wSize.y - 10;
    windowSize.setSize(mesSize); // ������������� ����������������� ������� ���� ���������

    // ��������� ���
    windowSize.setFillColor(backgroundColor);   // ������������� ���� ����    
    windowSize.setPosition((wSize.x - mesSize.x) / 2, (wSize.y - mesSize.y) / 2); // ���������� ����
    windowSize.setOutlineThickness(10.f);       // ������������� ������� �������
    windowSize.setOutlineColor(frameColor);     // ������������� ���� �������

    // ��������� ������ ���������
    dashboardText.setString(text);         // ������ ������ ������ ��� �����������
    dashboardText.setFillColor(textColor); // ������������� ���� 
    sf::FloatRect textBounds = dashboardText.getLocalBounds(); // �������� ������� ��������� ������ ���������
    dashboardText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    dashboardText.setPosition(sf::Vector2f(wSize.x / 2, wSize.y / 2));  // ������������� ���������

    window.draw(windowSize);
    window.draw(dashboardText);
}

//������� ���������� �������� �������
void PrepareBackgroundImage(string imageFileName)
{
    backgroundTexture.loadFromFile(imageFileName); // ��������� ������� ��������
    backgroundSprite.setTexture(backgroundTexture); // ���������� �� �� ������
    backgroundSprite.setColor(sf::Color(255, 255, 255, 100)); // ������������� ������������ �������� �������
}

//������� �������� �������� ����������� ����
void PrepareDashboardImage(string scoreImageFileName, string timerImageFileName)
{
    scoreTexture.loadFromFile(scoreImageFileName); // ��������� ��������
    scoreSprite.setTexture(scoreTexture); // ���������� �� �� ������

    timerTexture.loadFromFile(timerImageFileName); // ��������� ��������
    timerSprite.setTexture(timerTexture); // ���������� �� �� ������

    // �������������. �������� ������� ���� ����
    sf::Vector2u wSize = window.getSize();
    // ��������� ���������� ������������ ������
    unsigned int scoreXPos = wSize.x / 2 - 8 * sprSize;
    unsigned int scoreYPos = wSize.y - dashboardSprSize - 10; // 48 ������ ������� ������, 10 - ������ ����� ����
    unsigned int timerXPos = wSize.x / 2 + 3 * sprSize;
    // ������������� ������� ��������
    scoreSprite.setPosition(sf::Vector2f(scoreXPos, scoreYPos));
    timerSprite.setPosition(sf::Vector2f(timerXPos, scoreYPos));
    // ��������� ������� ������ ��� ����� ���� = ��������_������� + ������ ������� + ������
    scoreTextPosition.x = scoreXPos + dashboardSprSize + 10;
    scoreTextPosition.y = scoreYPos - 15;
    // ��������� ������� ������ ��� ������� ���� = ��������_������� + ������ ������� + ������
    timeTextPosition.x = timerXPos + dashboardSprSize + 10;
    timeTextPosition.y = scoreYPos - 15;
}

// ������� �������� �������
void PrepareFonts(string headerFontName, string dashboardFontName)
{
    // ��������� ��������� ����
    headerFont.loadFromFile(headerFontName); // ��������� �����
    headerText.setFont(headerFont); // ������������� ����� ��� ������
    headerText.setCharacterSize(45); // ������������� ������ ��������
    headerText.setString("MAZE GAME");  // ������ ������ ������ ��� �����������
    headerText.setFillColor(sf::Color(255, 0, 0, 200)); // ������������� ���� ������� � ������������
    headerText.setStyle(sf::Text::Bold);  // ������ ����� ������
    // ��������� ������ ����������� ����
    dashboardFont.loadFromFile(dashboardFontName); // ��������� �����
    dashboardText.setFont(dashboardFont);// ������������� ����� ��� ������
    dashboardText.setCharacterSize(45);// ������������� ������ ��������
    dashboardText.setString("0");// ������ ������ ������ ��� �����������
    dashboardText.setFillColor(sf::Color(255, 255, 35)); // ������������� ������-������ ���� ������
    dashboardText.setStyle(sf::Text::Bold | sf::Text::Italic);// ������ ����� ������ � ��������, ��������� ������� �
    // ������������� ���������. �������� ������� ���� ����
    sf::Vector2u wSize = window.getSize();
    // �������� ������� ��������� ������ ���������. textBounds.width - ������ ������ � ��������
    sf::FloatRect textBounds = headerText.getLocalBounds();
    float x = wSize.x / 2 - textBounds.width / 2;
    headerText.setPosition(sf::Vector2f(x, 5));  // ������������� ���������
}

// �������, ���������������� ����� ���������. ������������ ������ PrintMaze
void RedrawMaze(int maze[LY][LX], int width, int height)
{
    // ������� "����������" �������� �� ������� ����, ��� ����� ����� ��������� ����������
    // �������� ������ ���� ���������. ��� ����� ������� �������� ������� ���� ����
    sf::Vector2u wSize = window.getSize();
    // ��� ����� ������� � �������� ������ ���� ��������� ������ ����?
    // ���� ������ ���� �������� �� 2 � ������� �� ����������� �������� �������� ������ ���������
    unsigned int xMaze = wSize.x / 2 - LX * sprSize / 2;
    // ���������� ��������� � �
    unsigned int yMaze = wSize.y / 2 - LY * sprSize / 2;
    // ����� ���������� ��� ���������� � �������� ������� ����� ��� ������ ���������

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            // �������� �� ������� ��������� ��� ��������������� �������
            int index = maze[j][i];
            // ������ ������� ������� � ����. ������� ����� �� � - ������ �������� �������
            // ����������� �� ������ ������� �� �����������. �� � - ������ ������, �����������
            // �� ������ ������� �� ���������
            sprites[index].setPosition(sf::Vector2f(xMaze + i * sprSize, yMaze + j * sprSize));
            // ������������ ������
            if (index > 0) window.draw(sprites[index]);
        }
    }
}

// �������� ������� � ������������� ��������
void PrepareSpriteTexture(std::string names[spritesCount], int count = spritesCount)
{
    for (int i = 0; i < count; i++) // ���� �������� ������� � ������������� ��������
    {
        textures[i].loadFromFile(names[i]); // ������ �������� �� �����
        textures[i].setSmooth(true); // ��������� ���������� �������� ��� ���������� ������� ������������
        textures[i].setRepeated(false); // ��������� ����������� ���������� ������� ���������
        sprites[i].setTexture(textures[i]); // ���������� �������� �� ������
    }
}

// ���������� ������ �� ������� ������� �� dx ������ �� �����������
// � dy ������ �� ���������
void Move(int dx, int dy)
{
    if (playerPos.y + dy >= 0 && playerPos.y + dy < LY && // ��������� �� ����� �� ������� �����
        playerPos.x + dx >= 0 && playerPos.x + dx < LX)
    {
        if (maze[playerPos.y + dy][playerPos.x + dx] != 1) // ���������, ���� �� � ������� �����������
        {                                                  // ������, � ���� ��� - ������������
            maze[playerPos.y][playerPos.x] = 0; // ������� ������ �� ��������� � ������� �������
            playerPos.x += dx; // �������� ����������
            playerPos.y += dy; // ������
            switch (maze[playerPos.y][playerPos.x]) //��������� ���������� ������ ���������, ������� ������ �����
            {
            case 2: // ���� ����� - ���� 500 ������� � ������ ������ ���� �� �������
                score += 500; // ����������� ������
                gameState = 1;
                break;
            case 4: // ���� ������ - ��������� 100 �������
                score += 100; // ����������� ������
                break;
            case 5: // ���� ����� - ��������� 300 �������
                score += 300; // ����������� ������
                break;
            case 6: // ���� ����� - ��������� 10 ������
                addTime += sf::milliseconds(10000); // ����������� ���������� �����
                break;
            case 7: // ���� ���� - �������� 200 �������
                score -= 200; // ��������� ������
                break;
            case 8: // ���� ����2 - �������� 8 ������
                addTime -= sf::milliseconds(8000); // ��������� ���������� �����
                break;
            }
            maze[playerPos.y][playerPos.x] = 3; // ���������� ������ � ����� ������� ���������
        }
    }
}

void HandleKeyboardEvents() // ������������ ������� ����������
{
    // ������� isKeyPressed ������������ ��� �������� - ���� �� ������ ������������ �������.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // ������ ������� �����?
    {
        Move(-1, 0); // ���� �����
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))// ������ ������� ������?
    {
        Move(1, 0);// ���� ������
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  // �����
    {
        Move(0, -1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // ����
    {
        Move(0, 1);
    }
}

void RenderScene()
{
    window.draw(backgroundSprite);  // ������������ ������ � ������� ������������ � ������ �����
    RedrawMaze(maze, LX, LY); // ��������� ���������
    window.draw(headerText); // ������� ��������� ����
    window.draw(scoreSprite); // ������� ������ �����
    window.draw(timerSprite); // ������� ������ �������
    UpdateScore(score); // ��������� ����� �����
    UpdateClock(gameClock.getElapsedTime()); // ��������� ���������� ����� ����
}

int main()
{
    PrepareSpriteTexture(textureNames, spritesCount); // �������������� ������� ���������
    PrepareBackgroundImage(backgroundImage); // ������� ������ ����
    PrepareDashboardImage(scoreImage, timerImage); // ������� ������� ����������� ����
    PrepareFonts(headerTextFile, dashboardTextFile); // ������� ����� ��� ������ ������� � �����
    gameClock.restart(); // ��������� ������ � ����

    if (!FindPlayerStartPosition(maze, LX, LY)) {    // ���� �������� �� �������� ����������� ����   
        RenderScene(); // ��������� (��������) �����     
        ShowMessageWindow("Maze is not valid", sf::Color(0, 100, 0), sf::Color::White); // �������� � �������������� ���������
        window.display();  // ��������� ������ ������ � ����
        Sleep(5000); // ���� ����� ������� ��������� ����� ���������
        return 1;
    }

    while (window.isOpen()) // ���� ��������� �������, ���� ���� ��������� �� �������
    {
        sf::Event event; // ��������� ������ �������
        while (window.pollEvent(event)) // �������� ������� �� �������
        {
            if (event.type == sf::Event::Closed) // ���������, ���� ��� �������� ����, �� ��������� ����
                window.close();
        }

        window.clear(); // ������� ����
        HandleKeyboardEvents(); // ��������� ������� ����������
        RenderScene(); // ��������� (��������) �����        
        if (gameState != 0) // ���������, �� ����������� �� ����
        {
            // �������� � ����� ���� � ��������� ����
            if (gameState == 1) {
                ShowMessageWindow("Winner !!!", sf::Color::Blue, sf::Color::White);
            }
            else ShowMessageWindow("Time is over.\nYou lost !!!", sf::Color(139, 0, 0), sf::Color::Yellow);

            window.display();  // ��������� ������ ������ � ����
            Sleep(5000); // ���� ����� ������� ��������� ����� ���������
            break; // ����� �� �����, ���������� ����
        }
        window.display();  // ��������� ������ ������ � ����
        Sleep(100); // ����� 100��
    }
}