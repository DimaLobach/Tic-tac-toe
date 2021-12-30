#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <iostream>
#include "AI.hpp"

const int size = 3;
const int cellSize = 100;

char MAP[size * size];//X - cross, O - zero
bool step = true;
char human = 'X';

void initMap() {
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			MAP[x + y * size] = ' ';
		}
	}
}

int main() {
	RenderWindow window(VideoMode(300, 300), "XO");

	Texture cross, zero;
	cross.loadFromFile("resources/cross.png");
	zero.loadFromFile("resources/zero.png");


	initMap();
	step = true;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++) {
				if(MAP[x + y * size] != ' ') {
					Sprite sp(MAP[x + y * size] == 'X' ? cross : zero);
					sp.setPosition(Vector2f(x * cellSize + 1, y * cellSize + 1));
					sp.setScale(Vector2f(
					                (float)(cellSize - 2) / sp.getTexture()->getSize().x,
					                (float)(cellSize - 2) / sp.getTexture()->getSize().y
					            ));
					window.draw(sp);
				} else {
					RectangleShape shape(Vector2f(cellSize - 2, cellSize - 2));
					shape.setFillColor(Color::White);
					shape.setPosition(x * cellSize + 1, y * cellSize + 1);
					window.draw(shape);
					if(Mouse::isButtonPressed(Mouse::Left) && (human == 'X' == step)) {
						if(shape.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) {
							MAP[x + y * size] = human;
							step = !step;
//							human = human == 'X' ? 'O' : 'X';
						}
					}
				}
			}

		window.display();

		for(int i = 0; i < size; i++) {
			char target = MAP[i];
			if(target == ' ') {
				goto yCheck;
			}
			for(int j = 0; j < size; j++) {
				if(MAP[i + j * size] != target) {
					goto yCheck;
				}
			}
			initMap();
			step = true;
			human = 'X';
			MessageBox(NULL, target == 'X' ? "X wins!" : "O wins!", "XO", MB_OK);
		yCheck:
			target = MAP[i * size];
			if(target == ' ') {
				goto bb;
			}
			for(int j = 0; j < size; j++) {
				if(MAP[j + i * size] != target) {
					goto bb;
				}
			}
			initMap();
			step = true;
			human = 'X';
			MessageBox(NULL, target == 'X' ? "X wins!" : "O wins!", "XO", MB_OK);
		bb:
			window.display();
		}
		{
			char target = MAP[0];
			if(target == ' ') {
				goto check;
			}
			for(int i = 0; i < size; i++) {
				if(MAP[i + i * size] != target) {
					goto check;
				}
			}
			initMap();
			step = true;
			human = 'X';
			MessageBox(NULL, target == 'X' ? "X wins!" : "O wins!", "XO", MB_OK);
		check:
			target = MAP[(size - 1) * size];
			if(target == ' ') {
				goto br;
			}
			for(int i = 0; i < size; i++) {
				if(MAP[size - i - 1 + i * size] != target) {
					goto br;
				}
			}
			initMap();
			step = true;
			human = 'X';
			MessageBox(NULL, target == 'X' ? "X wins!" : "O wins!", "XO", MB_OK);
		br:
			window.display();

			for(int y = 0; y < size; y++) {
				for(int x = 0; x < size; x++) {
					if(MAP[x + y * size] == ' ') {
						goto br1;
					}
				}
			}
			initMap();
			step = true;
			human = 'X';
			MessageBox(NULL, "Nobody wins!", "XO", MB_OK);
		br1:
			window.display();
		}

		if(human == 'X' == step) {
			Vector2i ai = AI(MAP, size, human, human == 'X' ? 'O' : 'X');
			MAP[ai.x + ai.y * size] = human;
			step = !step;
		}
		
		if(human == 'X' != step) {
			Vector2i ai = AI2(MAP, size, human == 'X' ? 'O' : 'X', human);
			MAP[ai.x + ai.y * size] = human == 'X' ? 'O' : 'X';
			step = !step;
		}
	}
	
	return 0;
}
