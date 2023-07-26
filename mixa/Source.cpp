#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <cstdlib> 
#include <ctime>
#include <iostream>
#include <vector>
#include "MainMenu.h"

using namespace sf;
using namespace std;


class Animation {
public:
    Animation() = default;
    Animation(int x, int y, int width, int height)
    {
        texture.loadFromFile("pic.png");
        for (int i = 0; i < nFrames; i++) {
            frames[i] = { x + i * width,y,width,height };
        }
    }
    void ApplyToSprite(Sprite& s) const {
        s.setTexture(texture);
        s.setTextureRect(frames[iFrame]);
    }
    void UpdateAnim(float deltaTime) {
        time += deltaTime;
        while (time >= holdTime) {
            time -= holdTime;
            Advance();
        }
    }
private:
    void Advance() {
        if (++iFrame >= nFrames) {
            iFrame = 0;
        }
    }

private:
    static constexpr int nFrames = 8;
    static constexpr float holdTime = 0.1f;
    Texture texture;
    IntRect frames[nFrames];
    int iFrame = 0;
    float time = 0.0f;
};
enum class EnemyColor {
    Red,
    Black,
    Yellow
};
class Enemy {
public:
    RectangleShape rectangle;
    float speed;
    EnemyColor color;
};
class Spy {
private:
    enum class AnimationIndex {
        WalkingUp,
        WalkingDown,
        WalkingLeft,
        WalkingRight,
        count
    };

public:
    Spy(const Vector2f& pos) : pos(pos)
    {
        sprite.setTextureRect({ 0,0,64,64 });
        animations[int(AnimationIndex::WalkingUp)] = Animation(64, 0, 64, 64);
        animations[int(AnimationIndex::WalkingDown)] = Animation(64, 128, 64, 64);
        animations[int(AnimationIndex::WalkingLeft)] = Animation(64, 64, 64, 64);
        animations[int(AnimationIndex::WalkingRight)] = Animation(64, 192, 64, 64);
    }
    bool IsColliding(const Enemy& enemy) const {
        FloatRect spyBounds = sprite.getGlobalBounds();
        FloatRect enemyBounds = enemy.rectangle.getGlobalBounds();
        return spyBounds.intersects(enemyBounds);
    }
    void Draw(RenderTarget& rt) const
    {
        rt.draw(sprite);
    }
    void SetDirection(const Vector2f& directory) {
        velocity = directory * speed;
        if (directory.x > 0.0f) {
            currentAnimation = AnimationIndex::WalkingRight;
        }
        else if (directory.x < 0.0f) {
            currentAnimation = AnimationIndex::WalkingLeft;
        }
        else if (directory.y < 0.0f) {
            currentAnimation = AnimationIndex::WalkingUp;
        }
        else if (directory.y > 0.0f) {
            currentAnimation = AnimationIndex::WalkingDown;
        }
    }
    void Update(float deltaTime) {
        Vector2f newpos = pos + velocity * deltaTime;

        if (newpos.x < 0.0f) {
            //left boundary
            newpos.x = 0.0f;
        }
        else if (newpos.x > 800.0f - 64.0f) {
            //right boundary
            newpos.x = 800.0f - 64.0f;
        }
        if (newpos.y < 0.0f) {
            //top boundary
            newpos.y = 0.0f;
        }
        else if (newpos.y > 600.0f - 64.0f) {
            //bottom boundary
            newpos.y = 600.0f - 64.0f;
        }

        pos = newpos;

        animations[int(currentAnimation)].UpdateAnim(deltaTime);
        animations[int(currentAnimation)].ApplyToSprite(sprite);
        sprite.setPosition(pos);
    }


private:
    static constexpr float speed = 150.0f;
    Vector2f pos;
    Vector2f velocity = { 0.0f,0.0f };
    Sprite sprite;

    Animation animations[int(AnimationIndex::count)];
    AnimationIndex currentAnimation = AnimationIndex::WalkingDown;
};
class Heart {
public:
    Heart(const Vector2f& pos) : pos(pos) {
        texture.loadFromFile("heart.png");
        sprite.setTexture(texture);
        sprite.setPosition(pos);
    }

    void Draw(RenderTarget& rt) const {
        rt.draw(sprite);
    }
private:
    Vector2f pos;
    Sprite sprite;
    Texture texture;
};
class Gamewin {
public:
    Gamewin() {
        if (!font2.loadFromFile("Vogue.ttf")) {
            throw runtime_error("Failed to load font");
        }
        text.setFont(font2);
        text.setString("Congratulation!");
        text.setCharacterSize(60);
        text.setFillColor(Color::Red);
        text.setPosition(300, 180);

        text2.setFont(font2);
        text2.setString("YOU SURVIVED THE NAZES!!!!");
        text2.setCharacterSize(60);
        text2.setFillColor(Color::Red);
        text2.setPosition(200, 240);
    }
    void Draw(RenderTarget& rt) const {
        rt.draw(text);
        rt.draw(text2);
    }
private:
    Font font2;
    Text text;
    Text text2;
};
class GameOverText {
public:
    GameOverText() {
        if (!font2.loadFromFile("Vogue.ttf")) {
            throw runtime_error("Failed to load font");
        }
        text.setFont(font2);
        text.setString("Game Over!");
        text.setCharacterSize(60);
        text.setFillColor(Color::Red);
        text.setPosition(350, 200);
    }

    void Draw(RenderTarget& rt) const {
        rt.draw(text);
    }

private:
    Font font2;
    Text text;
};
class Timer {
public:
    Timer() {
        clock.restart();
    }

    float getElapsedTime() const {
        return clock.getElapsedTime().asSeconds();
    }

    void restart() {
        clock.restart();
    }

private:
    Clock clock;
};
class About {
public:
    About() {
        if (!font.loadFromFile("Vogue.ttf")) {
            throw runtime_error("Failed to load font");
        }

        if (!heartTexture.loadFromFile("heart.png")) {
            throw runtime_error("Failed to load heart texture");
        }

        if (!spyTexture.loadFromFile("pic.png")) {
            throw std::runtime_error("Failed to load enemy texture");
        }

        heartSprite.setTexture(heartTexture);
        spySprite.setTexture(spyTexture);

        heartSprite.setPosition(20, 290);
        spySprite.setPosition(-500, 0);

        uptext.setFont(font);
        uptext.setString("press UP button on keybord");
        uptext.setCharacterSize(30);
        uptext.setFillColor(Color::Green);
        uptext.setPosition(70, 10);

        lefttext.setFont(font);
        lefttext.setString("press left button on keybord");
        lefttext.setCharacterSize(30);
        lefttext.setFillColor(Color::Blue);
        lefttext.setPosition(70, 80);

        downtext.setFont(font);
        downtext.setString("press down button on keybord");
        downtext.setCharacterSize(30);
        downtext.setFillColor(Color::Red);
        downtext.setPosition(70, 150);

        righttext.setFont(font);
        righttext.setString("press right button on keybord");
        righttext.setCharacterSize(30);
        righttext.setFillColor(Color::Yellow);
        righttext.setPosition(70, 220);

        enemies.setSize(Vector2f(36.0f, 36.0f));
        enemies.setFillColor(Color::Black);
        enemies.setPosition(20, 370);

        enemies1.setSize(Vector2f(36.0f, 36.0f));
        enemies1.setFillColor(Color::Red);
        enemies1.setPosition(20, 400);

        enemies2.setSize(Vector2f(36.0f, 36.0f));
        enemies2.setFillColor(Color::Yellow);
        enemies2.setPosition(20, 430);

        avoidenemy.setFont(font);
        avoidenemy.setString("Avoid enemies");
        avoidenemy.setCharacterSize(30);
        avoidenemy.setFillColor(Color::Black);
        avoidenemy.setPosition(60, 390);

        about.setFont(font);
        about.setString("It is World War 2, and a georgian soldier is trying to escape a German siege");
        about.setCharacterSize(30);
        about.setFillColor(Color::Red);
        about.setPosition(30, 460);

        about1.setFont(font);
        about1.setString("Help him to do so.");
        about1.setCharacterSize(30);
        about1.setFillColor(Color::Red);
        about1.setPosition(200, 500);
    }

    void Draw(RenderTarget& rt) const {
        rt.draw(text);
        rt.draw(uptext);
        rt.draw(downtext);
        rt.draw(righttext);
        rt.draw(lefttext);
        rt.draw(avoidenemy);
        rt.draw(heartSprite);
        rt.draw(spySprite);
        rt.draw(enemies);
        rt.draw(enemies1);
        rt.draw(enemies2);
        rt.draw(about);
        rt.draw(about1);
    }

private:
    Font font;
    Text text;
    Text uptext;
    Text downtext;
    Text righttext;
    Text lefttext;
    Text avoidenemy;
    Text about;
    Text about1;
    Texture heartTexture;
    Texture spyTexture;
    Sprite heartSprite;
    Sprite spySprite;
    RectangleShape enemies;
    RectangleShape enemies1;
    RectangleShape enemies2;

};

int main()
{
    RenderWindow window(VideoMode(800, 600), "SFML works!", Style::Default);
    MainMenu mainMenu(window.getSize().x, window.getSize().y);

    SoundBuffer buffer;
    if (!buffer.loadFromFile("georgia.ogg"))
        return -1;

    Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(8);

    sound.play();



    Texture mainTexture;
    if (!mainTexture.loadFromFile("mainmenu-photo.jpg")) {
        return -1;
    }
    Font font;
    if (!font.loadFromFile("Vogue.ttf")) {
        return -1;
    }

    RectangleShape background(Vector2f(800, 600));
    background.setTexture(&mainTexture);

    About about;

    bool isPlaying = false;
    bool isMenu = true;
    bool isAbout = false;
    bool isGameOver = false;
    bool isGameWin = false;
    Clock gameOverClock;
    Spy spy({ 0.0f,540.0f });

    vector<Enemy> enemies;
    srand(static_cast<unsigned>(time(0)));

    float interval = 1.0f;
    Clock timer;
    float dt = 0.0f;

    Heart heart1({ 10, 10 });
    Heart heart2({ 70, 10 });
    Heart heart3({ 130, 10 });
    vector<Heart> hearts = { heart1, heart2, heart3 };

    int heartsRemaining = 3;
    GameOverText gameOverText;

    Gamewin gamewin;

    Timer gameTimer;
    const float gameDuration = 80.0f;

    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();


            if (isMenu) {
                if (event.type == Event::KeyReleased) {
                    if (event.key.code == Keyboard::Up) {
                        mainMenu.MoveUp();
                    }
                    if (event.key.code == Keyboard::Down) {
                        mainMenu.MoveDown();
                    }
                    if (event.key.code == Keyboard::Return) {
                        int x = mainMenu.MainMenuPressed();
                        if (x == 0) {
                            isMenu = false;
                            isPlaying = true;
                        }
                        else if (x == 1) {
                            isMenu = false;
                            isAbout = true;
                        }
                        else if (x == 2) {
                            window.close();
                        }
                    }
                }
            }
            else if (isAbout) {
                if (event.type == Event::KeyReleased) {
                    if (event.key.code == Keyboard::A) {
                        isAbout = false;
                        isMenu = true;
                    }
                }
            }
        }
        float elapsedSeconds = gameTimer.getElapsedTime();
        if (elapsedSeconds >= gameDuration) {
            isPlaying = false;
            isGameWin = true;
            sound.stop();
        }
        window.clear();

        window.draw(background);

        if (isMenu) {
            mainMenu.draw(window);
        }
        else if (isPlaying) {
            if (timer.getElapsedTime().asSeconds() >= interval) {
                int minX = 800; // rightmost boundary
                int maxX = 800; // leftmost boundary
                int minY = 0;
                int maxY = 600 - 32; // adjust maxY to consider enemy size

                int randomY = rand() % (maxY - minY + 1) + minY;

                // createing new enemies
                Enemy newEnemy;
                newEnemy.rectangle.setSize(Vector2f(16.0f, 16.0f));
                newEnemy.rectangle.setFillColor(Color::Red);
                newEnemy.rectangle.setPosition(maxX, randomY);
                newEnemy.speed = 900.0f;

                int colorIndex = rand() % 3;
                if (colorIndex == 0)
                {
                    newEnemy.color = EnemyColor::Red;
                }
                else if (colorIndex == 1)
                {
                    newEnemy.color = EnemyColor::Black;
                }
                else
                {
                    newEnemy.color = EnemyColor::Yellow;
                }
                enemies.push_back(newEnemy);

                timer.restart();
            }
        }

        for (auto it = enemies.begin(); it != enemies.end(); )
        {
            RectangleShape& enemyRectangle = it->rectangle;
            if (it->color == EnemyColor::Red)
                enemyRectangle.setFillColor(Color::Red);
            else if (it->color == EnemyColor::Black)
                enemyRectangle.setFillColor(Color::Black);
            else if (it->color == EnemyColor::Yellow)
                enemyRectangle.setFillColor(Color::Yellow);

            enemyRectangle.move(-it->speed * dt, 0.0f); // move the enemy from right to left

            if (enemyRectangle.getPosition().x + enemyRectangle.getSize().x < 0.0f) {
                it = enemies.erase(it); // remove the enemy from the container
            }
            else {
                // collision detection
                if (spy.IsColliding(*it)) {
                    it = enemies.erase(it); // remove the enemy from the container
                    heartsRemaining--;
                    continue;
                }
                window.draw(enemyRectangle);
                it++;
            }
        }
        dt = 0.0f;
        {
            static auto previousTime = chrono::steady_clock::now();
            const auto currentTime = chrono::steady_clock::now();
            dt = chrono::duration<float>(currentTime - previousTime).count();
            previousTime = currentTime;
        }

        Vector2f dir = { 0.0f,0.0f };
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            dir.y -= 1.0f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            dir.y += 1.0f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            dir.x -= 1.0f;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            dir.x += 1.0f;
        }

        spy.SetDirection(dir);
        spy.Update(dt);

        if (heartsRemaining == 0) {
            isPlaying = false;
            isGameOver = true;
            sound.stop();
        }
        window.clear();

        window.draw(background);

        if (isGameOver) {
            gameOverText.Draw(window);
        }
        else if (isAbout) {
            about.Draw(window);
        }
        else if (isGameWin) {
            gamewin.Draw(window);
        }
        else if (isMenu) {
            mainMenu.draw(window);
        }
        else if (isPlaying) {
            spy.Draw(window);
            for (size_t i = 0; i < enemies.size(); i++) {
                window.draw(enemies[i].rectangle);
            }
            if (isPlaying) {
                for (int i = 0; i < heartsRemaining; i++) {
                    hearts[i].Draw(window);
                }
            }
            float remainingTime = gameDuration - elapsedSeconds;
            Text timeText;
            timeText.setFont(font);
            timeText.setString("Time: " + to_string(static_cast<int>(remainingTime)));
            timeText.setCharacterSize(40);
            timeText.setFillColor(Color::Red);
            timeText.setPosition(200, 10);
            window.draw(timeText);
        }
        window.display();

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
    }

    return 0;
}