#include "TileComponent.h"
#include <TextureComponent.h>
#include <EventQueue.h>
#include <utility>

using namespace boop;

TileComponent::TileComponent(std::vector<std::string> textures, int posX, int posY, bool isCyclical)
	: m_TileTextures(std::move(textures))
	, m_TileCol(posX)
	, m_TileRow(posY)
	, m_FlipState(FlipState::unFlipped)
	, m_IsCyclical(isCyclical)
{
	if (m_TileTextures.size() == 2)
	{
		m_FlipKind = FlipKind::twoState;
	}
	else
	{
		m_FlipKind = FlipKind::threeState;
	}
}

void TileComponent::OnFlip(bool gainScore)
{
	if (HasNextFlipState())
	{
		ToNextFlipState();
		if (gainScore)
		{
			auto* scoreEvent = new Event("ScoreGained", 25);
			EventQueue::GetInstance().Broadcast(scoreEvent);
		}
	}
	else
	{
		if (m_IsCyclical)
		{
			ToPreviousFlipState();
			if (gainScore)
			{
				auto* scoreEvent = new Event("ScoreGained", 25);
				EventQueue::GetInstance().Broadcast(scoreEvent);
			}
		}
	}
}
void TileComponent::OnUnFlip(bool gainScore)
{
	if (HasPreviousFlipState())
	{
		ToPreviousFlipState();
		if (gainScore)
		{
			auto* scoreEvent = new Event("ScoreGained", 25);
			EventQueue::GetInstance().Broadcast(scoreEvent);
		}
	}
}

void TileComponent::UpdateTexture()
{
	int textureIndex = static_cast<std::underlying_type<FlipState>::type>(m_FlipState);
	if (m_FlipKind == FlipKind::twoState && m_FlipState == FlipState::flipped)
	{
		textureIndex--;
	}
	m_pOwner->GetComponentOfType<TextureComponent>()->SetTexture(m_TileTextures[textureIndex]);
}

int TileComponent::GetRow() const
{
	return m_TileRow;
}
int TileComponent::GetColumn() const
{
	return m_TileCol;
}

FlipState TileComponent::GetFlipState() const
{
	return m_FlipState;
}

bool TileComponent::HasNextFlipState() const
{
	return m_FlipState != FlipState::flipped;
}
bool TileComponent::HasPreviousFlipState() const
{
	return m_FlipState != FlipState::unFlipped;
}

void TileComponent::ToNextFlipState()
{
	switch (m_FlipKind)
	{
	case FlipKind::twoState:
		m_FlipState = FlipState::flipped;
		break;
	case FlipKind::threeState:
		if (HasNextFlipState())
		{
			const int flipIntValue = static_cast<std::underlying_type<FlipState>::type>(m_FlipState);
			m_FlipState = static_cast<FlipState>(flipIntValue + 1);
		}
		break;
	}
	UpdateTexture();
}
void TileComponent::ToPreviousFlipState()
{
	switch (m_FlipKind)
	{
	case FlipKind::twoState:
		m_FlipState = FlipState::unFlipped;
		break;
	case FlipKind::threeState:
		if (HasPreviousFlipState())
		{
			const int flipIntValue = static_cast<std::underlying_type<FlipState>::type>(m_FlipState);
			m_FlipState = static_cast<FlipState>(flipIntValue - 1);
		}
		break;
	}
	UpdateTexture();
}
