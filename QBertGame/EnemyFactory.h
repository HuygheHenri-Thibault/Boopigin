#pragma once
#include <memory>
#include <string>
#include <GameObject.h>

#include "LevelComponent.h"

class EnemyFactory
{
public:
	static std::shared_ptr<boop::GameObject> MakeSlick(
		boop::Scene& scene,
		LevelComponent* pLevel,
		const glm::ivec2& startCoordinate = {0,0});

	static std::shared_ptr<boop::GameObject> MakeSam(
		boop::Scene& scene,
		LevelComponent* pLevel,
		const glm::ivec2& startCoordinate = {0,0});

	static float m_LeapTime;
	static unsigned int m_Depth;
	
	static float m_SlickSamSize;
	static std::string m_SlickTexturePath;
	static std::string m_SamTexturePath;

private:
	static std::shared_ptr<boop::GameObject> MakeTumbler(
		boop::Scene& scene,
		LevelComponent* pLevel,
		const std::string& texturePath,
		const glm::ivec2& startCoordinate = { 0,0 });
};
