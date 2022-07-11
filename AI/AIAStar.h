#ifndef __AI_A_STAR_H_INCLUDED__
#define __AI_A_STAR_H_INCLUDED__
//K-D Lab / Balmer

#include <map>
#include <vector>
#include <algorithm>

///////////////////////////AIAStar/////////////////////
//AIAStar::FindPath ����� ���� �� ����� from 
//� ����� IsEndPoint.
//����-����� ���������������� �� ������ ���������� �����

/*
class Heuristic
{
	float GetH(int x,int y);//�������������� ������� �� ����������� �� pos1 � ���������
	float GetG(int x1,int y1,int x2,int y2);//������� �� ����������� �� pos1 � pos2
	bool IsEndPoint(int x,int y);//�������� ������ ���������� �����
	//�� ���� ����� AIAStar ��������� �������� ��������� ����� ��������� ������ ����
};
*/

#define FAST_ERASE 0
//#define HEURISTIC_MAX 0xFFFF // ������������ ��� ����������� ������������ ���
#define HEURISTIC_MAX 0xFFFFFF // ������������ ��� ����������� ������������ ���

//FAST_ERASE==0 ������ ������ - ������ ��������
//FAST_ERASE!=0 ������ ������ - ������ ��������
//�� ������� ������� ����� ���

template<class Heuristic,class TypeH=float>
class AIAStar
{
public:
#if FAST_ERASE
  struct OnePoint;
  typedef multimap<TypeH, OnePoint*> type_point_map;
#else
  typedef multimap<TypeH,POINT> type_point_map;
#endif

	struct OnePoint
	{
		TypeH g;//������� �� ����������� �� ���� �����
		TypeH h;//�������������� ������� �� ����������� �� ������
		DWORD used;
		OnePoint* parent;
		bool is_open;

#if FAST_ERASE
		POINT pt;
		type_point_map::iterator self_it;
#endif

		inline TypeH f(){return g+h;}
	};
protected:
	int dx,dy;
	OnePoint* chart;
	type_point_map open_map;

	DWORD is_used_num;//���� is_used_num==used, �� ������ ������������

	int num_point_examine;//���������� ���������� �����
	int num_find_erase;//������� �������� ������ ������ ��� ��������
	Heuristic* heuristic;
public:
	AIAStar();
	~AIAStar();

	void Init(int dx,int dy);
	bool FindPath(POINT from,Heuristic* h,vector<POINT>& path);
	void GetStatistic(int* num_point_examine,int* num_find_erase);

	//Debug
	OnePoint* GetInternalBuffer(){return chart;};
	DWORD GetUsedNum(){return is_used_num;}
protected:
	void clear();
	inline POINT PosBy(OnePoint* p)
	{
#if FAST_ERASE
		return p->pt;
#else
		int offset=p-chart;
		POINT pos;
		pos.x=offset%dx;
		pos.y=offset/dx;
		return pos;
#endif
	}
};

template<class Heuristic,class TypeH>
AIAStar<Heuristic,TypeH>::AIAStar()
{
	chart=NULL;
	heuristic=NULL;
}

template<class Heuristic,class TypeH>
void AIAStar<Heuristic,TypeH>::Init(int _dx,int _dy)
{
	dx=_dx;dy=_dy;

	int size=dx*dy;
	chart=new OnePoint[size];
	clear();
}

template<class Heuristic,class TypeH>
void AIAStar<Heuristic,TypeH>::clear()
{
	int size=dx*dy;
	is_used_num=0;
	for(int i=0;i<size;i++)
		chart[i].used=0;
}

template<class Heuristic,class TypeH>
AIAStar<Heuristic,TypeH>::~AIAStar()
{
	delete[] chart;
}

template<class Heuristic,class TypeH>
bool AIAStar<Heuristic,TypeH>::FindPath(POINT from,Heuristic* hr,vector<POINT>& path)
{
	num_point_examine=0;
	num_find_erase=0;

	is_used_num++;
	open_map.clear();
	path.clear();
	if(is_used_num==0)
		clear();//��� ����, ����� ��������� ��� �������, ���������� ��������� �����
	xassert(from.x>=0 && from.x<dx && from.y>=0 && from.y<dy);
	heuristic=hr;

	OnePoint* p=chart+from.y*dx+from.x;
	p->g=0;
	p->h=heuristic->GetH(from.x,from.y);
	p->used=is_used_num;
	p->is_open=true;
	p->parent=NULL;

#if FAST_ERASE
	p->pt = from;
	p->self_it = open_map.insert(type_point_map::value_type(p->f(),p));
#else
	open_map.insert(type_point_map::value_type(p->f(),from));
#endif

	const int size_child=8;
	const int sx[size_child]={-1,+1,+1,-1, 0,-1, 0,+1};
	const int sy[size_child]={-1,-1,+1,+1,-1, 0,+1, 0};


	while(!open_map.empty())
	{
		type_point_map::iterator low=open_map.begin();
#if FAST_ERASE
		OnePoint* parent=low->second;
		POINT pt = parent->pt;
#else
		POINT pt=(*low).second;
		OnePoint* parent=chart+pt.y*dx+pt.x;
#endif

		parent->is_open=false;
		open_map.erase(low);

		if(heuristic->IsEndPoint(pt.x,pt.y))
		{
			//��������������� ����
			POINT p;
			while(parent)
			{
				p=PosBy(parent);;
				path.push_back(p);

				if(parent->parent)
				{
					POINT pp;
					pp=PosBy(parent->parent);
					xassert(abs(p.x-pp.x)<=1 && 
						abs(p.y-pp.y)<=1);
				}

				parent=parent->parent;
			}
			xassert(p.x==from.x && p.y==from.y);
			reverse(path.begin(),path.end());
			return true;
		}

		//��� ������� ���������� child ���� parent
		for(int i=0;i<size_child;i++)
		{
			POINT child={pt.x+sx[i],pt.y+sy[i]};
			num_point_examine++;

			if(child.x<0 || child.y<0 || 
			   child.x>=dx || child.y>=dy)continue;
			p=chart+child.y*dx+child.x;


			TypeH addg=heuristic->GetG(pt.x,pt.y,child.x,child.y);
			TypeH newg=parent->g+addg;

			if(p->used==is_used_num)
			{
				if(!p->is_open)continue;
				if(p->g<=newg)continue;

#if FAST_ERASE
				open_map.erase(p->self_it);
#else
				//������� ������� �� open_map
				TypeH f=p->f();
				type_point_map::iterator cur=open_map.find(p->f());
				bool erase=false;
				while(cur!=open_map.end())
				{
					if((*cur).first!=f)break;
					if((*cur).second.x==child.x && (*cur).second.y==child.y)
					{
						open_map.erase(cur);
						erase=true;
						break;
					}
					num_find_erase++;
					cur++;
				}
				num_find_erase++;
				//xassert(erase);
				if(!erase)continue;
#endif
			}

			p->parent=parent;
/*
			{
				POINT pp=PosBy(parent);
				POINT pc=PosBy(p);
				xassert(abs(pc.x-pp.x)<=1 && 
						abs(pc.y-pp.y)<=1);
			}
*/
			p->g=newg;
			p->h=heuristic->GetH(child.x,child.y);

#if FAST_ERASE
			p->pt = child;
			p->self_it = open_map.insert(type_point_map::value_type(p->f(),p));
#else
			open_map.insert(type_point_map::value_type(p->f(),child));
#endif
			p->is_open=true;
			p->used=is_used_num;
		}
	}

	return false;
}

template<class Heuristic,class TypeH>
void AIAStar<Heuristic,TypeH>::GetStatistic(
		int* p_num_point_examine,int* p_num_find_erase)
{
	if(p_num_point_examine)
		*p_num_point_examine=num_point_examine;
	if(p_num_find_erase)
		*p_num_find_erase=num_find_erase;
}

///////////////////////AIAStarGraph/////////////
//AIAStarGraph - ���-�� ����� ����, �� ��������������� �� ����� � ������������ �����
/*
class Node
{
	typedef ... iterator;
	iterator begin();//������ �� ������� ��������� � ���� Node ���.
	iterator end();

	void* AIAStarPointer;//������������ � AIAStarGraph
};

class Heuristic
{
	float GetH(Node* pos);//�������������� ������� �� ����������� �� pos1 � ���������
	float GetG(Node* pos1,Node* pos2);//������� �� ����������� �� pos1 � pos2
	bool IsEndPoint(Node* pos);//�������� ������ ���������� �����
	//�� ���� ����� AIAStar ��������� �������� ��������� ����� ��������� ������ ����
};
*/

template<class Heuristic,class Node,class TypeH=float>
class AIAStarGraph
{
public:
	struct OnePoint;
	typedef multimap<TypeH, OnePoint*> type_point_map;

	struct OnePoint
	{
		TypeH g;//������� �� ����������� �� ���� �����
		TypeH h;//�������������� ������� �� ����������� �� ������
		DWORD used;
		OnePoint* parent;
		bool is_open;

		Node* node;
		typename type_point_map::iterator self_it;

		inline TypeH f(){return g+h;}
	};
protected:
	vector<OnePoint> chart;
	type_point_map open_map;

	DWORD is_used_num;//���� is_used_num==used, �� ������ ������������

	int num_point_examine;//���������� ���������� �����
	int num_find_erase;//������� �������� ������ ������ ��� ��������
	Heuristic* heuristic;
public:
	AIAStarGraph();

	//����� ���������� �����. ���������, ������� �� ������ ��������,
	//���� ���������� �����, ����������� �� ��.
	void Init(vector<Node>& all_node);

	bool FindPath(Node* from,Heuristic* h,vector<Node*>& path);
	void GetStatistic(int* num_point_examine,int* num_find_erase);

	//Debug
	OnePoint* GetInternalBuffer(){return chart;};
	DWORD GetUsedNum(){return is_used_num;}
protected:
	void clear();
	inline Node* PosBy(OnePoint* p)
	{
		return p->node;
	}
};

template<class Heuristic,class Node,class TypeH>
AIAStarGraph<Heuristic,Node,TypeH>::AIAStarGraph()
{
	heuristic=NULL;
}

template<class Heuristic,class Node,class TypeH>
void AIAStarGraph<Heuristic,Node,TypeH>::Init(vector<Node>& all_node)
{
	int size=all_node.size();
	chart.resize(size);

	for(int i=0;i<size;i++)
	{
		OnePoint* c=&chart[i];
		c->node=&all_node[i];
		c->node->AIAStarPointer=(void*)c;
	}
	clear();
}

template<class Heuristic,class Node,class TypeH>
void AIAStarGraph<Heuristic,Node,TypeH>::clear()
{
	is_used_num=0;
	vector<OnePoint>::iterator it;
	FOR_EACH(chart,it)
		it->used=0;
}

template<class Heuristic,class Node,class TypeH>
bool AIAStarGraph<Heuristic,Node,TypeH>::FindPath(Node* from,Heuristic* hr,vector<Node*>& path)
{
	num_point_examine=0;
	num_find_erase=0;

	is_used_num++;
	open_map.clear();
	path.clear();
	if(is_used_num==0)
		clear();//��� ����, ����� ��������� ��� �������, ���������� ��������� �����
	heuristic=hr;

	OnePoint* p=(OnePoint*)from->AIAStarPointer;
	Node* from_node=p->node;
	p->g=0;
	p->h=heuristic->GetH(NULL, p->node);
	p->used=is_used_num;
	p->is_open=true;
	p->parent=NULL;

	p->self_it = open_map.insert(type_point_map::value_type(p->f(),p));

	while(!open_map.empty())
	{
		type_point_map::iterator low=open_map.begin();

		OnePoint* parent=low->second;
		Node* node = parent->node;

		parent->is_open=false;
		open_map.erase(low);

		if(heuristic->IsEndPoint(node))
		{
			bool full_path = true;
			if(parent->g >= HEURISTIC_MAX)
				full_path = false;

			//��������������� ����
			Node* p;
			while(parent)
			{
				p=PosBy(parent);
				xassert(parent->used==is_used_num);

				if(parent->g < HEURISTIC_MAX) path.push_back(p);
				parent=parent->parent;
			}
			xassert(p==from_node);
			reverse(path.begin(),path.end());
			return full_path;
		}

		//��� ������� ���������� child ���� parent
		Node::iterator it;
		FOR_EACH(*node,it)
		{
			Node* cur_node=*it;
			OnePoint* p=(OnePoint*)cur_node->AIAStarPointer; // !!! exception
			num_point_examine++;

			TypeH addg=heuristic->GetG(node,cur_node);
			TypeH newg=parent->g+addg;

			if(p->used==is_used_num)
			{
				if(!p->is_open)continue;
				if(p->g<=newg)continue;

				open_map.erase(p->self_it);
				num_find_erase++;
			}

			p->parent=parent;
			p->g=newg;
			p->h=heuristic->GetH(node,cur_node);

			p->self_it = open_map.insert(type_point_map::value_type(p->f(),p));

			p->is_open=true;
			p->used=is_used_num;
		}
	}

	xassert(!"����� ��������� ������...");
	return false;
}

template<class Heuristic,class Node,class TypeH>
void AIAStarGraph<Heuristic,Node,TypeH>::GetStatistic(
		int* p_num_point_examine,int* p_num_find_erase)
{
	if(p_num_point_examine)
		*p_num_point_examine=num_point_examine;
	if(p_num_find_erase)
		*p_num_find_erase=num_find_erase;
}

///////////////////////AIFindMaxium/////////////

/*
struct Maps
{
	//�������� ���� ������ � ����� (x,y)
	TypeH Get(int x,int y);

	//���������� ������ ����� ����������
	bool IsOptiumGood(TypeH optium,int x,int y);
};
*/

//���� ����������� ��������, �� �� �� ���� �����
template<class Maps>
POINT AIFindMinium(int x,int y,
				  Maps& maps,
				  int dx,int dy)
{
	Maps::TypeH optium=maps.Get(x,y);
	int optiumx=x,optiumy=y;
	
	int maxi=max(max(x,dx-x),max(y,dy-y));
	for(int i=1;i<maxi;i++)
	{
		int curx,cury;
		int xmin=max(0,x-i),xmax=min(dx-1,x+i);
		int ymin=max(0,y-i),ymax=min(dy-1,y+i);
		//up
		cury=y-i;
		if(cury>=0)
		for(curx=xmin;curx<=xmax;curx++)
		{
			Maps::TypeH o=maps.Get(curx,cury);
			if(o<optium)
			{
				optium=o;
				optiumx=curx;
				optiumy=cury;
			}
		}
		
		//down
		cury=y+i;
		if(cury<dy)
		for(curx=xmin;curx<=xmax;curx++)
		{
			Maps::TypeH o=maps.Get(curx,cury);
			if(o<optium)
			{
				optium=o;
				optiumx=curx;
				optiumy=cury;
			}
		}

		//left
		curx=x-i;
		if(curx>=0)
		for(cury=ymin;cury<=ymax;cury++)
		{
			Maps::TypeH o=maps.Get(curx,cury);
			if(o<optium)
			{
				optium=o;
				optiumx=curx;
				optiumy=cury;
			}
		}

		//right
		curx=x+i;
		if(curx<dx)
		for(cury=ymin;cury<=ymax;cury++)
		{
			Maps::TypeH o=maps.Get(curx,cury);
			if(o<optium)
			{
				optium=o;
				optiumx=curx;
				optiumy=cury;
			}
		}

		if(maps.IsOptiumGood(optium,optiumx,optiumy))
			break;
	}

	POINT p={optiumx,optiumy};
	return p;
}

#endif
