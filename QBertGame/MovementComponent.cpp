#include "MovementComponent.h"
#include "LevelFactory.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "QuadDirectionalJumpAnimationComponent.h"

MovementComponent::MovementComponent(const glm::ivec2& startPos, LevelComponent* level, JumpComponent* jumper)
	: m_CurrentPos(startPos)
	, m_pLevel(level)
	, m_pJumper(jumper)
	, m_DoUnFlip(false)
	, m_DoFlip(false)
	, m_DoGainScore(false)
{
	EventQueue::GetInstance().Subscribe("JumpCompleted", this);
}

void MovementComponent::Startup()
{
	SetPosition(m_CurrentPos);
}

bool MovementComponent::MoveUp()
{
	return Move(Direction::up);
}
bool MovementComponent::MoveRight()
{
	return Move(Direction::right);
}
bool MovementComponent::MoveDown()
{
	return Move(Direction::down);
}
bool MovementComponent::MoveLeft()
{
	return Move(Direction::left);
}
bool MovementComponent::MoveEscheresqueRight()
{
	return Move(Direction::escheresqueRight);
}
bool MovementComponent::MoveEscheresqueLeft()
{
	return Move(Direction::escheresqueLeft);
}

bool MovementComponent::Move(Direction movementDirection)
{
	glm::ivec2 coordinateChange = { 0, 0 };
	
	switch (movementDirection)
	{
	case Direction::up:
		coordinateChange.y = -1;
		break;
	case Direction::right:
		coordinateChange.x = 1;
		break;
	case Direction::down:
		coordinateChange.y = 1;
		break;
	case Direction::left:
		coordinateChange.x = -1;
		break;
	case Direction::escheresqueRight:
		coordinateChange.x = -1;
		coordinateChange.y = 1;
		break;
	case Direction::escheresqueLeft:
		coordinateChange.x = 1;
		coordinateChange.y = -1;
		break;
	}

	const glm::ivec2 newTileCoordinate = m_CurrentPos + coordinateChange;

	return MoveTo(newTileCoordinate, movementDirection);
}
bool MovementComponent::MoveTo(const glm::ivec2& tileCoordinate, Direction direction)
{
	if (m_pLevel->IsCoordinateInBounds(tileCoordinate))
	{
		TileComponent* newTile = m_pLevel->GetTileWithCoordinate(tileCoordinate);
		const glm::vec2 newPos = GetTileStandPosition(newTile);

		if (m_pJumper->StartJump(newPos))
		{
			m_CurrentPos = { newTile->GetColumn(), newTile->GetRow() };
			m_LastMovedInDirection = direction;
			return true;
		}
		return false;
	}
	else
	{
		//if (m_pLevel.DoesCoordinateHaveTeleporter(tileCoordinate))
		//{
		//	
		//}
		//else
		{
			glm::vec2 jumpOffset{};
			const float halfTileSize = LevelFactory::m_TileSize / 2;
			switch (direction)
			{
			case Direction::up:
				jumpOffset.x = halfTileSize;
				jumpOffset.y = -halfTileSize;
				break;
			case Direction::escheresqueRight:
			case Direction::right:
				jumpOffset.x = halfTileSize;
				jumpOffset.y = halfTileSize;
				break;
			case Direction::down:
				jumpOffset.x = -halfTileSize;
				jumpOffset.y = halfTileSize;
				break;
			case Direction::escheresqueLeft:
			case Direction::left:
				jumpOffset.x = -halfTileSize;
				jumpOffset.y = -halfTileSize;
				break;
			}
			
			const glm::vec2 jumpOffPoint = GetTileStandPosition(m_pLevel->GetTileWithCoordinate(m_CurrentPos)) + jumpOffset;
			
			if (m_pJumper->StartJump(jumpOffPoint))
			{
				m_CurrentPos = tileCoordinate;
				m_LastMovedInDirection = direction;
				// die
				return true;
			}
		}
		
	}
	return false;
}

bool MovementComponent::OnEvent(const Event& event)
{
	if (event.message == "JumpCompleted")
	{
		auto* jumperThatDidJump = event.GetData<JumpComponent*>();
		if (jumperThatDidJump == m_pJumper)
		{
			if (ChangesTileStateOnWalk() && m_pLevel->IsCoordinateInBounds(m_CurrentPos))
			{
				if (m_DoFlip)
				{
					m_pLevel->GetTileWithCoordinate(m_CurrentPos)->OnFlip(m_DoGainScore);
				}
				else
				{
					m_pLevel->GetTileWithCoordinate(m_CurrentPos)->OnUnFlip(m_DoGainScore);
				}	
			}
			return true;
		}
	}
	return false;
}

bool MovementComponent::ChangesTileStateOnWalk() const
{
	return m_DoUnFlip || m_DoFlip;
}

void MovementComponent::SetPosition(const glm::ivec2& coordinate)
{
	const glm::vec2 pos = GetTileStandPosition(m_pLevel->GetTileWithCoordinate(coordinate));
	m_pJumper->SetStartPos(pos);
}

glm::ivec2 MovementComponent::GetCurrentPosition() const
{
	return m_CurrentPos;
}
Direction MovementComponent::GetLastMovedInDirection() const
{
	return m_LastMovedInDirection;
}
const LevelComponent* MovementComponent::GetLevel() const
{
	return m_pLevel;
}
const JumpComponent* MovementComponent::GetJumper() const
{
	return m_pJumper;
}

glm::vec2 MovementComponent::GetTileStandPosition(TileComponent* tile) const
{
	if (tile == nullptr)
	{
		return { 0,0 };
	}

	return GetTileStandPosition(tile, GetTextureDimension(m_pOwner));
}

glm::vec2 MovementComponent::GetTileStandPosition(TileComponent* tile, const glm::ivec2& textureDimension)
{
	if (tile == nullptr)
	{
		return { 0,0 };
	}

	const auto entityHeight = static_cast<float>(textureDimension.y);
	const auto entityWidth = static_cast<float>(textureDimension.x);

	const float verticalAdjustment = LevelFactory::m_TileSize / 4 - entityHeight;
	const float horizontalAdjustment = LevelFactory::m_TileSize / 2 - entityWidth / 2;
	const glm::vec3 position = tile->GetOwner()->GetComponentOfType<boop::TransformComponent>()->GetPosition();

	return { position.x + horizontalAdjustment, position.y + verticalAdjustment };
}

glm::ivec2 MovementComponent::GetTextureDimension(boop::GameObject* object)
{
	auto* texture = object->GetComponentOfType<boop::TextureComponent>();
	if (texture != nullptr)
	{
		return { texture->GetWidth(), texture->GetHeight() };
	}

	auto* animation = object->GetComponentOfType<boop::AnimationComponent>();
	if (animation != nullptr)
	{
		return { animation->GetWidth(), animation->GetHeight() };
	}

	return { 0, 0 };
}
