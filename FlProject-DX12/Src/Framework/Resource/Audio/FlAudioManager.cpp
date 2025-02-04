#include "FlAudioManager.h"

FlAudioManager::FlAudioManager() noexcept
{
	{ // <Initialize:FMOD::System>
		auto rawSystem{ static_cast<FMOD::System*>(nullptr) };

		FMOD::System_Create(&rawSystem);
		m_upSystem.reset(rawSystem);
		m_upSystem->init(512, FMOD_INIT_NORMAL, nullptr);
	}

	{ // <Initialize:FMOD::ChannelGroup>
		auto rawChannelGroup{ static_cast<FMOD::ChannelGroup*>(nullptr) };

		m_upSystem->createChannelGroup("MasterGroup", &rawChannelGroup);
		m_upChannelGroup.reset(rawChannelGroup);
	}
}

const std::shared_ptr<FMOD::Sound> FlAudioManager::LoadSound(const std::string& filePath)
{
	// <Quick Return:std::shared_ptr<FMOD::Sound>>
	if (m_soundController.find(filePath) != m_soundController.end()) return m_soundController[filePath].first;

	auto rawSound{ static_cast<FMOD::Sound*>(nullptr) };
	if (m_upSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &rawSound) != FMOD_OK)
	{
		_ASSERT_EXPR(false, "Failed To Create Sound");
		return nullptr;
	}

	auto upNewSound{ std::unique_ptr<FMOD::Sound, FMODSoundDeleter>(rawSound) };

	auto spSound{ std::shared_ptr<FMOD::Sound>(upNewSound.release(), FMODSoundDeleter()) };
	m_soundController[filePath] = { spSound, nullptr }; 

	return spSound;
}

const std::shared_ptr<FMOD::Sound> FlAudioManager::LoadStreamSound(const std::string& filePath)
{
	if (m_soundController.find(filePath) != m_soundController.end()) return m_soundController[filePath].first;

	auto rawSound{ static_cast<FMOD::Sound*>(nullptr) };

	if (m_upSystem->createSound(filePath.c_str(), FMOD_CREATESTREAM | FMOD_DEFAULT, nullptr, &rawSound) != FMOD_OK)
	{
		_ASSERT_EXPR(false, "Failed To Create Stream Sound");
		return nullptr;
	}

	auto upNewSound{ std::unique_ptr<FMOD::Sound, FMODSoundDeleter>(rawSound) };

	auto spSound{ std::shared_ptr<FMOD::Sound>(upNewSound.release(), FMODSoundDeleter()) };
	m_soundController[filePath] = { spSound, nullptr };

	return spSound;
}

void FlAudioManager::Play()
{
	auto it{ m_soundController.begin() };

	Stop();

	while (it != m_soundController.end())
	{
		if (!LoadSound(it->first)) continue;

		auto& soundChannelPair{ m_soundController[it->first] };
		auto& spSound         { soundChannelPair.first };
		auto& upChannel       { soundChannelPair.second };

		auto mode{ FMOD_MODE{} };
		upChannel->getMode(&mode);

		if (spSound)
		{
			auto rawChannel{ static_cast<FMOD::Channel*>(nullptr) };
			m_upSystem->playSound(spSound.get(), m_upChannelGroup.get(), false, &rawChannel);

			if (rawChannel) rawChannel->setMode(mode);

			upChannel.reset(rawChannel);
		}

		++it;
	}
}

void FlAudioManager::Play(const std::string& filePath, const bool isLoop)
{
	// <Quick Return:オーディオファイルがないもしくは再生中であるなら>
	if (!LoadSound(filePath) || IsPlay(filePath)) return;

	auto& soundChannelPair{ m_soundController[filePath] };
	auto& spSound         { soundChannelPair.first };
	auto& upChannel       { soundChannelPair.second };

	if (spSound)
	{
		auto rawChannel{ static_cast<FMOD::Channel*>(nullptr) };
		m_upSystem->playSound(spSound.get(), m_upChannelGroup.get(), false, &rawChannel);

		if (rawChannel && isLoop) rawChannel->setMode(FMOD_LOOP_NORMAL);

		upChannel.reset(rawChannel);
	}
}

void FlAudioManager::Play3D()
{
	auto it{ m_soundController.begin() };

	Stop();

	while (it != m_soundController.end())
	{
		if (!LoadSound(it->first)) continue;

		auto& soundChannelPair{ m_soundController[it->first] };
		auto& spSound{ soundChannelPair.first };
		auto& upChannel{ soundChannelPair.second };

		auto mode{ FMOD_MODE{} };
		upChannel->getMode(&mode);

		if (spSound)
		{
			auto rawChannel{ static_cast<FMOD::Channel*>(nullptr) };
			m_upSystem->playSound(spSound.get(), m_upChannelGroup.get(), false, &rawChannel);

			if (rawChannel) rawChannel->setMode(mode | FMOD_3D);

			upChannel.reset(rawChannel);
		}

		++it;
	}
}

void FlAudioManager::Play3D(const std::string& filePath, const bool isLoop)
{
	// <Quick Return:オーディオファイルがないもしくは再生中であるなら>
	if (!LoadSound(filePath) || IsPlay(filePath)) return;

	auto& soundChannelPair{ m_soundController[filePath] };
	auto& spSound{ soundChannelPair.first };
	auto& upChannel{ soundChannelPair.second };

	if (spSound)
	{
		auto rawChannel{ static_cast<FMOD::Channel*>(nullptr) };
		m_upSystem->playSound(spSound.get(), m_upChannelGroup.get(), false, &rawChannel);

		if (rawChannel && isLoop) rawChannel->setMode(FMOD_LOOP_NORMAL | FMOD_3D);

		upChannel.reset(rawChannel);
	}
}

const bool FlAudioManager::IsPlay()
{
	auto is{ false };
	if (m_upChannelGroup) m_upChannelGroup->isPlaying(&is);
	return is;
}

const bool FlAudioManager::IsPlay(const std::string& filePath)
{
	auto it{ m_soundController.find(filePath) };
	auto is{ false };
	if (it != m_soundController.end() && it->second.second) it->second.second->isPlaying(&is);
	return is;
}

void FlAudioManager::Stop()
{
	if (m_upChannelGroup) m_upChannelGroup->stop();
}

void FlAudioManager::Stop(const std::string& filePath)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->stop();
}

void FlAudioManager::Set3DSoundSettings(const Math::Vector3& position, const Math::Vector3& velocity)
{
	auto pos{ SimpleMathToFmodVec(position) };
	auto vel{ SimpleMathToFmodVec(velocity) };
	if (m_upChannelGroup) m_upChannelGroup->set3DAttributes(&pos, &vel);
}

void FlAudioManager::Set3DSoundSettings(const std::string& filePath, const Math::Vector3& position, const Math::Vector3& velocity)
{
	auto pos{ SimpleMathToFmodVec(position) };
	auto vel{ SimpleMathToFmodVec(velocity) };
	auto it { m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->set3DAttributes(&pos, &vel);
}

void FlAudioManager::Set3DListenerAttributes(const Math::Vector3& listenerPosition, const Math::Vector3& listenerVelocity, const Math::Vector3& forward, const Math::Vector3& up)
{
	auto pos  { SimpleMathToFmodVec(listenerPosition) };
	auto vel  { SimpleMathToFmodVec(listenerVelocity) };
	auto fwd  { SimpleMathToFmodVec(forward) };
	auto upVec{ SimpleMathToFmodVec(up) };

	m_upSystem->set3DListenerAttributes(Def::IntZero, &pos, &vel, &fwd, &upVec);
}

void FlAudioManager::SetVolume(float volume)
{
	if (m_upChannelGroup) m_upChannelGroup->setVolume(volume);
}

void FlAudioManager::SetVolume(const std::string& filePath, float volume)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setVolume(volume);
}

void FlAudioManager::Mute(const bool isMute)
{
	if (m_upChannelGroup) m_upChannelGroup->setMute(isMute);
}

void FlAudioManager::Mute(const std::string& filePath, const bool isMute)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setMute(isMute);
}

void FlAudioManager::SetPitch(float pitch)
{
	if (m_upChannelGroup) m_upChannelGroup->setPitch(pitch);
}

void FlAudioManager::SetPitch(const std::string& filePath, float pitch)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setPitch(pitch);
}

void FlAudioManager::SetReverb(float reverbLevel)
{
	auto it{ m_soundController.begin() };
	auto rawReverbDSP{ static_cast<FMOD::DSP*>(nullptr) };

	m_upSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &rawReverbDSP);
	rawReverbDSP->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, reverbLevel);

	while (it != m_soundController.end())
	{
		if (it->second.second) (*it).second.second->addDSP(0, rawReverbDSP);
		++it;
	}
	rawReverbDSP->release();
}

void FlAudioManager::SetReverb(const std::string& filePath, float reverbLevel)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second)
	{
		auto rawReverbDSP{ static_cast<FMOD::DSP*>(nullptr) };
		m_upSystem->createDSPByType(FMOD_DSP_TYPE_SFXREVERB, &rawReverbDSP);
		rawReverbDSP->setParameterFloat(FMOD_DSP_SFXREVERB_WETLEVEL, reverbLevel);
		it->second.second->addDSP(0, rawReverbDSP);
		rawReverbDSP->release();
	}
}

void FlAudioManager::SetMode(const FMOD_MODE mode)
{
	if (m_upChannelGroup) m_upChannelGroup->setMode(mode);
}

void FlAudioManager::SetMode(const std::string& filePath, const FMOD_MODE mode)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setMode(mode);
}

void FlAudioManager::Pause()
{
	if (m_upChannelGroup) m_upChannelGroup->setPaused(true);
}

void FlAudioManager::Pause(const std::string& filePath)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setPaused(true);
}

void FlAudioManager::Resume()
{
	if (m_upChannelGroup) m_upChannelGroup->setPaused(false);
}

void FlAudioManager::Resume(const std::string& filePath)
{
	auto it{ m_soundController.find(filePath) };
	if (it != m_soundController.end() && it->second.second) it->second.second->setPaused(false);
}

void FlAudioManager::Update()
{
	m_upSystem->update();
}
