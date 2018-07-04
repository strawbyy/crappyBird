#include <iostream> // displaying everything
#include <conio.h> // for getch and kbhit
#include <ctime> // for randomeness 
#include <cstdlib> // for screen refresh - cls/
#include <windows.h> // sleep()
#include <vector> // enemy vector

void menu();
void space();
bool gameOver;
char map[122][25];
int mapWidth = 122;
int mapHeight = 25;
int score = -1;

class bird {
public:
	void jump() {
		birdY -= 4;
		birdX+=2;
		tailShape = '-';
		bodyShape = 'O';
	}
	void place() {
		int birdX = mapWidth - (mapWidth * 0.85);
		int birdY = mapHeight - (mapHeight * 0.4);
	}
	int birdX = mapWidth - (mapWidth * 0.85);
	int birdY = mapHeight - (mapHeight * 0.4);
	bool fell = false;
	const char birdShape = '<';
	char tailShape = '_';
	char bodyShape = 'o'; 
	// _o<   _o<     _o<  _o<      _o<  _o< _o< _o<   _o< _o<   _o<   _o<     _o<  
};

class enemyBird {
public:
	bool exists = false;
	int speed = 7;
	void place() {
		evilBirdX = mapWidth - 4;
		evilBirdY = ((mapHeight-5) - (mapHeight * static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1))-1);
		exists = true;
	}
	void move() {
		//evilBirdX -= speed; Difficulty settings broke the game - Fix this later
		evilBirdX-=(rand()%4);
	}
	const char birdShape = 'X';
	int evilBirdX;
	int evilBirdY;
};

std::vector<enemyBird> enemies;
bird flappyBird;

void initMap(){
	for (int mapY = 0; mapY < 25; mapY++) {
		for (int mapX = 0; mapX< 120; mapX++) {
			if (mapY == 0 || mapY == 24 || mapX == 0) {
				map[mapX][mapY] = '@';
			}
			else if (mapX == 119) {
				map[mapX][mapY] = '|';
			}
			else {
				map[mapX][mapY] = ' ';
			}
		}
	}
}

void draw() {
	system("cls");
	for (int mapY = 0; mapY < 25; mapY++) {
		for (int mapX = 0; mapX < 120; mapX++) {
			if (flappyBird.birdX == mapX && flappyBird.birdY == mapY) {
				map[mapX][mapY] = flappyBird.tailShape;
				map[mapX + 1][mapY] = flappyBird.bodyShape;
				map[mapX + 2][mapY] = flappyBird.birdShape;
			}
			for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
					if (enemies[enemyNumber].evilBirdX == mapX && enemies[enemyNumber].evilBirdY == mapY && enemyNumber < enemies.size()) {
						if (mapX < 120 && mapY < 25) {
							map[mapX][mapY] = enemies[enemyNumber].birdShape;
						}
					}
			}
			std::cout << map[mapX][mapY];
		}
		std::cout << std::endl;
	}
	std::cout << "score: " << score;
	/*debug
	for (int i = 0; i < enemies.size(); i++) {
		std::cout << "\n(" << enemies[i].evilBirdX << ", " << enemies[i].evilBirdY << ") - ";
	}*/

}

void input() {
	if (_kbhit()) { // if a key is hit
		if (_getch() == ' ') { // check what it is, and if it's a space
			flappyBird.jump(); // jump
		}
	}
}

void spawnEnemies() {
	//srand(time(0));
	for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
		if (!enemies[enemyNumber].exists) {
			enemies[enemyNumber].place();
		}
	}
}

void moveEnemies(enemyBird& enemyObj) {
	if (enemyObj.exists) // this was a workaround because enemies spawned in the wrong places
		enemyObj.move();
}

void falling() {
	if (!flappyBird.fell) { // this is so that it does not fall down too quickly, alternates between falling and staying up
		flappyBird.birdY+=2;
		flappyBird.birdX++;
		flappyBird.tailShape = '_';
		flappyBird.bodyShape = 'o';
		flappyBird.fell = true;
	}
	else {
		flappyBird.fell = false;
	}
}

void endOrRestartGame(bird& mainBird = flappyBird) {
	if (mainBird.birdX >= mapWidth-1) {
		mainBird.birdX = mapWidth - (mapWidth * 0.90);
		mainBird.birdY = mapHeight - (mapHeight * 0.4);
		for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
			enemies[enemyNumber].place();
		}
		score++;
	}
	if (mainBird.birdY >= mapHeight || mainBird.birdY <= 1) {
		gameOver = true;
	}
	for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
		if (mainBird.birdX == enemies[enemyNumber].evilBirdX && mainBird.birdY == enemies[enemyNumber].evilBirdY
			|| mainBird.birdX-1 == enemies[enemyNumber].evilBirdX && mainBird.birdY == enemies[enemyNumber].evilBirdY 
			|| mainBird.birdX-2 == enemies[enemyNumber].evilBirdX && mainBird.birdY == enemies[enemyNumber].evilBirdY) {
			gameOver = true; //yes, the list of conditions is messy - put it in a bool function later.
		}
	}
}
void killEnemies() {
	for(int i = 0; i < enemies.size(); i++)
	if (enemies[i].evilBirdX <= 1) {
   		enemies[i].exists = false; // building off the spawn workaround to kill them ^__^
	}
}

void update() {
	falling();
	for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
		spawnEnemies();
		moveEnemies(enemies[enemyNumber]);
		killEnemies();
		endOrRestartGame();
	}
}

void gameFunction() {
	while (!gameOver) {
		initMap();
		input();
		update();
		draw();
		Sleep(4);
		if (gameOver) {
			system("cls");
			space();
			std::cout << "GAMEOVER\n";
			menu();
		}
	}

}

void difficulty() {
	std::cout << "Choose 1 for easy or 2 for hard" << std::endl;
	int diff;
	std::cin >> diff;
	if (diff == 1) {
		for (int enemy_Number = 0; enemy_Number < enemies.size(); enemy_Number++) {
			enemies[enemy_Number].speed = 7;
			menu();
		}
	}
	else if (diff == 2) {
		for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
			enemies[enemyNumber].speed = 10;
			menu();
		}
	}
	else {
		std::cout << "Invalid choice, input again: ";
		std::cin >> diff;
		difficulty();
	}
}

void space() {
	for (int i = 0; i < (mapWidth / 2)-3; i++) {
		std::cout << " ";
	}
}

void menu() {
	enemies.resize(5);
	space();
	std::cout << "MAIN MENU\n\n\n";
	space();
	std::cout << "1:Play Game\n\n";
	space();
	std::cout << "2:Difficulty\n\n";
	space();
	std::cout << "3:Exit\n\n" << std::endl;
	space();
	std::cout << "Your option : ";
	int choice;
	std::cin >> choice;
	switch (choice) {
	case 1:
		gameOver = false;
		flappyBird.birdX = 300;
		endOrRestartGame();
		gameFunction();
		break;
	case 2:
		difficulty();
		break;
	default:
		exit(0);
	}
}

int main(void) {
	menu();
	std::cin.get();
	return 0;
}
