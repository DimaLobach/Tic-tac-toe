using namespace sf;
using namespace std;

bool wins(char* MAP, int size, char player) {
	char target = player;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			if(MAP[i + j * size] != target) {
				goto yCheck;
			}
		}
		return true;
	yCheck:
		for(int j = 0; j < size; j++) {
			if(MAP[j + i * size] != target) {
				goto bb;
			}
		}
		return true;
	bb:
		MAP[0] = MAP[0];
	}
	for(int i = 0; i < size; i++) {
		if(MAP[i + i * size] != target) {
			goto check;
		}
	}
	return true;
check:
	for(int i = 0; i < size; i++) {
		if(MAP[size - i - 1 + i * size] != target) {
			return false;
		}
	}
	return true;
}

vector<Vector2i> getEmptyCells(char* map, int size) {
	vector<Vector2i> results;
	for(int y = 0; y < size; y++) {
		for(int x = 0; x < size; x++) {
			if(map[x + y * size] == ' ') {
				results.push_back(Vector2i(x, y));
			}
		}
	}
	return results;
}

int minimax(char* map, int size, char player, char ai, char hu) {
	if(wins(map, size, ai)) return  10;
	if(wins(map, size, hu)) return -10;

	vector<Vector2i> cells = getEmptyCells(map, size);

	if(cells.size() == 0) return -1;

	int score = player == ai ? -1024 : 1024;

	for(Vector2i cell : cells) {
		map[cell.x + cell.y * size] = player;

		if (player == ai) {
			int result = minimax(map, size, hu, ai, hu);
			score = max(result, score);
		} else {
			int result = minimax(map, size, ai, ai, hu);
			score = min(result, score);
		}

		map[cell.x + cell.y * size] = ' ';
	}
	return score;
}

Vector2i AI(char* map, int size, char ai, char hu) {
	vector<Vector2i> cells = getEmptyCells(map, size);

	int bestScore = -1024;
	Vector2i bestCell;

	for(Vector2i cell : cells) {
		map[cell.x + cell.y * size] = ai;

		int result = minimax(map, size, hu, ai, hu);

		if(result > bestScore) {
			bestScore = result;
			bestCell = cell;
		}


		map[cell.x + cell.y * size] = ' ';
	}
	return bestCell;
}

Vector2i AI2(char* map, int size, char ai, char hu) {
	vector<Vector2i> cells = getEmptyCells(map, size);

	if(cells.empty())
		for(int i = 0; i < size * size; i++)
			if(map[i] == ai)
				return Vector2i(i % size, i / size);

	for(Vector2i cell : cells) {
		map[cell.x + cell.y * size] = ai;

		if(wins(map, size, ai)) {
			map[cell.x + cell.y * size] = ' ';
			return cell;
		}

		map[cell.x + cell.y * size] = hu;

		if(wins(map, size, hu)) {
			map[cell.x + cell.y * size] = ' ';
			return cell;
		}

		map[cell.x + cell.y * size] = ' ';
	}

	for(Vector2i cell : cells) {
		if(cell.x == (size / 2))
			if(cell.y == (size / 2))
				return cell;
	}

	for(Vector2i cell : cells) {
		if(cell.x == 0 || cell.x == (size - 1))
			if(cell.y == 0 || cell.y == (size - 1))
				return cell;
	}
	return cells[0];
}
