#pragma once
#include <Scene.h>
#include <vector>
#include <memory>
#include <GameObject.h>
#include "TileComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

struct LevelInfo
{
	int size;
	bool isTwoState;
};

class LevelFactory final
{
public:
	static std::vector<std::shared_ptr<boop::GameObject>> MakeLevel(
		boop::Scene& scene, 
		const glm::vec2& levelCenterPos, 
		int levelNumber, 
		LevelInfo* levelInfo = nullptr);
	
	static float m_TileSize;

private:
	//static void LinkTiles(std::vector<std::shared_ptr<boop::GameObject>>& levelTiles);
	//static void LinkTile(TileComponent* tile, const std::vector<std::shared_ptr<boop::GameObject>>& levelTiles);
	static float GetPyramidHeight(int size);

	static std::string m_TilesLocation;
	static std::string m_LevelsFilePath;
};

