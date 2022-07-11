#ifndef _FALLLEAVES_H_
#define _FALLLEAVES_H_

#include "SerializationTypes.h"
#include "StaticMap.h"

struct HandleFallLeaves{
	int data;
	HandleFallLeaves():data(-1){ }
	bool isValid(){return data!=-1;}
};

class cFallLeaves:public cBaseGraphObject
{
public:
	cFallLeaves();
	~cFallLeaves();

	void serialize(Archive& ar);
	void serializeForModel(Archive& ar, const char* modelName);

	HandleFallLeaves createHandle(c3dx* model, float intensityScale = 1.0f); //�������������� ��� ������ �� ��������, ���� ������ ����������, ����� �����������.
	void deleteHandle(HandleFallLeaves& handle); //���������� � handle ���������� ��������.
	//void setIntensity(HandleFallLeaves& handle, float intensity);
	void setBlow(HandleFallLeaves& handle);

	// virtuals:
	const MatXf& GetPosition() const{return MatXf::ID;}

	void Animate(float dt);
	void PreDraw(cCamera *pCamera);
	void Draw(cCamera *pCamera);
protected:
	void AnimateObjects(cCamera *pCamera);
	void AnimateParticles();
	void initTextures();

	struct TextureAttribute: public std::vector<std::string>{
		TextureAttribute(const char* fileName = "")
		: texture(fileName)
		, scale(1.0f)
		, intensity(1.0f)
		{}

		float intensity;
		float scale;
		std::string texture;

		void serialize(Archive& ar);

		bool operator==(const std::string& rhs) const{
			return texture == rhs;
		}
	};

	struct OneObject{
		c3dx* model;
		/// ������ ��������
		int texture_index;
		/// �������� � �������
		float scale;
		float intensity;
		float intensity_remainder;
		float intensity_blow;
	};

	void updateObjectTexture(OneObject& object);

	enum ParticleState{
		/// �������� ������
		P_BEGIN_FALL,
		/// ������ 
		P_FALL,
		/// ����� �� �����
		P_END_FALL,
	};

	struct OneParticle{
		Vect3f pos;
		ParticleState state;
		int texture_index;
		float alpha;//������������ �������.
		float z_angle, rotate_angle;
		float rotate_speed;
		float scale;
	};

	MTSection objects_lock;
	BackVector<OneObject> objects;
	BackVector<OneParticle> particles;
	cTextureComplex* pTexture;
	float deltatime;

	typedef StaticMap<std::string, int> ModelToTextureIndexMap;
	ModelToTextureIndexMap modelToTextureIndex_;

	typedef std::vector<TextureAttribute> TextureAttributes;
	TextureAttributes textureAttributes_;

	float intensity_;
	float intensityWindMult_;
	float intensityBlow_;

	float rotationSpeedMax_;
};

void fCommandBlowFallLeaves(XBuffer& buffer);

#endif _FALLLEAVES_H_
