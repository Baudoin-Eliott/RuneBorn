#include "runeSystem.h"
#include "textureManager.h"
#include "MagicHelper.h"  
#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>


using json = nlohmann::json;

RuneSystem::RuneSystem() {
	isCasting = false;
	isDrawing = false;
	difficulty = 0;
	spacingX = 0;
	spacingY = 0;
	tolerance = 20.0f;
	frame = 0;
	frameStart = 0;
	frameDelay = 100;
}

RuneSystem::~RuneSystem() {
}

void RuneSystem::openRuneMenu(int dif) {
	isCasting = true;
	isDrawing = false;
	difficulty = dif;
	connections.clear();  // Reset les connexions
}

std::vector<int> RuneSystem::closeRuneMenu() {
	isCasting = false;
	isDrawing = false;
	return connections;
}

void RuneSystem::DrawRuneMenu() {
	SDL_Texture* StarTex = TextureManager::LoadTexture("Assets/FX/Magic/Circle/SpriteSheetSpark2.png");
	drawBack();

	spacingX = 668.0f / (difficulty + 1);
	spacingY = 640.0f / (difficulty + 1);
	float StartSize = (spacingX - 8.0f) < 40 ? (spacingX - 8.0f) : 40;

	// Animation des étoiles (optionnel)
	/*
	if (SDL_GetTicks() - frameStart >= frameDelay) {
		frameStart = SDL_GetTicks();
		frame = (frame + 1) % 4;
	}
	*/


	// Dessine les points de la grille
	for (int i = 0; i < difficulty; i++) {
		for (int j = 0; j < difficulty; j++) {
			TextureManager::Draw(
				StarTex,
				{ (float)frame * 32, 0, 32, 32 },
				{ (i + 1) * spacingX - StartSize / 2,
				 (j + 1) * spacingY - StartSize / 2,
				 StartSize,
				 StartSize }
			);
		}
	}
	//dessine les aides si jamais il y a
	if (helpedSort != nullptr && helpedSort->pattern.size() > 0 && helpedSort->lvl == difficulty) {
		SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);  // Bleu
		for (int i = 0; i < helpedSort->pattern.size() - 1; i++) {
			Tuple2f start = getPointPosition(helpedSort->pattern[i]);
			Tuple2f end = getPointPosition(helpedSort->pattern[i + 1]);
			SDL_RenderLine(Game::renderer, start.x, start.y, end.x, end.y);
		}
		SDL_RenderLine(Game::renderer,
			getPointPosition(helpedSort->pattern.back()).x,
			getPointPosition(helpedSort->pattern.back()).y,
			getPointPosition(helpedSort->pattern[0]).x,
			getPointPosition(helpedSort->pattern[0]).y);
	}

	// Dessine les lignes entre points connectés
	SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);  // Jaune
	for (int i = 0; i < (int)connections.size() - 1; i++) {
		Tuple2f start = getPointPosition(connections[i]);
		Tuple2f end = getPointPosition(connections[i + 1]);
		SDL_RenderLine(Game::renderer, start.x, start.y, end.x, end.y);
	}

	// Si on dessine actuellement, ligne vers la souris
	SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
	if (isDrawing && !connections.empty()) {
		Tuple2f lastPoint = getPointPosition(connections.back());
		if ((float)Game::event.motion.x != 0.f && (float)Game::event.motion.y != 0.f)
		{
			lastMousePos = Tuple2f{
				(float)Game::event.motion.x,
				(float)Game::event.motion.y
			};
			Tuple2f mousePos = lastMousePos;
			SDL_RenderLine(Game::renderer, lastPoint.x, lastPoint.y, mousePos.x, mousePos.y);
		}
	}


}

void RuneSystem::drawBack() {
	SDL_Texture* BackTex = TextureManager::LoadTexture("Assets/Ui/Dialog/FacesetBox.png");

	// Draw the Left upper corner
	TextureManager::Draw(BackTex, { 0, 0, 16, 16 }, { 20, 20, 16, 16 });
	// Draw the Right upper corner
	TextureManager::Draw(BackTex, { 32, 0, 16, 16 }, { 632, 20, 16, 16 });
	// Draw the Left lower corner
	TextureManager::Draw(BackTex, { 0, 32, 16, 16 }, { 20, 604, 16, 16 });
	// Draw the Right lower corner
	TextureManager::Draw(BackTex, { 32, 32, 16, 16 }, { 632, 604, 16, 16 });
	// Draw the Upper border
	TextureManager::Draw(BackTex, { 16, 0, 16, 16 }, { 36, 20, 608, 16 });
	// Draw the Lower border
	TextureManager::Draw(BackTex, { 16, 32, 16, 16 }, { 36, 604, 608, 16 });
	// Draw the Left border
	TextureManager::Draw(BackTex, { 0, 16, 16, 16 }, { 20, 32, 16, 575 });
	// Draw the Right border
	TextureManager::Draw(BackTex, { 32, 16, 16, 16 }, { 632, 32, 16, 575 });
	// Fill the center
	TextureManager::Draw(BackTex, { 16, 16, 16, 16 }, { 36, 36, 604, 575 });
}

void RuneSystem::ComparePattern(std::vector<int> userPattern)
{

	std::ifstream file("Assets/Datas/sort.json");
	json data;
	file >> data;
	std::vector<std::vector<int>> spellPattern;
	std::string sort;
	int minSum = 9999;
	std::vector<int> trace;

	//on vient normaliser le pattern de l'utilisateur
	int xmin = userPattern[0] / difficulty;
	int xmax = userPattern[0] / difficulty;
	int ymin = userPattern[0] % difficulty;
	int ymax = userPattern[0] % difficulty;
	int dif = 0;
	for (int i = 0; i < userPattern.size(); i++) {
		xmin = userPattern[i] / difficulty < xmin ? userPattern[i] / difficulty : xmin;
		xmax = userPattern[i] / difficulty > xmax ? userPattern[i] / difficulty : xmax;
		ymin = userPattern[i] % difficulty < ymin ? userPattern[i] % difficulty : ymin;
		ymax = userPattern[i] % difficulty > ymax ? userPattern[i] % difficulty : ymax;
	}
	dif = xmax - xmin > ymax - ymin ? xmax - xmin : ymax - ymin;
	dif++;
	for (int i = 0; i < userPattern.size(); i++) {
		userPattern[i] = (userPattern[i] / difficulty - xmin) * dif + (userPattern[i] % difficulty - ymin);
	}
	std::cout << dif << std::endl;
	//on verifie si les point correspondent a un sort
	std::vector<int> value;
	std::string levelKey = "Lvl" + std::to_string(dif);
	if (!data[levelKey].is_null()) {
		for (auto it = data[levelKey].begin(); it != data[levelKey].end(); ++it) {
			std::string spellName = it.key();
			std::cout << spellName << std::endl;
			auto& patternJson = it.value();
			spellPattern = patternJson["pattern"].get<std::vector<std::vector<int>>>();
			value = patternJson["value"].get<std::vector<int>>();
			int sum = 0;
			for (int j = 0; j < userPattern.size(); j++) {
				sum += spellPattern[userPattern[j] / dif][userPattern[j] % dif];
			}
			if (sum < minSum) {
				minSum = sum;
				sort = spellName;
			}
		}

	}
	//en cas de sort parfait, on verifie si le tracer est bon


	if (minSum == 0 && userPattern.size() == value.size()) {
		std::vector<int> goodIndices;
		std::cout << "score parfait, on verifie le tracer" << std::endl;
		//on cherche le point de depart
		int indexStart = -1;
		for (int i = 0; i < value.size(); i++) {
			if (value[i] == userPattern[0]) {
				indexStart = i;
				break;
			}
		}
		//on affiche les deux tab pour debug
		for (int i = 0; i < userPattern.size(); i++) {
			std::cout << userPattern[i] << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < value.size(); i++) {
			std::cout << value[i] << " ";
		}
		std::cout << "indiceStart: " << indexStart << std::endl;

		for (int i = 0; i < userPattern.size(); i++) {
			if (userPattern[i] != value[(i + indexStart < value.size()) ? (i + indexStart) : (indexStart - (value.size() - i))]) {
				minSum += 2;
			}
			else goodIndices.push_back(i);
		}

		if (goodIndices[0] == 0 && goodIndices[1] == userPattern.size() / 2) {
			//on reverse le vecteur;
			minSum = 0;
			for (int i = 0; i < userPattern.size() / 2; i++) {
				int temps = userPattern[i];
				userPattern[i] = userPattern[userPattern.size()-1 - i];
				userPattern[userPattern.size() -1- i] = temps;
			}


			indexStart = -1;
			for (int i = 0; i < value.size(); i++) {
				if (value[i] == userPattern[0]) {
					indexStart = i;
					break;
				}
			}
			//on affiche les deux tab pour debug
			for (int i = 0; i < userPattern.size(); i++) {
				std::cout << userPattern[i] << " ";
			}
			std::cout << std::endl;
			for (int i = 0; i < value.size(); i++) {
				std::cout << value[i] << " ";
			}
			std::cout << "indiceStart: " << indexStart << std::endl;

			for (int i = 0; i < userPattern.size(); i++) {
				if (userPattern[i] != value[(i + indexStart < value.size()) ? (i + indexStart) : (indexStart - (value.size() - i))]) {
					minSum += 2;
					goodIndices.push_back(i);
				}
				else goodIndices.push_back(i);
			}
		}
	}



	minSum += abs((int)(value.size() - userPattern.size())) * 5;
	std::cout << minSum << std::endl;
	if (minSum <= 0) std::cout << "sort parfait ! On lance " << sort << std::endl;
	else if (minSum < 10) std::cout << "sort reussi ! On lance " << sort << " avec une puissance de " << (100 - minSum * 10) << "%" << std::endl;
	else if (minSum <= 20) std::cout << "sort " << sort << " echoue, on lance un debuff" << std::endl;
	else std::cout << "Aucun sort reconnu, on enleve du mana" << std::endl;
}

int RuneSystem::FindClosestPoint(Vector2D screenPos) {
	// screenPos est en coordonnées écran (0-668, 0-640)
	for (int i = 0; i < difficulty; i++) {
		for (int j = 0; j < difficulty; j++) {
			float posX = (i + 1) * spacingX;
			float posY = (j + 1) * spacingY;
			float dist = sqrt((screenPos.x - posX) * (screenPos.x - posX) +
				(screenPos.y - posY) * (screenPos.y - posY));
			if (dist < tolerance) {
				return i + j * difficulty;
			}
		}
	}
	return -1;
}

Tuple2f RuneSystem::getPointPosition(int pointId) {
	float x = (pointId % difficulty + 1) * spacingX;
	float y = (pointId / difficulty + 1) * spacingY;
	Tuple2f pos = { x, y };
	return pos;
}