#ifndef __OBSERVER_H_INCLUDED__
#define __OBSERVER_H_INCLUDED__

//��������� ��������� ������� � ����� ���������.
//ObserverLink - ��������� ����������.
//Observer - ���� ����������
//� � ��������� ����������� ���������� weak pointer

class Observer;
class ObserverLink
{
public:
	ObserverLink():observer(0){ }
	~ObserverLink();
	virtual void Update()=0;//��������� ���������� �� Observer
protected:
	Observer* observer;
	friend class Observer;

	void SetLink(Observer* o);
	void ClearLink(Observer* o);
};

class Observer
{
	friend class ObserverLink;
	vector<ObserverLink*> links;
public:
	Observer();
	~Observer();

	void AddLink(ObserverLink* link);
	void BreakLink(ObserverLink* link);
	void UpdateLink();//�������� ���������� ObserverLink

	bool empty()const{return links.empty();}
};

#endif
