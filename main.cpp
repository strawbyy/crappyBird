#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <vector>

void menu();
bool gameOver;
char map[120][25];
int mapWidth = 120;
int mapHeight = 25;
int score;

class bird {
public:
	void jump() {
		birdY -= 4;
		birdX+=2;
		tailShape = '-';
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
};

class enemyBird {
public:
	bool exists = false;
	int speed = 7;
	void place() {
		evilBirdX = mapWidth - 4;
		evilBirdY = (mapHeight - (mapHeight * static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1))-1);
		exists = true;
	}
	void move() {
		evilBirdX -= speed;
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
				map[mapX + 1][mapY] = 'o';
				map[mapX + 2][mapY] = flappyBird.birdShape;
			}
			for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
				if (enemies[enemyNumber].evilBirdX == mapX && enemies[enemyNumber].evilBirdY == mapY) {
					map[mapX][mapY] = enemies[enemyNumber].birdShape;
				}
			}
			std::cout << map[mapX][mapY];
		}
		std::cout << std::endl;
	}
	std::cout << "score: " << score;
	/*debug
	for (int i = 0; i < enemies.size(); i++) {
		std::cout << "(" << enemies[i].evilBirdX << ", " << enemies[i].evilBirdY << ") - ";
	}*/

}

void input() {
	if (_kbhit()) {
		if (_getch() == ' ') {
			flappyBird.jump();
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
	if (enemyObj.exists)
		enemyObj.move();
}

void falling() {
	if (!flappyBird.fell) {
		flappyBird.birdY+=2;
		flappyBird.birdX++;
		flappyBird.tailShape = '_';
		flappyBird.fell = true;
	}
	else {
		flappyBird.fell = false;
	}
}

void endOrRestartGame(bird& mainBird = flappyBird) {
	if (mainBird.birdX >= mapWidth) {
		mainBird.birdX = mapWidth - (mapWidth * 0.90);
		mainBird.birdY = mapHeight - (mapHeight * 0.4);
		score++;
	}
	if (mainBird.birdY == mapHeight || mainBird.birdY == 0) {
		gameOver = true;
	}
	for (int enemyNumber = 0; enemyNumber < enemies.size(); enemyNumber++) {
		if (mainBird.birdX == enemies[enemyNumber].evilBirdX && mainBird.birdY == enemies[enemyNumber].evilBirdY) {
			gameOver = true;
		}
	}
}
void killEnemies() {
	for(int i = 0; i < enemies.size(); i++)
	if (enemies[i].evilBirdX <= 1) {
   		enemies[i].exists = false;
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
		Sleep(20);
		if (gameOver) {
			system("cls");
			std::cout << "game over :(";
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

void menu() {
	enemies.resize(5);
	std::cout << "\nMain Menu\n\n\n1:Play Game\n\n2:Difficulty\n\n3:Exit\n\n" << std::endl << "Your option: ";
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
