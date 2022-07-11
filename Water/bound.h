//2000 Balmer (Poskryakov Dmitry) 
#pragma once

//Bound - ����� ��������������� ��� �������� � ���� ����� 
//		  ������� ��� ������� �������
//Bound - ����� �������� - ����� ������ ������ � �������
//Bound - ������������� ��������� �����

//����������� � vector, ����� �������������, �� ������ ����� �������� �������������� ����.

struct BoundRange
{
	int xmin,xmax;
};

class Bound
{
public:
	typedef vector<BoundRange> vBoundRange;
	virtual void New();
	virtual void Delete();

	//�������� ������� � ������� ������ � ��������
	void CopyLine(int i,//���� ����������
		vBoundRange& from//������ ����������
		);
private:
	void OnLineOr(int ii,vBoundRange& b);

	void OnLineAnd(int ii,vBoundRange& b);

	void CopyLine(int i,//���� ����������
		Bound& b,int j//������ ����������
		);

	void OnLineOrAdd(int ii,vBoundRange& b,int move_x);

	void OnLineClip(int ii,
				  int r_left,int r_right
				  );

	void CopyLinePlus(int i,
		Bound& b,int j,
		int move_x
		);
public:
	vBoundRange* yrange;
	int basey,dy;

	Bound();
	Bound(int _basey,int _dy);
	~Bound();
private:
	Bound(const Bound& ){};
protected:
	bool Realloc(int _basey,int _dy,bool proverka=true);//true - Ok
public:
	vBoundRange temp;
	Bound& operator |=(Bound& bnd);
	Bound& operator &=(Bound& bnd);
	virtual Bound& operator =(Bound& bnd);

	void NewSize(int _basey,int _dy);//���������� ������ � ������� ����� ��������������������
	
	void InitEmpty(int _basey,int _dy);//���������� ������ � ������� ������

	Bound* Not(RECT r);//r - �� ������������ (��. Clip() )
//	Bound& NotAnd(Bound& bnd);

	void Clear();
	//��� ������� ������� Bound
	//	�� ��� � ��� ����, ������ ��� �������� �� �� 
	//	��� ����� Bound �� ������� ������
	//	� �������� �������� �����, �� �� ������� �������� 
	//	� ����� ���������� �������� � ������� ����� ������
	//	������� ������ ������� ������� Bound ���������

	bool IsEmpty();

	enum{COMP_REALLOC=1,COMP_SUM=2};
	void Compress(int flag=COMP_REALLOC|COMP_SUM);
					//���� ���� �������������� ������ � ������ 
					//��� � ����� �������������� ��� ����� COMP_REALLOC
					//���� ���� ����� ����� �������� ��� ������� �����
					//�� ������ �� ��� ���� ����� ������� �������� COMP_SUM

	RECT CalcRect();

	//�����������, �� ���� - ����������������� �������� ��
	//��� ���������� �������� ������� �������� ��������� ��� 
	//��������, ����� ���� �������� �������� Bound � ����� �� �����
	void Clip(RECT r);
	//r - ����� ����� ����������� ������
	//	1 - �� ������������ �� ������� ������ ���
	//	2 - ������ & � Bound ���������� �� ������� r (��. CreateRect)

	void Move(int move_x,int move_y);
	//��������� ������ � ����� �� ����� 
	//�� ���������� move_x,move_y
	
	Bound& OrMove(Bound& b,int move_x,int move_y,RECT r);
	//������� ������� ��� ����� ������������
	//������� �������������� ����� b �� (move_x,move_y)
	//����� ������� ����������� ���������� ��������������� r
	//��� ���� ��������������, ��� �������� ������� ���������
	//�������� � r, ����� ��������� ��������������

	bool IsIn(POINT p);//��������� �� ����� ������?
//	bool In(Bound& b);//��������� �� ������� ������ ����� ��������

//	void Copy(ScrBitmap& b,int x,int y);

	void SetRect(int x1,int y1,int x2,int y2);
	void SetCircle(int x0,int y0,int r);
	void SetEllipse(int x1,int y1,int x2,int y2);

	void AddRect(const RECT& rc);//������ �� �������������

	void Print(FILE* f);
};


void ShowBound(BYTE* pg,int mx,int my,Bound& b,BYTE color);

void AbsoluteShowBound(BYTE* pg,int x,int y,int mx,int my,Bound& b,BYTE color);
void AbsoluteShowBoundAdd(BYTE* pg,int x,int y,int mx,int my,Bound& b,BYTE color);

Bound* CreateRect(int x1,int y1,int x2,int y2);
Bound* CreateCirc(int x0,int y0,int r);
