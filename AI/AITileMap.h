#ifndef __AITILEMAP_H__
#define __AITILEMAP_H__

#include "map2d.h"
#include "ClusterFind.h"

struct AITile
{
	enum { 
		kmGrid_ = 2, // ����������� �������� �� vMap
		tile_size_world_shl = 4, // ������ ����� � ������� �����������
		tile_size = 1 << (tile_size_world_shl - kmGrid_), // ������ ����� � �������� ����� ���������
		tile_area = tile_size*tile_size // ����������
		};

	unsigned char height_min; // ������� ������ ����� 
	int dig_work;	// ������ �� ������������
	bool dig_less; // �� ��������

	bool impassability;
	bool water;
	bool building;

	AITile() : building(false), impassability(false), water(false) {}
	bool update(int x,int y); // returns whether the state (completeness) was changed
	bool completed() const { return !dig_work; } // ���������
};

class AITileMap : public Map2D<AITile, AITile::tile_size_world_shl>
{
public:

	AITileMap(int hsize,int vsize);
	~AITileMap();

	void InitialUpdate();
	void UpdateRect(int x,int y,int dx,int dy); // world coords

	// ��������� ������ 
	void placeBuilding(const Vect2i& v1, const Vect2i& size, bool place); // map coords
	bool readyForBuilding(const Vect2i& v1, const Vect2i& size); // map coords

	//������ � �����
	friend void waterTileChange(int x, int y);

	// ����� ����
	bool findPath(const Vect2i& from, const Vect2i& to, vector<Vect2i>& out_path, AStarTile flags);
	void recalcPathFind();

	// Debug
	void drawWalkMap();
protected:
	
	ClusterFind* path_finder;
	ClusterFind* path_finder2;

	void rebuildWalkMap(AStarTile* walk_map);

	cTexture* pWalkMap;
	void updateWalkMap(AStarTile* walk_map);
};


extern AITileMap* ai_tile_map;

void waterTileChange(int x, int y);

#endif //__AITILEMAP_H__
