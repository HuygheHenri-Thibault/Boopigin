#include "EnemyFactory.h"

#include <TextureComponent.h>
#include <TransformComponent.h>
#include "StateComponent.h"
#include "TumbleMovementComponent.h"

float EnemyFactory::m_LeapTime = 0.7f;
unsigned int EnemyFactory::m_Depth = 1;

float EnemyFactory::m_SlickSamSize = 32.f;
std::string EnemyFactory::m_SlickTexturePath = "textures/slickFrame.png"; // Todo: this must become an animation
std::string EnemyFactory::m_SamTexturePath = "textures/samFrame.png";

std::shared_ptr<boop::GameObject> EnemyFactory::MakeSlick(boop::Scene& scene, LevelComponent* pLevel,
	const glm::ivec2& startCoordinate)
{
	return MakeTumbler(scene, pLevel, m_SlickTexturePath, startCoordinate);
}

std::shared_ptr<boop::GameObject> EnemyFactory::MakeSam(boop::Scene& scene, LevelComponent* pLevel,
	const glm::ivec2& startCoordinate)
{
	return MakeTumbler(scene, pLevel, m_SamTexturePath, startCoordinate);
}

std::shared_ptr<boop::GameObject> EnemyFactory::MakeTumbler(boop::Scene& scene, LevelComponent* pLevel, const std::string& texturePath,
	const glm::ivec2& startCoordinate)
{
	auto go = std::make_shared<boop::GameObject>();
	go->AddComponent(new boop::TextureComponent(texturePath, m_SlickSamSize, m_SlickSamSize));
	auto* transform = new boop::TransformComponent();
	go->AddComponent(transform);

	auto* state = new StateComponent();
	go->AddComponent(state);

	auto* jumper = new JumpComponent(transform, state, m_LeapTime);
	go->AddComponent(jumper);
	go->AddComponent(new TumbleMovementComponent(pLevel->GetTileWithCoordinate(startCoordinate), pLevel, jumper, state));
	scene.Add(go, m_Depth);
	return go;
}
