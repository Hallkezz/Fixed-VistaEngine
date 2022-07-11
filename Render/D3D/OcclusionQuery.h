#ifndef __OCCLUSION_QUERY_H_INCLUDED__
#define __OCCLUSION_QUERY_H_INCLUDED__

class cCamera;
/*
VisibleCount()
IsVisible()
 ������ ���������� ������ �� ��������� ����. �� ���� �� Test
*/
	  
class cOcclusionQuery
{
	IDirect3DQuery9* pQuery;
	bool draw;
	int testedCount_;
public:
	cOcclusionQuery();
	~cOcclusionQuery();

	bool Init();
	void Done();

	bool IsInit(){return pQuery!=NULL;}
	void Test(const Vect3f& pos);
	void Test(const Vect3f* point, int numPoints);
	void Clear() { draw = false; }

	int TestedCount() const{ return testedCount_; }
	int VisibleCount();
	bool IsVisible();

	void Begin();
	void End();
};

#endif
