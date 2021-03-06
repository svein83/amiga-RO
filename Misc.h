/*
**
** Misc.h
**
*/

extern BOOL global_Locale;
extern BOOL cfg_Completed;
extern BOOL global_Unfolded;

extern LONG global_SleepClock;

extern APTR app_RumorOpus;
extern APTR bt_StatusBar;
extern APTR wi_Main;
extern APTR pg_Page[2];
extern APTR st_PathGadget[2];
extern APTR lv_Directory[2];
extern APTR bt_StatusText[2];
/*
extern APTR gr_List[2];
*/

extern char global_GroupSeparator[2];
extern char global_Exec_String[256];
extern char global_ARexxName[11];

__far extern char cfg_RecogString[100][41];
__far extern char cfg_RecogHex[100][41];
__far extern char cfg_RecogName[100][41];
__far extern char cfg_RecogCommand[100][81];
__far extern char cfg_RecogCommandB[100][81];

extern char global_IdleStatus[41];

extern struct Catalog * global_Catalog;
extern struct ProcMsg * global_ClockMessage;

extern void UpdateClock( void );
extern void UpdateStatusText ( int side );
extern char * StringRequester ( char *Title_String, char *Gadget_String, char *Reject_String, int MaxLength, int SkipGadget, BOOL *Skip, BOOL *Cancel );
extern char * GetPath ( int side );
extern BOOL Exists( char * File_String );

LONG Active_Side;
LONG global_Actions;
BOOL global_DirLoaded[2];
BOOL global_AskOnce;

BOOL CheckMemory ( int Needed_Memory );

int OtherSide ( int side );
int MouseSide ( int X, int Y );

void Fail( void );
void Init( void );
void InitVars( void );
void SleepWindow( BOOL Sleep_BOOL );
void SleepClock( BOOL Sleep_BOOL );
void SetActiveColor ( int side );

char * GetCatStr( int num, char *defstring );

extern BOOL cfg_ShowMem;
extern BOOL cfg_ShowClock;
extern BOOL cfg_ShowDate;
extern BOOL cfg_ShowDay;
