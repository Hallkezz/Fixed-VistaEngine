#ifndef __SOURCE_FLOCK_H_INCLUDED__
#define __SOURCE_FLOCK_H_INCLUDED__
#include "SourceBase.h"

class Archive;

class Bird;

class SourceFlock : public SourceBase
{
public:

	enum FlockType {
		FLOCK_BIRD,
		FLOCK_FISH
	};

	SourceFlock();
	SourceFlock(const SourceFlock& original);
	SourceFlock* clone() const;

	~SourceFlock();
	SourceType type() const { return SOURCE_FLOCK; }

	void showEditor() const;
	void showDebug() const;
	void quant();

	void serialize(Archive& ar);

	const Rangef& height() const { return height_; }
	const char* modelName() const { return modelName_.c_str(); }
	const char* animationName() const { return animationName_; }
	
	float animationTime() const { return animationPeriod_; }
	const Rangef& modelSize() const { return size_; }
	
	bool hasModel() const { return animationPeriod_ >= 0.f; }
	bool hasAnimation() const { return animationPeriod_ > FLT_EPS; }
	FlockType flockType() const { return flockType_; }

protected:
	void start();
	void stop();

private:
	void release();
	void regenerate();

	void setComboList();

	// ������!!!
	enum{ KNOTS_NUM = 4 };

	struct CenterNode{
		// ������������ ������ �������
		float a_, b_, c_, d_;
		float angle_;
		Vect3f position_;
		void regenerate(const SourceFlock* owner);
		Vect2f getDelta() const;
		CenterNode(){ angle_ = 0.f; };
	};
	typedef std::vector<CenterNode> Knots;
	
	FlockType flockType_;

	// ���� ������ ������� ��������� ����
	Knots knots_;

	// ���������� ������ � ����
	int flockCount_;
	// ������ �������
	Rangef size_;
	// ������ ���� ��� ������
	Rangef height_;
	
	// ��������� [0, 1]
	float flockness_;

	/// ��� ����� 3D ������
	string modelName_;
	/// ��� ������� ��������
	ComboListString animationName_;
	/// ������ �������� � ��������
	float animationPeriod_;

	// ��� �������� �������� ����� �������
	typedef std::vector<ShareHandle<Bird> > Birds;
	Birds birds_;
};

#endif

