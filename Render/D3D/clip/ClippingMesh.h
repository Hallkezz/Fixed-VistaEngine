#ifndef __CLIPPING_MESH_H_INCLUDED__
#define __CLIPPING_MESH_H_INCLUDED__

struct AEdge
{
	int vertex[2];
	set<int> face;
};

struct AFace
{
	set<int> edge;
};

struct AMesh
{
	vector<Vect3f> V;
	vector<AEdge> E;
	vector<AFace> F;

	void CreateABB(Vect3f& vmin,Vect3f& vmax);
};

struct CVertex
{
	Vect3f point;
	float distance;
	int occurs;
	bool visible;

	CVertex(){	distance=0;occurs=0;visible=true;}
};

struct CEdge:public AEdge
{
	bool visible;
	CEdge(){visible=true;}
};

struct CFace:public AFace
{
	bool visible;
	CFace(){visible=true;}
};
/*
struct Plane
{
	float A,B,C,D;

	inline float distance(const Vect3f& p)
	{
		return A*p.x+B*p.y+C*p.z+D;
	}
};
*/
struct APolygons
{
	vector<Vect3f> points;
	vector<vector<int> > faces;
};

///����� ��� �������� ��������� ���������� �������� �����������.
struct CMesh
{
	vector<CVertex> V;
	vector<CEdge> E;
	vector<CFace> F;
public:
	CMesh(AMesh& mesh);	
	CMesh();
	void Set(AMesh& mesh);

	int Clip(sPlane4f clipplane);

	void BuildPolygon(APolygons& p);
protected:
	float epsilon;
	bool GetOpenPolyline(const CFace& face,int& start,int& final);
};

#endif
