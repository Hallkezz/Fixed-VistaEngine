#include "stdafxTr.h"


unsigned int RNDVAL = 83838383;
unsigned int realRNDVAL = 83838383;

inline unsigned RND(unsigned int m)
{
	RNDVAL = RNDVAL * 214013L + 2531011L;

	if(!m) return 0;

	return ((RNDVAL>>16) & 0x7fff) %m;
}

inline unsigned realRND(unsigned int m)
{
	realRNDVAL = realRNDVAL * 214013L + 2531011L;

	if(!m) return 0;

	return ((realRNDVAL>>16)& 0x7fff) %m;
}

#undef random
#define random(num) (int)RND(num)



/* ----------------------------- EXTERN SECTION ---------------------------- */
extern int RestoreLog,CGenLog,BorderLog,MobilityLog,InitLog,GlassLog,WHLog;
//extern int WaterPrm;
//extern int RenderingLayer;
extern int ShotPrm,GlassPrm;
extern int StartshotPrm;
/* --------------------------- PROTOTYPE SECTION --------------------------- */
void r_net_init(void);
void m_net_init(void);
void rough_init(void);
void generate_roughness_map(void);
void map_init(void);
void generate_alt_map(void);

void worldPrepare(void);
void worldClean(void);
void worldFree(void);
void worldInit(void);
void worldRelease(void);
void generate_noise(void);
void head_vmpWrite(XStream& fmap);
void partWrite(XStream& ff,int mode, int Stage);
void worldPreWrite(XStream& ff);

/* --------------------------- DEFINITION SECTION -------------------------- */
static unsigned char GEONET_POWER=6;
static unsigned int WPART_POWER=7;
static int MINSQUARE_POWER=4;
static int GeonetMESH=200;
static int NOISE_AMPL=1;

static unsigned char QUANT =1 << GEONET_POWER;
static unsigned int part_map_size_y = 1 << WPART_POWER;
static int MINSQUARE = 1 << MINSQUARE_POWER;
static unsigned int part_map_size;
static unsigned int net_size;
static unsigned int PART_MAX;


#define COLOR_MAP(y,x)	  (*(color_map + ((y) << vMap.H_SIZE_POWER) + (x)))
#define ALT_MAP(y,x)	  (*(alt_map   + ((y) << vMap.H_SIZE_POWER) + (x)))
#define PTR_ALT_MAP(y,x)  (alt_map   + ((y) << vMap.H_SIZE_POWER) + (x))
#define TEMP_MAP(y,x)	  (*(temp_map  + ((y) << vMap.H_SIZE_POWER) + (x)))
#define R_NET_MAP(y,x)	  r_net_map[((y) << (vMap.H_SIZE_POWER - GEONET_POWER)) + (x)]
#define M_NET_MAP(y,x)	  m_net_map[((y) << (vMap.H_SIZE_POWER - GEONET_POWER)) + (x)]

const int MAX_POWER_Y = 15;
const int MAX_POWER = 10;
const int WPART_POWER_MAX = 7;

const int FLOOD_INIT = 80;

const int CLUSTER_SIZE = 10;
const int CLUSTER_MAXDELTA = 10;
const int CLUSTER_HOLE = 1;

int DEFAULT_TERRAIN = 1;

static unsigned int r_preRNDVAL[MAX_POWER];
static unsigned int m_preRNDVAL[MAX_POWER];
static unsigned int r_cycleRNDVAL[1 << MAX_POWER_Y - WPART_POWER_MAX][MAX_POWER];
static unsigned int m_cycleRNDVAL[1 << MAX_POWER_Y - WPART_POWER_MAX][MAX_POWER];

static unsigned short* alt_map;
static unsigned short* color_map;
static unsigned short* temp_map;
static unsigned short* surf_map;
static unsigned short* proto_map;
static unsigned short* current_map;

//static unsigned short lastlineA[H_SIZE];
//static unsigned short lastlineC[H_SIZE];
static unsigned char lastlineF[MAX_H_SIZE]; //��� �����������
static unsigned short* r_net_map;
static unsigned short* m_net_map;
static unsigned short* proto_m_net_map;
static unsigned short* new_m_net_map;
static unsigned int Stage;

static int geoUsed[1 << MAX_POWER_Y - WPART_POWER_MAX],geoFirst;

static int min_alt,max_alt;
static int Cmin_alt = MIN_VX_HEIGHT;
static int Cmax_alt = MAX_VX_HEIGHT;
static int noiselevel;
static int minsquare;

static int* xx[2];
static int* yy[2];

int Verbose = 1;

unsigned storeRNDVAL,storerealRNDVAL;


void RestrictMinMax(int& v)
{
	if(v > MAX_VX_HEIGHT) v=MAX_VX_HEIGHT;
	else if(v < MIN_VX_HEIGHT) v=MIN_VX_HEIGHT;
}

void r_net_init(void)
{
//	if(Verbose) cout << "RoughNetInit..."<<endl;
	register unsigned int x,y;

	for(x = 0;x < GEONET_POWER; x++) r_preRNDVAL[x] = XRnd(0xFFFFFFFF);
	for(y = 0;y < PART_MAX;y++)
		for(x = 0;x < GEONET_POWER; x++)
			r_cycleRNDVAL[y][x] = XRnd(0xFFFFFFFF);

	for(y = 0;y < vMap.V_SIZE/QUANT;y++){
		for(x = 0;x < vMap.H_SIZE/QUANT;x++)
			switch(WORLD_TYPE){
				case 0:
					if(!XRnd(10))
						R_NET_MAP(y,x) = (max_alt - 1);
					else
						R_NET_MAP(y,x) = (unsigned short)((max_alt - 1)*(x + y)/(vMap.H_SIZE + vMap.V_SIZE));
					break;
				case 1:
//					  R_NET_MAP(y,x) = (unsigned char)((max_alt - 1)*(x + y)/(H_SIZE + V_SIZE));
					R_NET_MAP(y,x) = (unsigned short)XRnd(max_alt - 1);
					break;
				case 2:
//					  R_NET_MAP(y,x) = (unsigned char)((max_alt - 1)*((x + y)*QUANT)/abs(x*QUANT - H_SIZE/2 - 1));
					R_NET_MAP(y,x) = (unsigned short)XRnd(max_alt - 1);
					break;
				case 3:
//					  R_NET_MAP(y,x) = (unsigned char)((max_alt - 1)*((x + y)*QUANT)/abs(x*QUANT - H_SIZE/2 - 1));
					R_NET_MAP(y,x) = MIN_VX_HEIGHT + (unsigned short)XRnd(MAX_VX_HEIGHT - MIN_VX_HEIGHT);
					break;
				}
		}
}

void m_net_init(void)
{
//	if(Verbose) cout << "MapNetInit..."<<endl;
	register unsigned int x,y;

	for(x = 0;x < GEONET_POWER; x++) m_preRNDVAL[x] = XRnd(0xFFFFFFFF);
	for(y = 0;y < PART_MAX; y++)
		for(x = 0;x < GEONET_POWER; x++)
			m_cycleRNDVAL[y][x] = XRnd(0xFFFFFFFF);

	for(y = 0;y < vMap.V_SIZE/QUANT;y++){
		for(x = 0;x < vMap.H_SIZE/QUANT;x++)
			M_NET_MAP(y,x) = MIN_VX_HEIGHT + (unsigned short)XRnd(MAX_VX_HEIGHT - MIN_VX_HEIGHT);
		}
}

void rough_init(void)
{
//	if(Verbose) cout << "RoughInit..."<<endl;
	register unsigned int x,y,i,j;

	for(y = 0,j = Stage*(part_map_size_y/QUANT);y <= part_map_size_y + QUANT;y += QUANT,j++){
		if(j == PART_MAX*(part_map_size_y/QUANT)) j = 0;
		for(x = 0,i = 0;x < vMap.H_SIZE;x += QUANT,i++)
			COLOR_MAP(y,x) = R_NET_MAP(j,i);
		}
}

void generate_roughness_map(void)
{
	if(MINSQUARE > 1){
		RNDVAL = r_cycleRNDVAL[Stage][0];
		register unsigned int i;
		unsigned short* pc = color_map;
		unsigned short* pa = alt_map;
		for(i = 0;i < part_map_size + vMap.H_SIZE;i++){
			*pc++ = random(noiselevel);
			*pa++ = random(noiselevel);
			}
		}

	rough_init();

//	if(Verbose) cout << "GenerateRoughnessMap..."<<endl;

	int which = 0;
	int square_size,x1,y1,x2,y2;
	int p1,p2,p3,p4;
	int random_center,random_range,ind;
	int i1,i2,i3,i4;
	int i1_9,i2_9,i3_9,i4_9;
	int i1_3,i2_3,i3_3,i4_3;

	for(ind = 0,square_size = QUANT;square_size > MINSQUARE;square_size >>= 1,ind++){
		RNDVAL = r_preRNDVAL[ind];
		for(y1 = 0;y1 <= (int)part_map_size_y;y1 += square_size){
			if(y1 == square_size) RNDVAL = r_cycleRNDVAL[Stage][ind];
			else if(y1 == (int)part_map_size_y) RNDVAL = r_preRNDVAL[ind];
			for(x1 = 0;x1 < vMap.H_SIZE;x1 += square_size){
				x2 = vMap.XCYCL(x1 + square_size);
				y2 = y1 + square_size;
				if(!which){
					i1 = COLOR_MAP(y1,x1);
					i2 = COLOR_MAP(y1,x2);
					i3 = COLOR_MAP(y2,x1);
					i4 = COLOR_MAP(y2,x2);
					}
				else {
					i1 = ALT_MAP(y1,x1);
					i2 = ALT_MAP(y1,x2);
					i3 = ALT_MAP(y2,x1);
					i4 = ALT_MAP(y2,x2);
					}
				i1_3 = i1*3; i1_9 = i1_3*3;
				i2_3 = i2*3; i2_9 = i2_3*3;
				i3_3 = i3*3; i3_9 = i3_3*3;
				i4_3 = i4*3; i4_9 = i4_3*3;
				p1 = (i1_9 + i2_3 + i3_3 + i4) >> 4;
				p2 = (i1_3 + i2_9 + i3 + i4_3) >> 4;
				p3 = (i1_3 + i2 + i3_9 + i4_3) >> 4;
				p4 = (i1 + i2_3 + i3_3 + i4_9) >> 4;

				random_center = square_size<<(VX_FRACTION); ///?
				random_range = random_center << 1;
				if(BIZARRE_ROUGHNESS_MAP){
					p1 += random(random_center) + random_range - (Cmax_alt - p1)/6;
					p2 += random(random_center) + random_range - (Cmax_alt - p2)/6;
					p3 += random(random_center) + random_range - (Cmax_alt - p3)/6;
					p4 += random(random_center) + random_range - (Cmax_alt - p4)/6;

					if(p1 < Cmin_alt) p1 += Cmin_alt + 1; else if(p1 > Cmax_alt) p1 %= Cmax_alt + 1;
					if(p2 < Cmin_alt) p2 += Cmin_alt + 1; else if(p2 > Cmax_alt) p2 %= Cmax_alt + 1;
					if(p3 < Cmin_alt) p3 += Cmin_alt + 1; else if(p3 > Cmax_alt) p3 %= Cmax_alt + 1;
					if(p4 < Cmin_alt) p4 += Cmin_alt + 1; else if(p4 > Cmax_alt) p4 %= Cmax_alt + 1;
					}
				else {
					p1 += random(random_range) - random_center;
					p2 += random(random_range) - random_center;
					p3 += random(random_range) - random_center;
					p4 += random(random_range) - random_center;

					if(p1 < Cmin_alt) p1 = Cmin_alt; else if(p1 > Cmax_alt) p1 = Cmax_alt;
					if(p2 < Cmin_alt) p2 = Cmin_alt; else if(p2 > Cmax_alt) p2 = Cmax_alt;
					if(p3 < Cmin_alt) p3 = Cmin_alt; else if(p3 > Cmax_alt) p3 = Cmax_alt;
					if(p4 < Cmin_alt) p4 = Cmin_alt; else if(p4 > Cmax_alt) p4 = Cmax_alt;
					}
				x2 = vMap.XCYCL(x1 + (square_size >> 1));
				y2 = y1 + (square_size >> 1);

				if(!which){
					ALT_MAP(y1,x1) = (unsigned short)p1;
					ALT_MAP(y1,x2) = (unsigned short)p2;
					ALT_MAP(y2,x1) = (unsigned short)p3;
					ALT_MAP(y2,x2) = (unsigned short)p4;
					}
				else {
					COLOR_MAP(y1,x1) = (unsigned short)p1;
					COLOR_MAP(y1,x2) = (unsigned short)p2;
					COLOR_MAP(y2,x1) = (unsigned short)p3;
					COLOR_MAP(y2,x2) = (unsigned short)p4;
					}
				}
			}
		which = 1 - which;
		}

	if(!which)
		memcpy(temp_map,color_map,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );
	else
		memcpy(temp_map,alt_map,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );

		//memset(temp_map,0,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );
		//memset(temp_map,0,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );
		//memset(temp_map,0,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );
}

void map_init(void)
{
//	if(Verbose) cout << "MapInit..."<<endl;
	register unsigned int i,j,x,y;

	for(y = 0,j = Stage*(part_map_size_y/QUANT);y <= part_map_size_y + QUANT;y += QUANT,j++){
		if(j == PART_MAX*(part_map_size_y/QUANT)) j = 0;
		for(x = 0,i = 0;x < vMap.H_SIZE;x += QUANT,i++)
			COLOR_MAP(y,x) = M_NET_MAP(j,i);
		}
}

void generate_alt_map(void)
{
	int which = 0;
	int square_size,x1,y1;
	int x2,y2;
	int p1,p2,p3,p4;
	int random_center,random_range,ind;
	int i1,i2,i3,i4;
	int i1_9,i2_9,i3_9,i4_9;
	int i1_3,i2_3,i3_3,i4_3;

	generate_roughness_map();
	map_init();

//	if(Verbose) cout << "GenerateAltMap..."<<endl;

	for(ind = 0,square_size = QUANT;square_size > 1;square_size >>= 1,ind++){
		RNDVAL = m_preRNDVAL[ind];
		for(y1 = 0;y1 <= (int)part_map_size_y;y1 += square_size){
			if(y1 == square_size) RNDVAL = m_cycleRNDVAL[Stage][ind];
			else if(y1 == (int)part_map_size_y) RNDVAL = m_preRNDVAL[ind];
			for(x1 = 0;x1 < vMap.H_SIZE;x1 += square_size){
				x2 = vMap.XCYCL(x1 + square_size);
				y2 = y1 + square_size;
				if(!which){
					i1 = COLOR_MAP(y1,x1);
					i2 = COLOR_MAP(y1,x2);
					i3 = COLOR_MAP(y2,x1);
					i4 = COLOR_MAP(y2,x2);
					}
				else {
					i1 = ALT_MAP(y1,x1);
					i2 = ALT_MAP(y1,x2);
					i3 = ALT_MAP(y2,x1);
					i4 = ALT_MAP(y2,x2);
					}
				i1_3 = i1*3; i1_9 = i1_3*3;
				i2_3 = i2*3; i2_9 = i2_3*3;
				i3_3 = i3*3; i3_9 = i3_3*3;
				i4_3 = i4*3; i4_9 = i4_3*3;
				p1 = (i1_9 + i2_3 + i3_3 + i4) >> 4;
				p2 = (i1_3 + i2_9 + i3 + i4_3) >> 4;
				p3 = (i1_3 + i2 + i3_9 + i4_3) >> 4;
				p4 = (i1 + i2_3 + i3_3 + i4_9) >> 4;
				random_center = square_size*TEMP_MAP(y1,x1)/GeonetMESH;
				random_range = random_center << 1;
				if(BIZARRE_ALT_MAP){
					p1 += (random(random_range) - random_center + p1/4);
					p2 += (random(random_range) - random_center + p2/4);
					p3 += (random(random_range) - random_center + p3/4);
					p4 += (random(random_range) - random_center + p4/4);

					if(p1 < Cmin_alt) p1 = Cmin_alt; else if(p1 > Cmax_alt) p1 -= Cmax_alt + 1;
					if(p2 < Cmin_alt) p2 = Cmin_alt; else if(p2 > Cmax_alt) p2 -= Cmax_alt + 1;
					if(p3 < Cmin_alt) p3 = Cmin_alt; else if(p3 > Cmax_alt) p3 -= Cmax_alt + 1;
					if(p4 < Cmin_alt) p4 = Cmin_alt; else if(p4 > Cmax_alt) p4 -= Cmax_alt + 1;
					}
				else {
					p1 += (random(random_range) - random_center);
					p2 += (random(random_range) - random_center);
					p3 += (random(random_range) - random_center);
					p4 += (random(random_range) - random_center);

					if(p1 < Cmin_alt) p1 = Cmin_alt; else if(p1 > Cmax_alt) p1 = Cmax_alt;
					if(p2 < Cmin_alt) p2 = Cmin_alt; else if(p2 > Cmax_alt) p2 = Cmax_alt;
					if(p3 < Cmin_alt) p3 = Cmin_alt; else if(p3 > Cmax_alt) p3 = Cmax_alt;
					if(p4 < Cmin_alt) p4 = Cmin_alt; else if(p4 > Cmax_alt) p4 = Cmax_alt;
					}
				x2 = vMap.XCYCL(x1 + (square_size >> 1));
				y2 = y1 + (square_size >> 1);

				if(!which){
					ALT_MAP(y1,x1) = (unsigned short)p1;
					ALT_MAP(y1,x2) = (unsigned short)p2;
					ALT_MAP(y2,x1) = (unsigned short)p3;
					ALT_MAP(y2,x2) = (unsigned short)p4;
					}
				else {
					COLOR_MAP(y1,x1) = (unsigned short)p1;
					COLOR_MAP(y1,x2) = (unsigned short)p2;
					COLOR_MAP(y2,x1) = (unsigned short)p3;
					COLOR_MAP(y2,x2) = (unsigned short)p4;
					}
				}
			}
		which = 1 - which;
		}

	if(!which) memcpy(alt_map,color_map,part_map_size*sizeof(unsigned short));
}
//static 
void vrtMap::calc_Flood(int x0,int y0,int level,int ttype,int ti)
{
	const int DriftNoise = 7;
	const int DriftPower = 2;

	const int N = 1024 << 6;

	if(!xx[0]){
		xx[0] = new int[N];
		xx[1] = new int[N];
		yy[0] = new int[N];
		yy[1] = new int[N];
		}

	int index = 0;
	int num[2];
	xx[0][0] = x0;
	yy[0][0] = y0;
	num[0] = 1;

//	unsigned char** lt = vMap -> lineT;
	unsigned char *pf;//* pv,
	int na,h,t;

	int log = 1,i,j,x,y;
	while(log){
		log = 0;
		for(j = 0,i = 0;i < num[index];i++){
			y = yy[index][i];
			pf = &(SurBuf[(y)*H_SIZE]);
			{ //if(pv){
				//pv += (x = xx[index][i]);
				x = xx[index][i];
				pf += (x);
				na = level;
				h = GetAlt(x,y);//*pv;
				if(ti) na += (level - h)*DriftPower/16 + XRnd(DriftNoise);
				t = *pf;
				if(h < level && t != ttype && j < N - 4 ){ //&& ActiveTerrain[t]
					*pf = ttype; log=1;

					if(ti) PutAlt(x,y,na);//*pv = (unsigned char)na;

					xx[1 - index][j] = (x - 1) & clip_mask_x;
					yy[1 - index][j] = y;
					j++;
					xx[1 - index][j] = (x + 1) & clip_mask_x;
					yy[1 - index][j] = y;
					j++;
					xx[1 - index][j] = x;
					yy[1 - index][j] = (y - 1) & clip_mask_y;
					j++;
					xx[1 - index][j] = x;
					yy[1 - index][j] = (y + 1) & clip_mask_y;
					j++;
				}
			}
		}
		index = 1 - index;
		num[index] = j;
	}
}



void vrtMap::WORLD_Drift(int x0,int y0,int level,int ttype)
{
		calc_Flood(x0,y0,level,(MAX_SURFACE_TYPE - 1));
		calc_Flood(x0,y0,level,ttype,1);
}


//static void calc_color_map(void)
//{
//	if(Verbose) cout << "CalcColorMap..."<<endl;
//	WORLD_colcalc(Stage << WPART_POWER,vMap.YCYCL(((Stage + 1) << WPART_POWER)),0);
//}



void worldPrepare(void)
{
	if(r_net_map) return;

	part_map_size = vMap.H_SIZE << WPART_POWER;
	PART_MAX = vMap.V_SIZE >> WPART_POWER;
	net_size = vMap.H_SIZE*vMap.V_SIZE/QUANT/QUANT;

	r_net_map = new unsigned short [net_size];
	m_net_map = new unsigned short [net_size];
	proto_m_net_map = new unsigned short [net_size];
	new_m_net_map = new unsigned short [net_size];
}

void worldFree(void)
{
	if(r_net_map) {
		delete r_net_map;
		delete m_net_map;
		delete proto_m_net_map;
		delete new_m_net_map;
	}
	r_net_map = 0;
}


void worldInit(void)
{
//	if(Verbose) cout << "Memory allocation..." <<endl;

	alt_map = new unsigned short[part_map_size + (QUANT + 1)*vMap.H_SIZE];
	color_map = new unsigned short[part_map_size + (QUANT + 1)*vMap.H_SIZE];
	temp_map = new unsigned short[part_map_size + (QUANT + 1)*vMap.H_SIZE];
	surf_map = new unsigned short[part_map_size + vMap.H_SIZE];
	proto_map = new unsigned short[part_map_size];
	current_map = new unsigned short[part_map_size];

	worldClean();
}

void worldClean(void)
{
//	if(Verbose) cout << "Memory cleaning..."<<endl;
	memset(alt_map,0,(part_map_size + (QUANT + 1)*vMap.H_SIZE)*sizeof(unsigned short) );
	memset(color_map,0,(part_map_size + (QUANT + 1)*vMap.H_SIZE)*sizeof(unsigned short) );
	memset(temp_map,0,(part_map_size + (QUANT + 1)*vMap.H_SIZE)*sizeof(unsigned short) );
	memset(surf_map,0,(part_map_size + vMap.H_SIZE)*sizeof(unsigned short) );
	memset(proto_map,0,(part_map_size)*sizeof(unsigned short) );
	memset(current_map,0,(part_map_size)*sizeof(unsigned short) );

	//memset(lastlineA,0,vMap.H_SIZE); 
	//memset(lastlineC,0,vMap.H_SIZE);
}

void worldRelease(void)
{
	delete alt_map;
	delete color_map;
	delete temp_map;
	delete surf_map;
	delete proto_map;
	delete current_map;
	alt_map = color_map = temp_map = NULL;
}

void generate_noise(void)
{
	unsigned short* pa = alt_map;
	register int i,j;
	for(i = 0;i < (int)part_map_size_y;i++)
		for(j = 0;j < vMap.H_SIZE;j++,pa++)
			*pa += XRnd(NOISE_AMPL);
}

void head_vmpWrite(XStream& fmap)
{
	vrtMap::sVmpHeader VmpHeader;
	//const char id[4]={'S','2','T','0'};
	//int i;
	fmap.seek(0,XS_BEG);
	//*(int*)VmpHeader.id = *(int*)id;
	VmpHeader.setID("S2T0");
	//VmpHeader.XS=2048;
	//VmpHeader.YS=2048;
	fmap.write(&VmpHeader,sizeof(VmpHeader));
}

void partWrite(XStream& ff,int mode, int Stage)
{
	vrtMap::sVmpHeader VmpHeader;
	register int i,j;
	unsigned short* pa = alt_map;
	unsigned short* pf = surf_map;
	unsigned char * pw;
	pw = new unsigned char [vMap.H_SIZE];
	if(!mode)
		for(i = 0;i < (int)part_map_size_y;i++,pa += vMap.H_SIZE){
			for(j=0; j<vMap.H_SIZE; j++) {
				pw[j]=pa[j]>>VX_FRACTION;
			}
			ff.seek(sizeof(VmpHeader)+(Stage*(int)part_map_size_y*vMap.H_SIZE)+i*vMap.H_SIZE,XS_BEG);
			//������ � VxGBuf
			ff.write(pw, vMap.H_SIZE*sizeof(unsigned char));
			for(j=0; j<vMap.H_SIZE; j++) {
				pw[j]=pa[j]&VX_FRACTION_MASK;
			}
			ff.seek(sizeof(VmpHeader)+(Stage*(int)part_map_size_y*vMap.H_SIZE)+i*vMap.H_SIZE+ vMap.H_SIZE*vMap.V_SIZE*2,XS_BEG); //����� ���������, ��� ���������� ������� ����� ���� 3-�
			//������ � ����� ���������
			ff.write(pw, vMap.H_SIZE*sizeof(unsigned char));

		}
}

void worldPreWrite(XStream& ff) //attribute
{
	int i;
	memset(lastlineF,0,vMap.H_SIZE);

	//������ Geo Surface
	for(i = 0;i < vMap.V_SIZE;i++){
		ff.write(lastlineF,vMap.H_SIZE);
	}
	//������ Dam Surface
	for(i = 0;i < vMap.V_SIZE;i++){
		ff.write(lastlineF,vMap.H_SIZE);
	}

	//������ AtrBuf
	memset(lastlineF,0,vMap.H_SIZE);
	for(i = 0;i < vMap.V_SIZE;i++){
		ff.write(lastlineF,vMap.H_SIZE);
	}

	//������ RnrBuf 
	memset(lastlineF,DEFAULT_TERRAIN,vMap.H_SIZE);
	for(i = 0;i < vMap.V_SIZE;i++){
		ff.write(lastlineF,vMap.H_SIZE);
	}
}


int vrtMap::buildMountainWorld(void)
{
	worldPrepare();

	if(!alt_map){
	       worldInit();
		//first = 1;
	}

	storeRNDVAL = RNDVAL;
	storerealRNDVAL = realRNDVAL;

	XStream ff;

	r_net_init();
	m_net_init();

	ff.open(getTargetName(vrtMap::worldRasterDataFileSource),XS_OUT);
	head_vmpWrite(ff);
	worldPreWrite(ff);
	int off = ff.tell();
	for(Stage = 0;Stage < PART_MAX;Stage++){
//		if(Verbose) cout <<endl<< "------------- Part: " << Stage << " -------------" <<endl;
		//��������� ���������� ������������� Stag-�(���� ���������)
		min_alt = MIN_VX_HEIGHT;
		max_alt = MAX_VX_HEIGHT;
		noiselevel = NOISELEVEL;
		minsquare = MINSQUARE;

		generate_alt_map();
		generate_noise();
		partWrite(ff,0,Stage);
	}
	ff.close();

//	cout <<endl << "Recoloring in progress..." <<endl;

	worldRelease();

	worldFree();
	return 1;
}

/*
void vrtMap::GeoRecalc(int n, int level, int delta)
{
	//delta ���������� ��� ������������(���������� � ��������� 0-1) ������� ����� ����������� � �� ����������� ����� ������
	int r_delta=delta; //if(r_delta==0)r_delta=1; //if (absdelta<0)absdelta=-absdelta; 
	int y0 = n << WPART_POWER;
	int y1 = (((n + 1) << WPART_POWER) - 1) & clip_mask_y;

	Stage = n;
	SetPP(n);


	register int i,j;
	int y1m = (y1 + 1) & clip_mask_y;
	//���� ������ �� ����� 0 
	if(r_delta!=0){
		//1-� ������: ��������� ���������������  � ������ �������� ��� ����(�������� GeonetMESH=1000)
		memcpy(m_net_map,proto_m_net_map,net_size*sizeof(unsigned short));//����������� ������ ����� ����� � ������� �����
		int archivGeonetMESH=GeonetMESH;
		GeonetMESH=1000; //���������� ������ � ������ ���������� ����
		generate_alt_map(); //��������� ��������������� �������� //�� ����� m_net_map ��������� ����������� � alt_map
		memcpy(proto_map,alt_map,part_map_size*sizeof(unsigned short)); //���������� ��������� � proto_map 

		memcpy(m_net_map,new_m_net_map,net_size*sizeof(unsigned short));//����������� ����� ����� ����� � ������� �����
		generate_alt_map(); //��������� ������ ��������

		//������� �������� ��������� ���������
		unsigned short* pa = alt_map;
		unsigned short* pp = proto_map;
		short* pcm = (short*)current_map; //���������� ��������� ���� ��������
		for(i = y0;i != y1m;i = (i + 1) & clip_mask_y){
			for(j = 0;j < H_SIZE;j++,pa++,pp++,pcm++){
				*pcm=(int)*pa-(int)*pp;
			}
		}
		GeonetMESH=archivGeonetMESH; //�������������� ��������������� ����
		memcpy(proto_map,alt_map,part_map_size*sizeof(unsigned short));//���������� �� ����������� ����� �����������
	//////
		//memcpy(m_net_map,proto_m_net_map,net_size*sizeof(unsigned short));
		//generate_alt_map();
		//memcpy(proto_map,alt_map,part_map_size*sizeof(unsigned short));

		//2-� ������: ��������� ������ ��������� � �����(�������� MESH ������������)

		memcpy(m_net_map,new_m_net_map,net_size*sizeof(unsigned short));//����������� ����� ����� ����� � ������� �����(����� �� ������)
		generate_alt_map(); //��������� ������ ��������� � �����

		unsigned char* ch = changedT;
		pa = alt_map;
		pp = proto_map;
		pcm = (short*)current_map;
		for(i = y0;i != y1m;i = (i + 1) & clip_mask_y){
			for(j = 0;j < H_SIZE;j++,pa++,pp++,pcm++){//p = lt[i],//,p++ ///!pc++,
				//��������� ������� �����-pcm; ��������� ���������������(*pcm/r_delta) ��� ��������� �� ����� ��� ("*pa-*pp")
				if(*pcm) voxSet(j,i,( (int)*pa -(int)*pp )*((int)*pcm)/r_delta + (int)*pcm );
				//if(*pa != *pp) pixSet(j,i,((int)*pa - GetAlt(j,i))*((int)*pa - (int)*pp)/absdelta );
				//pixSet(j,i,(int)*pa - (int)*pp);
			}
			ch[i] = 1;
		}
	}
	else {//���� delta==0
	//////
		//1-� ������: ��������� ������� ��������� � �����
		memcpy(m_net_map,proto_m_net_map,net_size*sizeof(unsigned short));
		generate_alt_map();
		memcpy(proto_map,alt_map,part_map_size*sizeof(unsigned short));

		//2-� ������: ��������� ������ ��������� � �����

		memcpy(m_net_map,new_m_net_map,net_size*sizeof(unsigned short));//����������� ����� ����� ����� � ������� �����(����� �� ������)
		generate_alt_map(); //��������� ������ ��������� � �����

		unsigned char* ch = changedT;
		unsigned short* pa = alt_map;
		unsigned short* pp = proto_map;
		for(i = y0;i != y1m;i = (i + 1) & clip_mask_y){
			for(j = 0;j < H_SIZE;j++,pa++,pp++){//p = lt[i],//,p++ ///!pc++,
				//��������� ������� �����-pcm; ��������� ���������������(*pcm/r_delta) ��� ��������� �� ����� ��� ("*pa-*pp")
				//if(*pcm) pixSet(j,i,( (int)*pa -(int)*pp )*((int)*pcm)/r_delta + (int)*pcm );
				//if(*pa != *pp) pixSet(j,i,((int)*pa - GetAlt(j,i))*((int)*pa - (int)*pp)/absdelta );
				voxSet(j,i,(int)*pa - (int)*pp);
			}
			ch[i] = 1;
		} 

	}
}

void vrtMap::GeoPoint(int x,int y,int level,int delta,int mode)
{
	int v,vm;
	switch(mode){
		case 0:
			//� ������� ����� ������ ��������� ������
			if(!(x%QUANT) && !(y%QUANT)){ //�������� �� ��������� � ������� �����
				v = ((y - 3*QUANT) & clip_mask_y) >> WPART_POWER;
				vm = (1 + (((y + 3*QUANT) & clip_mask_y) >> WPART_POWER)) & (PART_MAX - 1);
				for(;v != vm;v = (v + 1) & (PART_MAX - 1)) geoUsed[v] = 1;
				x >>= GEONET_POWER; //���������� � ����������� �����
				y >>= GEONET_POWER;
				if(!delta)
					v = level;
				else {
					v = M_NET_MAP(y,x);
					v += delta;
					RestrictMinMax(v);// ����������� �� min-max
					//if(delta > 0){ if(v > 255) v = 255; }
					//else if(v < 0) v = 0;
					}
				M_NET_MAP(y,x) = (unsigned short)v; //��������� � ����� ����� ����� ������
				}
			break;
		case 1:
			// �������������� ������, ��������� �������������� ����� � proto_m_net_map
			worldInit();
			for(v = 0;v < (int)PART_MAX;v++) geoUsed[v] = 0;
			geoFirst = 0;
			memcpy(proto_m_net_map,m_net_map,net_size*sizeof(unsigned short));
			break;
		case 2:
			//��������� ����� ����� ����� � new_m_net_map
			memcpy(new_m_net_map,m_net_map,net_size*sizeof(unsigned short));
			v = geoFirst;
			//� ������ ���� � ������� ���������� ����� ����� �������� ��������� ���������
			do {
				if(geoUsed[v]) GeoRecalc(v,level,delta);
				v = (v + 1) & (PART_MAX - 1);
				} while(v != geoFirst);
			//������������ �������
			worldRelease();
			break;
		}
}
*/
/*
void CUTVMP(void)
{
	const int LEN = 13;
	const int X0 = 1600;
	const int Y0 = 700;

	cout << "CUTVMP running..."<<endl;

	char* name = strdup(vMap -> fname);
	memcpy(name + strlen(name) - 10,"cutvmp",6);
	XStream ff(name,XS_IN | XS_OUT);
	unsigned char* buf = new unsigned char[512];
	ff.read(buf,512);
	
	int h_size = 1 << *(buf + LEN);
	int v_size = 1 << *(buf + LEN + 1);

	delete buf;
	buf = new unsigned char[2*h_size];

	vMap -> openMirror();
	int y = vMap.YCYCL(Y0 - v_size/2);

	register int i,j,x;
	unsigned char* pa,*pf,*p;
	unsigned char** lt = vMap -> lineT;
	for(j = 0;j < v_size;j++,y = vMap.YCYCL(y + 1)){
		x = vMap.XCYCL(X0 - h_size/2);
		pa = lt[y] + x; pf = pa + vMap.H_SIZE;
		p = buf;
		for(i = 0;i < h_size;i++,x = vMap.XCYCL(x + 1),p++,pa++,pf++){
			if(!x){ pa = lt[y]; pf = pa + vMap.H_SIZE; }
			*p = *pa; *(p + h_size) = *pf;
			}
		ff.write(buf,2*h_size);
		}

	vMap -> closeMirror();
}*/
