#include <stdbool.h>
#include <sys/time.h>

typedef struct {
	uint32_t x1;
	uint32_t x2;
	uint32_t y1;
	uint32_t y2;
	uint32_t id;
} TouchPoint;

#define MAXTP 20

typedef struct {
	int _cs;
	uint16_t tpc;
	uint16_t tpx;
	time_t lsec;
	suseconds_t lusec;
	TouchPoint tps[MAXTP];
	bool _calibration;
	int16_t _min_xp; // Minimum xp calibration
	int16_t _min_yp; // Minimum yp calibration
	int16_t _max_xp; // Maximum xp calibration
	int16_t _max_yp; // Maximum yp calibration
	int16_t _min_xc; // Minimum x coordinate
	int16_t _min_yc; // Minimum y coordinate
	int16_t _max_xc; // Maximum x coordinate
	int16_t _max_yc; // Maximum y coordinate
} TouchInfo;

int xptGetit(int cmd);
void xptGetxy(int chipSelect, int *xp, int *yp);
void xptInit(TouchInfo *hoge);
void xptDump(TouchInfo *hoge);
void xptSetPoint(TouchInfo *hoge,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t id);
int xptGetPoint(uint8_t chipSelect, TouchInfo *hoge);
