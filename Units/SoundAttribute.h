#ifndef __SOUND_ATTRIBUTE_H__
#define __SOUND_ATTRIBUTE_H__

#include "TypeLibrary.h"
#include "Timers.h"

long CalcVolume(int vol);
class Archive;
struct SNDSoundParam;
class SoundAttribute;
class SNDSound;
class Sound;

long CalcVolume(int vol);

enum SoundSurfKind{
	SOUND_SURF_ALL = 0,
	SOUND_SURF_KIND1 = 1,
	SOUND_SURF_KIND2 = 2,
	SOUND_SURF_KIND3 = 3,
	SOUND_SURF_KIND4 = 4,
};

typedef StringTable<StringTableBasePolymorphic<SoundAttribute> > SoundAttributeLibrary;
typedef StringTablePolymorphicReference<SoundAttribute, false> SoundReference;

/// ������� ��������� �����.
class SoundAttribute : public PolymorphicBase
{
public:
	SoundAttribute();
	virtual ~SoundAttribute(){ }

	virtual bool is3D() const = 0;
	virtual bool isRaceDependent() const = 0;
	virtual bool isLanguageDependent() const = 0;

	//bool isEmpty() const { return soundFiles_.empty(); }
	bool isEmpty() const { return soundFiles_.empty(); }

	virtual void serialize(Archive& ar);

	//virtual bool convert(SNDSoundParam& param) const;

	// ���� ������ ���������� �� �� ����� ���
	// TODO: ���� ����������� �� ����� � ���� - 
	// ������ ����� ����� � ������ �����������,
	// ��� ������ ��� ������� � ����� �����
	const char* convertFileName(const char* file_name) const;

	bool play(const Vect3f& position = Vect3f::ZERO) const;

	virtual Sound* CreateSound(const char* name) const;
	void CreateSounds() const;
	virtual void ApplyParameters(Sound* sound) const;
	const char* GetPlaySoundName() const;

	bool cycled() const { return cycled_; }
	int fadeOut()const {return fadeOut_;}
	int fadeIn()const {return fadeIn_;}
	int surfKind()const {return (int)surfKind_;}
	bool stopInFogOfWar()const {return stopInFogOfWar_;}
	bool useGlobalFade()const {return useGlobalFade_;}

protected:

	/// ��������� �����
	float volume_;
	/// ����������� ��������� �����
	float volumeMin_;

	/// ������������ ���������� ������������ �������� ������.
	/**
	 � ������ 2D ��� ������� ��������� ������ ����� ���������� ���� �� �����������.
	 � ������ 3D:
	    ���� == 1, ����������� ������ 1 ����, ����������� �� �����������.
	    ���� > 1, ����������� ��� �����, �� ������ ������ ���������.
	*/
	int maxCount_;

	/// ��������� ��������� ������� ��� ������ �����.
	bool frequencyRnd_;
	/// �� ������� ��� ������� ����� ����� �����������.
	float frequencyRndMin_;
	/// �� ������� ��� ������� ����� ����� �������������.
	float frequencyRndMax_;

	/// �������� �����, 
	/**
	 ���� �� ���������, �� ��� ������ ����� �������� ���������� �����-�� �� ���.
	*/
	struct SoundFile : string
	{
		SoundFile(const char* name = "") : string(name) {}
		bool serialize (Archive& ar, const char* name, const char* nameAlt);
	};

	std::vector<SoundFile> soundFiles_;

	bool cycled_;
	int fadeIn_;
	int fadeOut_;
	bool stopInFogOfWar_;
	bool useGlobalFade_;
	SoundSurfKind surfKind_;

	friend SNDSound;
};

/// ��������� �������� �����.
class Sound2DAttribute : public SoundAttribute
{
public:
	bool is3D() const { return false; }
	bool isRaceDependent() const { return false; }
	bool isLanguageDependent() const { return false; }
};

/// ��������� ���������� �����.
class Sound3DAttribute : public SoundAttribute
{
	friend SNDSound;
public:
	Sound3DAttribute();

	void serialize(Archive& ar);

	bool is3D() const { return true; }
	bool isRaceDependent() const { return false; }
	bool isLanguageDependent() const { return false; }

	bool modulateFrequency() const { return modulateFrequency_; }

	//bool convert(SNDSoundParam& param) const;

	virtual Sound* CreateSound(const char* name) const;
	virtual void ApplyParameters(Sound* sound) const;

private:

	/// �������� �������, � ����� ���������� ��������� ����� �������� ������� (�� ���������: 50)
	float radius_; 
	/// ������������ ���������� �� �������, ����� �������� ���� �������� ���� �������� (�� ���������: oo)
	float radiusMax_;

	/// ������ ������� ����� (� ����������� �� �������� ��������� � �.�.)
	bool modulateFrequency_;
	bool muteByMaxDistance_;

};

/// ��������� ���������� ���������.
class VoiceAttribute : public ShareHandleBase
{
public:
	VoiceAttribute();

	struct VoiceFile 
    {
		VoiceFile(const char* name = "") 
		{
			fileName_ = name;
			duration_ = 0.f; 
		}

		void serialize (Archive& ar);

		const char* fileName() const;
		float duration() const; // ����������� ��-�� ������� ����� � ��������� ������

		float duration_;
		string fileName_;
	
		static void saveVoiceFileDurations();
		static void loadVoiceFileDuration();
		static void recalculateVoiceFileDuration(const char* language);

		typedef StaticMap<string, float> VoiceFileDurations;
		static VoiceFileDurations voiceFileDurations_;
	};

	void getNextVoiceFile() const; 
	const char* voiceFile() const; 
	float duration() const;

	bool isEmpty() const { return fileNames_.empty(); }
	
	void serialize(Archive& ar);

	static void setDeterministic(bool deterministic) { deterministic_ = deterministic; }

private:
	typedef vector<VoiceFile> FileNames;
	FileNames fileNames_;
	bool randomSelect_;
	float timeToFun_;
	mutable int index_; 
	mutable int curIndex_; // ����� �������� �����
	mutable DurationTimer funTimer_;

	static bool deterministic_;
};

struct SoundLogoAttributes
{
	void serialize(Archive& ar);
	SoundReference mouseMove_;
	SoundReference fishSwim_;
	SoundReference fishStop_;
	SoundReference fishRise_;
	SoundReference waterOut_;
	SoundReference backGround_;
};

//-------------------------------------------------------------------
class SoundReferences : public vector<SoundReference> 
{
	typedef vector<SoundReference> BaseClass;
public:
	const SoundAttribute* getSound(int surfKind) const;
	bool serialize(Archive& ar, const char* name, const char* nameAlt);
};

struct SoundMarker
{
public:
	SoundMarker();
	void serialize(Archive& ar);

	SoundReferences soundReferences_;
	float phase_;
	bool active_;
};
typedef vector<SoundMarker> SoundMarkers;

#endif /* __SOUND_ATTRIBUTE_H__ */
