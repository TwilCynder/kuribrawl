#pragma once
#include <memory>

class Game;
class GameConfiguration;

class GameManager : public std::unique_ptr<Game> {
    public:
    GameManager();
    void destroy();
    Game& start();
    Game& start(GameConfiguration&);
    Game& start(std::shared_ptr<GameConfiguration>&);
    void stop();
    void restart();

    private:
    using std::unique_ptr<Game>::reset;
    using std::unique_ptr<Game>::release;
    using std::unique_ptr<Game>::swap;
};