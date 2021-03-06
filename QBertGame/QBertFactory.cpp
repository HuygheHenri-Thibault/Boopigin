#include "QBertFactory.h"
#include <TransformComponent.h>
#include <TextureComponent.h>
#include <TextComponent.h>

#include "StateComponent.h"
#include "PlayerDataComponent.h"
#include "JumpComponent.h"
#include "LifeTrackerComponent.h"
#include "ControlledMovementComponent.h"
#include <ResourceManager.h>

#include "PlaySoundOnJumpComponent.h"
#include "ScoreDataComponent.h"
#include "ScoreTrackerComponent.h"
#include "QuadDirectionalJumpAnimationComponent.h"

int QBertFactory::m_QbertSrcSize = 16;
int QBertFactory::m_QbertDstSize = 32;
float QBertFactory::m_QbertLeapTime = 0.7f;
std::string QBertFactory::m_QBertTexturePath = "textures/qbert.png";
std::string QBertFactory::m_QBertJumpSoundPath = "../Data/sound/jump-3.wav";
unsigned int QBertFactory::m_QBertDepth = 1;

std::string QBertFactory::m_QBertTrackerFontPath = "Lingua.otf";
unsigned int QBertFactory::m_QBertTrackerFontSize = 16;
unsigned int QBertFactory::m_QBertTrackerDepth = 2;

std::shared_ptr<boop::GameObject> QBertFactory::MakePlayer(boop::Scene& scene, LevelComponent* pLevel, 
	std::vector<boop::KeyInfo>& controls, const glm::ivec2& startCoordinate)
{
	auto go = std::make_shared<boop::GameObject>();
	auto* transform = new boop::TransformComponent();
	go->AddComponent(transform);

	go->AddComponent(new QuadDirectionalJumpAnimationComponent(m_QBertTexturePath, m_QbertSrcSize, m_QbertSrcSize,
		m_QbertDstSize, m_QbertDstSize));
	
	auto* playerData = new PlayerDataComponent();
	go->AddComponent(playerData);
	auto* state = new StateComponent();
	go->AddComponent(state);

	auto* playerJumper = new JumpComponent(transform, state, m_QbertLeapTime);
	go->AddComponent(playerJumper);
	go->AddComponent(new ControlledMovementComponent(pLevel->GetTileWithCoordinate(startCoordinate), pLevel, playerJumper, controls));
	go->AddTag("qbert");

	go->AddComponent(new PlaySoundOnJumpComponent(m_QBertJumpSoundPath));
	
	scene.Add(go, m_QBertDepth);
	return go;
}

std::shared_ptr<boop::GameObject> QBertFactory::MakePlayerTracker(boop::Scene& scene, std::shared_ptr<boop::GameObject> playerToTrack)
{
	const auto font = boop::ResourceManager::GetInstance().LoadFont(m_QBertTrackerFontPath, m_QBertTrackerFontSize);
	auto go = std::make_shared<boop::GameObject>();
	go->AddComponent(new boop::TransformComponent());
	auto* lifeTrackText = new boop::TextComponent("Lives: X", font);
	go->AddComponent(lifeTrackText);
	go->AddComponent(new LifeTrackerComponent(playerToTrack->GetComponentOfType<PlayerDataComponent>(), lifeTrackText));
	scene.Add(go, m_QBertTrackerDepth);

	return go;
}

std::shared_ptr<boop::GameObject> QBertFactory::MakeScoreTracker(boop::Scene& scene,
	std::shared_ptr<boop::GameObject> scoreDataObject)
{
	const auto font = boop::ResourceManager::GetInstance().LoadFont(m_QBertTrackerFontPath, m_QBertTrackerFontSize);
	auto go = std::make_shared<boop::GameObject>();
	go->AddComponent(new boop::TransformComponent(0, 25.f));
	auto* scoreText = new boop::TextComponent("Score: X", font);
	go->AddComponent(scoreText);
	go->AddComponent(new ScoreTrackerComponent(scoreDataObject->GetComponentOfType<ScoreDataComponent>(), scoreText));
	scene.Add(go, m_QBertTrackerDepth);

	return go;
}
