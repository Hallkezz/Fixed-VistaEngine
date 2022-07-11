#ifndef __T_E_GRID_H_INCLUDED__
#define __T_E_GRID_H_INCLUDED__

class TEGrid  
{
public:
	TEGrid();
	TEGrid(int cellWidth, int cellHeight, int amongCellWidth, int amongCellHeight);
	~TEGrid();

	int getCellWidth() const;
	int getCellHeight() const;

	void setCellWidth(int value);
	void setCellHeight(int value);

	int getAmongCellsW() const;
	int getAmongCellsH() const;


	void setAmongCellsW(int value);
	void setAmongCellsH(int value);

	//! ���������� �������������� ������ ������, ���������� ����� � ����������� �
	int getHorzCellIndex(int x) const;
	//! ���������� ������������ ������ ������, ���������� ����� � ����������� y
	int getVertCellIndex(int y) const;
	//! ���������� ������� ������, � ������� �������� �����
	CPoint const getCellByPoint(POINT const& p) const;
	//! ���������� ���������� �������� ����� ������, ���������� ����� � ����������� y
	int toCellTop(int y) const;
	//! ���������� ���������� ������ ����� ������, ���������� ����� � ����������� x
	int toCellLeft(int x) const;
	//! ���������� ����� ������� ���� ������ �� �����
	void toCellLeftTop(POINT * point) const;
	
	//! ���������� ���������� �������� ����� ������ �� �������
	int getCellTopByIndex(int idx) const;
	//! ���������� ���������� ������ ����� ������ �� �������
	int getCellLeftByIndex(int idx) const;

	//! ���������� ������ ������ ������(������ �������� + ���������� ����� ���������)
	int getFullCellWidth() const;
	//! ���������� ������ ������ ������(������ �������� + ���������� ����� ���������)
	int getFullCellHeight() const;
protected:
	static int toCellEdge(int x, int cell_sz, int among_cells_sz);
	static int getCellIndex(int x, int cell_sz);
	static int indexToCellEdge(int idx, int cell_sz);

private:
	//! ������ ��������������
	int iCellHeight_;
	//! ������ ��������������
	int iCellWidth_;
	//! ���������� ����� ���������� �� ������
	int iAmongCellsH_;
	//! ���������� ����� ���������� �� ������
	int iAmongCellsW_;
};

#endif
