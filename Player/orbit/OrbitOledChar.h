/************************************************************************/
/*																		*/
/*	OrbitOledChar.h	--	Interface Declarations for OrbitOledChar.c		*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2013, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created											*/
/*	04/04/2013(JordanR):  Ported for Stellaris LaunchPad				*/
/*	06/06/2013(JordanR):  Prepared for release							*/
/*																		*/
/************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	OrbitOledSetCursor(int xch, int ych);
void	OrbitOledGetCursor(int * pxcy, int * pych);
int		OrbitOledDefUserChar(char ch, char * pbDef);
void	OrbitOledSetCharUpdate(int f);
int		OrbitOledGetCharUpdate();
void	OrbitOledPutChar(char ch);
void	OrbitOledPutString(char * sz);

/* ------------------------------------------------------------ */

/************************************************************************/
#ifdef __cplusplus
}
#endif