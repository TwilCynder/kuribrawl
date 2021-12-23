#pragma once
#include <memory>

class Game;

class GameManager : public std::unique_ptr<Game> {
    public:
    GameManager();
    void start();
    void stop();
};