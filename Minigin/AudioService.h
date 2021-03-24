#pragma once

namespace boop
{
	class AudioService
	{
	public:
		virtual ~AudioService() = default;

		virtual void PlayMusic(const std::string& filename, int volumePercentage) = 0;
		virtual void PlaySound(const std::string& filename, int volumePercentage) = 0;
	};

	class NullAudioService final : public AudioService
	{
	public:
#pragma warning( push )
#pragma warning( disable : 4100 )
		void PlayMusic(const std::string& filename, int volumePercentage) override {};
		void PlaySound(const std::string& filename, int volumePercentage) override {};
#pragma warning( pop )

	};
}

