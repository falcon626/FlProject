#pragma once

/// <summary> =Flyweight= </summary>
class FlAudioManager 
{
public:

	FlAudioManager() noexcept;
	~FlAudioManager() = default;

	const std::shared_ptr<FMOD::Sound> LoadSound(const std::string& filePath);
	const std::shared_ptr<FMOD::Sound> LoadStreamSound(const std::string& filePath);

	void Play();
	void Play(const std::string& filePath, const bool isLoop = false);

	void Play3D();
	void Play3D(const std::string& filePath, const bool isLoop = false);

	const bool IsPlay();
	const bool IsPlay(const std::string& filePath);

	void Stop();
	void Stop(const std::string& filePath);

	void Set3DSoundSettings(const Math::Vector3& position, const Math::Vector3& velocity);
	void Set3DSoundSettings(const std::string& filePath, const Math::Vector3& position, const Math::Vector3& velocity);

	void Set3DListenerAttributes(const Math::Vector3& listenerPosition, const Math::Vector3& listenerVelocity, const Math::Vector3& forward, const Math::Vector3& up);

	void SetVolume(float volume);
	void SetVolume(const std::string& filePath, float volume);

	void Mute(const bool isMute = true);
	void Mute(const std::string& filePath, const bool isMute = true);

	void SetPitch(float pitch);
	void SetPitch(const std::string& filePath, float pitch);

	void SetReverb(float reverbLevel);
	void SetReverb(const std::string& filePath, float reverbLevel);

	void SetMode(const FMOD_MODE mode);
	void SetMode(const std::string& filePath, const FMOD_MODE mode);

	void Pause();
	void Pause(const std::string& filePath);

	void Resume();
	void Resume(const std::string& filePath);

	void Update();

	inline auto ClearCache() noexcept { m_soundController.clear(); }

	FlAudioManager(const FlAudioManager&) = delete;
	FlAudioManager& operator=(const FlAudioManager&) = delete;
private:

	inline const auto FmodVecToSimpleMath(const FMOD_VECTOR& vec) noexcept { Math::Vector3 resultVec{ vec.x,vec.y,vec.z }; return resultVec; }
	inline const auto SimpleMathToFmodVec(const Math::Vector3& vec) noexcept { FMOD_VECTOR resultVec{ vec.x,vec.y,vec.z }; return resultVec; }

	// <Custom Deleter:FMOD System>
	struct FMODSystemDeleter {
		void operator()(FMOD::System* system) const {
			if (system) system->release();
		}
	};

	// <Custom Deleter:FMOD Sound>
	struct FMODSoundDeleter {
		void operator()(FMOD::Sound* sound) const {
			if (sound) sound->release();
		}
	};

	// <Custom Deleter:FMOD ChannelGroup>
	struct FMODChannelGroupDeleter {
		void operator()(FMOD::ChannelGroup* channelGroup) const {
			if (channelGroup) channelGroup->stop();
		}
	};

	// <Custom Deleter:FMOD Channel>
	struct FMODChannelDeleter {
		void operator()(FMOD::Channel* channel) const {
			if (channel) channel->stop();
		}
	};

	std::unique_ptr<FMOD::System, FMODSystemDeleter> m_upSystem;
	std::unique_ptr<FMOD::ChannelGroup, FMODChannelGroupDeleter> m_upChannelGroup;
	std::unordered_map<
		std::string, std::pair<
		std::shared_ptr<FMOD::Sound>, std::unique_ptr<FMOD::Channel, FMODChannelDeleter>
		>
	> m_soundController;
};