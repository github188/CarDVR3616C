#ifndef __BASE_MW_REGION_H__
#define __BASE_MW_REGION_H__

#include "base_comm.h"
#include "base_mw_overlay.h"

#ifdef __cplusplus
	extern "C"{
#endif

/* enum */
typedef enum {
	BASE_MW_REGION_SUCCESS,
	BASE_MW_REGION_FAIL,
} eBASE_MW_REGION_ERROR;

typedef enum{
	BASE_MW_COVER_START = 1,
	BASE_MW_COVER_STOP,
	BASE_MW_COVER_CHANGEATTR,
	BASE_MW_OVERLAY_START ,
	BASE_MW_OVERLAY_STOP,
	BASE_MW_OVERLAY_CHANGEATTR,
	BASE_MW_OVERLAY_SHOWBMP,
}eBASE_MW_REGIOIN_CMD;

typedef enum {
	BASE_MW_REGION_CH0,
	BASE_MW_REGION_CH1,
	BASE_MW_REGION_CH2,
	BASE_MW_REGION_CH_ALL,
} eBASE_MW_REGION_CH;

typedef enum {
	BASE_MW_REGION_NOTUSE,
	BASE_MW_REGION_ISUSE,
	BASE_MW_REGION_ISSTART
} eBASE_MW_REGION_STATE;

typedef enum {
	BASE_MW_REGION_DEFAULT=0x0,
	BASE_MW_REGION_BLACK=0x0,
	BASE_MW_REGION_WHITE=0xffffff,
	BASE_MW_REGION_BLUE=0xff,
	BASE_MW_REGION_GREEN=0xff00,
	BASE_MW_REGION_RED=0xff0000,
} eBASE_MW_REGION_COLOR;

/* struct */
typedef struct {
	LONG lShow;
	LONG lLayer;
	LONG lCmdType;
	eBASE_MW_REGION_COLOR lColor;
	sBASE_COMM_RECT sRegRect;
} sBASE_MW_REGION_ATTR;

typedef struct {
	eBASE_MW_REGION_STATE eState;
	LONG lRegionCh;
	RGN_HANDLE sCovHandle;
	MPP_CHN_S sCoverChn;
	sBASE_MW_REGION_ATTR* psAttr;
} sBASE_MW_REGION;

/* global */
sBASE_MW_REGION gsRegCov[BASE_MW_REGION_CH_ALL];
sBASE_MW_REGION_ATTR gsRegCovAttr[BASE_MW_REGION_CH_ALL];

/* function */
eBASE_MW_REGION_ERROR eBASE_MW_REGION_INIT(sBASE_MW_REGION* psReg, LONG lRegionCh, LONG lViCh);
eBASE_MW_REGION_ERROR eBASE_MW_REGION_START(sBASE_MW_REGION* psReg);
eBASE_MW_REGION_ERROR eBASE_MW_REGION_CHANGE(sBASE_MW_REGION* psReg);
eBASE_MW_REGION_ERROR eBASE_MW_REGION_STOP(sBASE_MW_REGION* psReg);
eBASE_MW_REGION_ERROR eBASE_MW_REGION_GetUpdatedParam(LONG *plPara,LONG* plVench,sBASE_MW_REGION* psReg,sBASE_MW_OVERLAY_ATTR* psOverlayAttr);
eBASE_MW_REGION_ERROR eBASE_MW_REGION_Handle(LONG lVench,sBASE_MW_REGION* psRegCov,sBASE_MW_OVERLAY_ATTR* psOverlayAttr);

#ifdef __cplusplus
	}
#endif

#endif