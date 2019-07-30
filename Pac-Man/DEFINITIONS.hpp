#pragma once

#define WINDOW_TITLE "Pac-Man"

#define SCREEN_WIDTH 672
#define SCREEN_HEIGHT 864


#define TOP_ICON_FILEPATH "Resources/Images/topIcon.png"

#define MAP_FILEPATH "Resources/Images/Map.png"
#define MAP_WHITE_FILEPATH "Resources/Images/Map_white.png"
#define PACMAN_FILEPATH "Resources/Images/Pac-man.png"
#define POINT_FILEPATH "Resources/Images/Point.png"
#define BIG_POINT_FILEPATH "Resources/Images/Big_point.png"

#define GHOST_FILEPATH "Resources/Images/Ghost.png"

#define LOGO_FILEPATH "Resources/Images/Pacman_logo.png"

#define FONT_FILEPATH "Resources/Fonts/font.ttf"

#define INTRO_MUSIC_FILEPATH "Resources/Sounds/pacman_beginning.ogg"

#define HIGH_SCORE_FILEPATH "Resources/highScore.txt"

#define TILESIZE 24
#define MAP_RES_X 28
#define MAP_RES_Y 36

#define POINT_COUNT 240
#define BIGPOINT_COUNT 4


enum class Dir { Right, Left, Down, Up };

enum class Mode { Chase, Scatter, Flee };


#define PACMAN_START_POS sf::Vector2f(336.0f, 636.0f)
#define PACMAN_ANIMATION_TIME 0.2f

#define GHOST_RESET_POSITION sf::Vector2i(14, 14)
#define GHOST_ANIMATION_TIME 0.3f


#define PACMAN_01 sf::IntRect(33, 0, 32, 32)
#define PACMAN_02 sf::IntRect(0, 0, 32, 32)
#define PACMAN_03 sf::IntRect(66, 0, 32, 32)

#define BLINKY_RIGHT_01 sf::IntRect(0, 0, 32, 32)
#define BLINKY_RIGHT_02 sf::IntRect(37, 0, 32, 32)
#define BLINKY_LEFT_01 sf::IntRect(73, 0, 32, 32)
#define BLINKY_LEFT_02 sf::IntRect(110, 0, 32, 32)
#define BLINKY_UP_01 sf::IntRect(146, 0, 32, 32)
#define BLINKY_UP_02 sf::IntRect(183, 0, 32, 32)
#define BLINKY_DOWN_01 sf::IntRect(219, 0, 32, 32)
#define BLINKY_DOWN_02 sf::IntRect(256, 0, 32, 32)

#define PINKY_RIGHT_01 sf::IntRect(0, 34, 32, 32)
#define PINKY_RIGHT_02 sf::IntRect(37, 34, 32, 32)
#define PINKY_LEFT_01 sf::IntRect(73, 34, 32, 32)
#define PINKY_LEFT_02 sf::IntRect(110, 34, 32, 32)
#define PINKY_UP_01 sf::IntRect(146, 34, 32, 32)
#define PINKY_UP_02 sf::IntRect(183, 34, 32, 32)
#define PINKY_DOWN_01 sf::IntRect(219, 34, 32, 32)
#define PINKY_DOWN_02 sf::IntRect(256, 34, 32, 32)

#define INKY_RIGHT_01 sf::IntRect(0, 68, 32, 32)
#define INKY_RIGHT_02 sf::IntRect(37, 68, 32, 32)
#define INKY_LEFT_01 sf::IntRect(73, 68, 32, 32)
#define INKY_LEFT_02 sf::IntRect(110, 68, 32, 32)
#define INKY_UP_01 sf::IntRect(146, 68, 32, 32)
#define INKY_UP_02 sf::IntRect(183, 68, 32, 32)
#define INKY_DOWN_01 sf::IntRect(219, 68, 32, 32)
#define INKY_DOWN_02 sf::IntRect(256, 68, 32, 32)

#define CLYDE_RIGHT_01 sf::IntRect(0, 102, 32, 32)
#define CLYDE_RIGHT_02 sf::IntRect(37, 102, 32, 32)
#define CLYDE_LEFT_01 sf::IntRect(73, 102, 32, 32)
#define CLYDE_LEFT_02 sf::IntRect(110, 102, 32, 32)
#define CLYDE_UP_01 sf::IntRect(146, 102, 32, 32)
#define CLYDE_UP_02 sf::IntRect(183, 102, 32, 32)
#define CLYDE_DOWN_01 sf::IntRect(219, 102, 32, 32)
#define CLYDE_DOWN_02 sf::IntRect(256, 102, 32, 32)

#define GHOST_FLEE_BLUE_01 sf::IntRect(0, 136, 32, 32)
#define GHOST_FLEE_BLUE_02 sf::IntRect(37, 136, 32, 32)
#define GHOST_FLEE_WHITE_01 sf::IntRect(73, 136, 32, 32)
#define GHOST_FLEE_WHITE_02 sf::IntRect(110, 136, 32, 32)

#define GHOST_EYES_RIGHT sf::IntRect(146, 136, 32, 32)
#define GHOST_EYES_LEFT sf::IntRect(183, 136, 32, 32)
#define GHOST_EYES_UP sf::IntRect(219, 136, 32, 32)
#define GHOST_EYES_DOWN sf::IntRect(256, 136, 32, 32)