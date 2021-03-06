#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************
	include
**********************************************************************************/
#include "base_mw_osd.h"
#include "base_mw_osd_ascii16x24.h"
#include "base_loadbmp.h"
#include "base_mw_boardinterface.h"

/*********************************************************************************
	macro
**********************************************************************************/
#define BASE_MW_OSD_ALPHT_SHIFT 15
#define BASE_MW_OSD_R_SHIFT 10
#define BASE_MW_OSD_G_SHIFT 5
#define BASE_MW_OSD_B_SHIFT 0

//#define OSD_DEBUG
#ifdef OSD_DEBUG
#define debug_log(fmt, args...) fprintf(stdout, "[ OSD MW ] : "fmt, ##args)
#define debug_log_funcIn() fprintf(stdout, "in %s\n", __func__)
#define debug_log_funcOut() fprintf(stdout, "out %s\n", __func__)
#define debug_log_showErr(err) fprintf(stderr, "%s <%d> : Error <%d>\n", __func__, __LINE__, err)
#define showval(x) printf("%s : <%#lx> @ <%p>\n", #x, x, &x)
#else
#define debug_log(fmt, args...)
#define debug_log_funcIn()
#define debug_log_funcOut()
#define debug_log_showErr(err)
#define showval(x)
#endif
/*********************************************************************************
	variable
**********************************************************************************/
/*bit 4-0 */
const UCHAR cOSDColorRed[]	= {0x1f,0,0};
const UCHAR cOSDColorGreen[]	= {0,0x1f,0};
const UCHAR cOSDColorBlue[]	= {0,0,0x1f};
const UCHAR cOSDColorBlack[]	= {0x00,0x00,0x00};
const UCHAR cOSDColorWhite[]	= {0x1f,0x1f,0x1f};
const UCHAR cOSDColorYellow[]	= {0x1f,0x1f,0x00};
const UCHAR cOSDColorPurple[] =  {20,4,30};
const UCHAR cOSDColorIndianRed[] = {26,12,12};
const UCHAR cOSDColorOliveDrab[] = {14,18,5};
const UCHAR cOSDColorSkyBlue[] = {17,26,30};
const UCHAR cOSDColorWheat[] = {31,28,23};
const UCHAR cOSDColorThistle[] = {27,24,27};
const UCHAR cOSDColorGray[] = {26,26,26};

/* color table */
const UCHAR* ucOSDColorTbl[BASE_MW_OSD_COLORNUM] = {
	cOSDColorRed,
	cOSDColorGreen,
	cOSDColorBlue,
	cOSDColorBlack,
	cOSDColorWhite,
	cOSDColorYellow,
	cOSDColorPurple,
	cOSDColorIndianRed,
	cOSDColorOliveDrab,
	cOSDColorSkyBlue,
	cOSDColorWheat,
	cOSDColorThistle,
	cOSDColorGray
};

const ULONG** ulOSDCharTbl[BASE_MW_OSD_MODENUM] = {	
	ascii_table_16_24
};
/*********************************************************************************
	prototype
**********************************************************************************/
__inline__  void vBASE_MW_OSD_SetColor(ULONG *pulColorPtr, UCHAR *pucColor);
__inline__  void vBASE_MW_OSD_SetFGColor(ULONG *pulColorPtr, UCHAR *pucColor);
__inline__  void vBASE_MW_OSD_SetBGColor(ULONG *pulColorPtr, UCHAR *pucColor);
eBASE_MW_OSD_ERROR eBASE_MW_OSD_StrToRgb(CHAR* pucOsdStr,LONG lStrLen, sBASE_MW_OSD_PARA* psOsdPara);
eBASE_MW_OSD_ERROR eBASE_MW_OSD_RgbBufToBitmap(BITMAP_S* bitmap,sBASE_MW_OSD_PARA *psOsdPara);
eBASE_MW_OSD_ERROR eBASE_MW_OSD_GetCharSize(sBASE_MW_OSD_PARA* psOsdPara);
void vBASE_MW_OSD_Init(CHAR* pcOsdStr,LONG lLen,sBASE_MW_OSD_PARA* psOsdPara);


/*********************************************************************************
	function
**********************************************************************************/

/*******************************************************************************
* @brief		: vBASE_MW_OSD_SetColor(ULONG *pulColorPtr, UCHAR *pucColor)
* @param		: 
* @retval		: void
* @note		: set the bitmap according to color
*******************************************************************************/
__inline__  void vBASE_MW_OSD_SetColor(ULONG *pulColorPtr, UCHAR *pucColor)
{
	debug_log_funcIn();
	/*****************************************/
	{
		*pulColorPtr = /*COMM_BIT(BASE_MW_OSD_ALPHT_SHIFT)
						|*/(pucColor[BASE_MW_OSD_RED]<<BASE_MW_OSD_R_SHIFT)
						|(pucColor[BASE_MW_OSD_GREEN]<<BASE_MW_OSD_G_SHIFT)
						|(pucColor[BASE_MW_OSD_BLUE]<<BASE_MW_OSD_B_SHIFT);
	}
	/*****************************************/

	debug_log_funcOut();
}

__inline__  void vBASE_MW_OSD_SetFGColor(ULONG *pulColorPtr, UCHAR *pucColor)
{
	debug_log_funcIn();
	/*****************************************/
	{
		*pulColorPtr = COMM_BIT(BASE_MW_OSD_ALPHT_SHIFT)
						|(pucColor[BASE_MW_OSD_RED]<<BASE_MW_OSD_R_SHIFT)
						|(pucColor[BASE_MW_OSD_GREEN]<<BASE_MW_OSD_G_SHIFT)
						|(pucColor[BASE_MW_OSD_BLUE]<<BASE_MW_OSD_B_SHIFT);
	}
	/*****************************************/

	debug_log_funcOut();
}

__inline__  void vBASE_MW_OSD_SetBGColor(ULONG *pulColorPtr, UCHAR *pucColor)
{
	debug_log_funcIn();
	/*****************************************/
	{
		*pulColorPtr = /*COMM_BIT(BASE_MW_OSD_ALPHT_SHIFT)
						|*/(pucColor[BASE_MW_OSD_RED]<<BASE_MW_OSD_R_SHIFT)
						|(pucColor[BASE_MW_OSD_GREEN]<<BASE_MW_OSD_G_SHIFT)
						|(pucColor[BASE_MW_OSD_BLUE]<<BASE_MW_OSD_B_SHIFT);
	}
	/*****************************************/

	debug_log_funcOut();
}

/*******************************************************************************
* @brief		: eBASE_MW_OSD_StrToRgb(CHAR* pucOsdStr,LONG lStrLen, sBASE_MW_OSD_PARA* psOsdPara)
* @param		: 
* @retval		: eBASE_MW_OSD_ERROR
* @note		: Convert osd string to RGB1555
*******************************************************************************/
eBASE_MW_OSD_ERROR eBASE_MW_OSD_StrToRgb(CHAR* pucOsdStr,LONG lStrLen, sBASE_MW_OSD_PARA* psOsdPara)
{
	eBASE_MW_OSD_ERROR eRet = BASE_MW_OSD_SUCCESS;
	
	LONG lWidth = 0;
	LONG lHeight = 0;
	ULONG ulColorData = 0;

	/* height count of one char */
	LONG lBitMapHeightCnt = 0;
	/* width count of one char */
	LONG lBitMapCharWidthCnt = 0;
	/* char count */
	LONG lStrCharCnt = 0;
	/* rgb buffer length */
	LONG lBytesLen = 0;

	UCHAR* pucRgbBuf = psOsdPara->pucOsdBitmapBuf;
	ULONG** ppulCharMode = NULL;

	USHORT* punRgbAddr = NULL;
 
	debug_log_funcIn();

	/*****************************************/
	{
		ppulCharMode = (ULONG**)ulOSDCharTbl[psOsdPara->eOsdMode];
		/* get size of one char */
		eBASE_MW_OSD_GetCharSize(psOsdPara);
		lWidth = psOsdPara->lOsdCharWidth;
		lHeight = psOsdPara->lOsdCharHeight;

		/* scan all row of string */
		 for(lBitMapHeightCnt = 0; lBitMapHeightCnt< lHeight; lBitMapHeightCnt ++)
		 {
			/* scan one row of all char of string */
		 	for(lStrCharCnt = 0; lStrCharCnt < lStrLen; lStrCharCnt ++)
			{
				ulColorData = (ppulCharMode[(int)psOsdPara->ucOsdString[lStrCharCnt]][lBitMapHeightCnt]);
		 		//showval(ulColorData);
				/* scan one char of string */
				for(lBitMapCharWidthCnt = 0; lBitMapCharWidthCnt < lWidth; lBitMapCharWidthCnt++)
				{		
					/* set the point color of RGB */
					punRgbAddr = (USHORT*) pucRgbBuf;
					*punRgbAddr = (USHORT)((ulColorData&(1<<lBitMapCharWidthCnt))? 
							psOsdPara->lForeColorVal: psOsdPara->lBackColorVal);;
					
					/* move RGB buffer pointer */
				    pucRgbBuf += 2;
					/* add RGB buffer */
				    lBytesLen += 2;
				}
		 	}
		 }
		/* restore osd RGB buffer information */
		 psOsdPara->pucOsdBitmapBuf = pucRgbBuf;
		 psOsdPara->lOsdBitmapBufLen += lBytesLen;
		 psOsdPara->lOsdBitmapWidth += lWidth * lStrLen;
		 psOsdPara->lOsdBitmapHeight = lHeight;
	}
	/*****************************************/

	debug_log_funcOut();
	return eRet;
}

/*******************************************************************************
* @brief		: eBASE_MW_OSD_RgbBufToBitmap(int len,BITMAP_S* bitmap,sBASE_MW_OSD_PARA *psOsdPara)
* @param		: 
* @retval		: eBASE_MW_OSD_ERROR
* @note		: Copy RGB1555 data to Bitmap buffer
*******************************************************************************/
eBASE_MW_OSD_ERROR eBASE_MW_OSD_RgbBufToBitmap(BITMAP_S* bitmap,sBASE_MW_OSD_PARA *psOsdPara)
{
	eBASE_MW_OSD_ERROR eRet = BASE_MW_OSD_SUCCESS;
	debug_log_funcIn();

	/*****************************************/
	{

		UCHAR* buffp = psOsdPara->ucOsdBitmapBuf;
		MALLOC(psOsdPara->lOsdBitmapBufLen);
		bitmap->pData = (char*)malloc(psOsdPara->lOsdBitmapBufLen);

		if(!(bitmap->pData))
		{
			debug_info("malloc error!\n");
			eRet = BASE_MW_OSD_MALLOCFAILURE;
			return eRet;
		}

		bitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
		bitmap->u32Width = psOsdPara->lOsdBitmapWidth;
		bitmap->u32Height = psOsdPara->lOsdBitmapHeight;
		memcpy((char*)(bitmap->pData),buffp,psOsdPara->lOsdBitmapBufLen);

	}
	/*****************************************/

	debug_log_funcOut();
	return eRet;
}

/*******************************************************************************
* @brief		: eBASE_MW_OSD_ERROR eBASE_MW_OSD_GetCharSize(sBASE_MW_OSD_PARA* psOsdPara)
* @param		: 
* @retval		: eBASE_MW_OSD_ERROR
* @note		: Get char size of current char mode
*******************************************************************************/
eBASE_MW_OSD_ERROR eBASE_MW_OSD_GetCharSize(sBASE_MW_OSD_PARA* psOsdPara)
{
	eBASE_MW_OSD_ERROR eRet = BASE_MW_OSD_SUCCESS;
	eBASE_MW_OSD_CHARMODE eOsdMode = psOsdPara->eOsdMode;
	
	debug_log_funcIn();

	/*****************************************/
	{
		switch(eOsdMode)
		{
			case BASE_MW_OSD_CHAR_16_24:
				psOsdPara->lOsdCharHeight = 24;
				psOsdPara->lOsdCharWidth = 16;
				break;		
			default:
				eRet = BASE_MW_OSD_FAILURE;
				break;
		}
	}
	/*****************************************/

	debug_log_funcOut();
	return eRet;
}
/*******************************************************************************
* @brief		: vBASE_MW_OSD_Init(CHAR* pcOsdStr,int len,sBASE_MW_OSD_PARA* psOsdPara)
* @param		: 
* @retval		: void
* @note		: Init of osd
*******************************************************************************/
void vBASE_MW_OSD_Init(CHAR* pcOsdStr,LONG lLen,sBASE_MW_OSD_PARA* psOsdPara)
{
	debug_log_funcIn();

	/*****************************************/
	{
		UCHAR *pucForeColor = NULL;
		UCHAR *pucBackColor = NULL;
		
		memset(psOsdPara,0,sizeof(sBASE_MW_OSD_PARA));
		
		psOsdPara->pucOsdBitmapBuf = psOsdPara->ucOsdBitmapBuf;
		psOsdPara->lOsdBitmapBufLen = 0;
		psOsdPara->eOsdMode = BASE_MW_OSD_CHAR_16_24;
		psOsdPara->eBackColor = BASE_MW_OSD_GRAY;
		psOsdPara->eForeColor = BASE_MW_OSD_WHITE;

		psOsdPara->lOsdStringLen = ((lLen>BASE_MW_OSD_STRINGSIZE)?BASE_MW_OSD_STRINGSIZE:lLen);
		memcpy(psOsdPara->ucOsdString,pcOsdStr,psOsdPara->lOsdStringLen);

		/* choose color according to foreground or background */
		pucBackColor = (UCHAR*)ucOSDColorTbl[(int)psOsdPara->eBackColor];
		pucForeColor = (UCHAR*)ucOSDColorTbl[(int)psOsdPara->eForeColor];
		vBASE_MW_OSD_SetBGColor((ULONG*)&(psOsdPara->lBackColorVal), pucBackColor);
		vBASE_MW_OSD_SetFGColor((ULONG*)&(psOsdPara->lForeColorVal), pucForeColor);
		
	}
	/*****************************************/

	debug_log_funcOut();
}
/*******************************************************************************
* @brief		: eBASE_MW_OSD_StrToBitmap(CHAR* pcOsdStr,int len,BITMAP_S* bitmap)
* @param		: 
* @retval		: eBASE_MW_OSD_ERROR
* @note		: Convert OSD string to Bitmap buffer
*******************************************************************************/
eBASE_MW_OSD_ERROR eBASE_MW_OSD_StrToBitmap(CHAR* pcOsdStr,LONG lLen,BITMAP_S* bitmap)
{
	eBASE_MW_OSD_ERROR eRet = BASE_MW_OSD_SUCCESS;
	sBASE_MW_OSD_PARA sOsdPara;
	LONG i;
	debug_log_funcIn();

	/*****************************************/
	{
		if(lLen <= 0) {
			debug_log("not input string!\n");
			return BASE_MW_OSD_FAILURE;
		}
		
		for(i=0;i<lLen;i++) {
			if(pcOsdStr[i] < 0x20 ||pcOsdStr[i] > 0x7f) {
				debug_log("the input character is not support!\n");
				return BASE_MW_OSD_FAILURE;
			}
		}
		
		/* init */
		vBASE_MW_OSD_Init(pcOsdStr,lLen,&sOsdPara);

		/* convert string to rgb1555 */
		eRet = eBASE_MW_OSD_StrToRgb(pcOsdStr,sOsdPara.lOsdStringLen,&sOsdPara);
		if(BASE_MW_OSD_SUCCESS != eRet)
		{
			debug_log("convert string to rgb error!\n");
			debug_log_showErr(eRet);
			return eRet;
		}
		
		/* covert rgb155 to bitmap */
		eRet = eBASE_MW_OSD_RgbBufToBitmap(bitmap,&sOsdPara);
		if(BASE_MW_OSD_SUCCESS != eRet)
		{
			debug_log("convert rgb buffer to bitmap error!\n");
			debug_log_showErr(eRet);
			return eRet;
		}
	}
	/*****************************************/

	debug_log_funcOut();
	return eRet;
}

/*******************************************************************************
* @brief		: eBASE_MW_OSD_LoadBmp(const char *filename, BITMAP_S *pstBitmap)
* @param		: const char *filename, BITMAP_S *pstBitmap
* @retval		: eBASE_MW_OSD_ERROR
* @note 		: load bmp from file
*******************************************************************************/
eBASE_MW_OSD_ERROR eBASE_MW_OSD_LoadBmp(const char *filename, BITMAP_S *pstBitmap) {
	eBASE_MW_OSD_ERROR eRet = BASE_MW_OSD_SUCCESS;
	OSD_SURFACE_S Surface;
	OSD_BITMAPFILEHEADER bmpFileHeader;
	OSD_BITMAPINFO bmpInfo;

	debug_log_funcIn();

	/*****************************************/
	{
		if(lBASE_GetBmpInfo(filename,&bmpFileHeader,&bmpInfo) < 0)
		{
			debug_info("lBASE_GetBmpInfo err!\n");
		    return BASE_MW_OSD_FAILURE;
		}

		Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;

		MALLOC(2*(bmpInfo.bmiHeader.biWidth)*(bmpInfo.bmiHeader.biHeight));
		pstBitmap->pData = malloc(2*(bmpInfo.bmiHeader.biWidth)*(bmpInfo.bmiHeader.biHeight));

		if(NULL == pstBitmap->pData)
		{
		    debug_info("malloc osd memroy err!\n");        
		    return BASE_MW_OSD_FAILURE;
		}

		eRet=lBASE_CreateSurfaceByBitMap(filename,&Surface,(HI_U8*)(pstBitmap->pData));
		if(BASE_MW_OSD_SUCCESS != eRet) {
			return BASE_MW_OSD_FAILURE;
		}

		pstBitmap->u32Width = Surface.u16Width;
		pstBitmap->u32Height = Surface.u16Height;
		pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;		 

 	}
	/*****************************************/

	debug_log_funcOut();
	
    return eRet;
}
#ifdef __cplusplus
}
#endif
