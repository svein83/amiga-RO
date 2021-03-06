#include "Includes.h"
#include "Application.h"

#define VERSION    "$VER: RO 1.28 Beta (19.06.18)"


/*
**
** MyDirListClass
**
*/

struct MUI_CustomClass *CL_MyDirList;

struct MyDirList_Data
{
	LONG dummy;
};

ULONG MyDirList_DragQuery(struct IClass *cl,Object *obj,struct MUIP_DragDrop *msg)
{
	if (msg->obj==obj)
	{
		return(DoSuperMethodA(cl,obj,msg));
	}
	else if ( ( msg->obj == lv_Dir[Left_Side] || msg->obj == lv_Dir[Right_Side] ) )
	{
		return(MUIV_DragQuery_Accept);
	}
	else
		return(MUIV_DragQuery_Refuse);
}

ULONG MyDirList_DragDrop(struct IClass *cl,Object *obj,struct MUIP_DragDrop *msg)
{
	int ErrorNum, i, num, sel=0, state;
	char * Str = NULL;

	if (msg->obj==obj)
	{
		return(DoSuperMethodA(cl,obj,msg));
	}
	else
	{
		if( msg->obj == lv_Dir[Left_Side] || msg->obj == lv_Dir[Right_Side] )
		{
			Sleep( TRUE );
			if ( CheckMemory( 50000 ) )
			{
				get( lv_Directory[Active_Side], MUIA_List_Entries, &num );
				for ( i = 0; i < num; i++ )
				{
					DoMethod( lv_Directory[Active_Side], MUIM_List_Select, i, MUIV_List_Select_Ask, &state );
					if ( state == MUIV_List_Select_On )
						sel++;
					if ( sel == 1 )
						break;
				}
				if ( sel == 0 )
					DoMethod( lv_Directory[Active_Side], MUIM_List_Select, MUIV_List_Select_Active, MUIV_List_Select_On, NULL );

				ErrorNum = DoFunction( drop_entries, Active_Side, NULL );
			}
			else
				ErrorNum = -1;
			if ( ErrorNum != 0 )
				Str = Error( ErrorNum );
			if ( Str != NULL )
			{
				strcpy( Status_String, Str );
				DoMethod( app_RumorOpus, MUIM_Application_ReturnID, ID_Message );
			}
			else
				SleepClock( FALSE );
			SleepWindow( FALSE );
		}
		return(0);
	}
}

SAVEDS ASM ULONG MyDirList_Dispatcher(REG(a0) struct IClass *cl,REG(a2) Object *obj,REG(a1) Msg msg)
{
	switch (msg->MethodID)
	{
		case MUIM_DragQuery: return(MyDirList_DragQuery(cl,obj,(APTR)msg));
		case MUIM_DragDrop : return(MyDirList_DragDrop (cl,obj,(APTR)msg));
	}
	return(DoSuperMethodA(cl,obj,msg));
}

APTR MakeLV ( int side )
{
	if ( !cfg_PageType )
	return (

		PageGroup,

			Child, VGroup,

				Child, bt_StatusText[side] = TextObject,
					TextFrame,
					MUIA_HelpNode, "DirInformation",
					MUIA_ShortHelp, ShortHelp[0],
					MUIA_Background, MUII_TextBack,
					MUIA_Text_SetMin, FALSE,
					MUIA_Text_Contents, GetCatStr( 1, "No Directory" ),
					MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,

				Child, lv_Directory[side] = ListviewObject,
					MUIA_Font, MUIV_Font_Fixed,
					MUIA_HelpNode, "Listview",
					MUIA_Listview_DragType, 1,
					MUIA_Listview_Input, TRUE,
					MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
					MUIA_Listview_MultiSelect, cfg_MultiSelect,
					MUIA_Listview_List, lv_Dir[side] = NewObject(CL_MyDirList->mcc_Class,NULL,
						InputListFrame,
						MUIA_Dirlist_FilterDrawers, FALSE,
						MUIA_Dirlist_MultiSelDirs, TRUE,
						MUIA_Dirlist_SortType, cfg_SortType[side],
						MUIA_Dirlist_SortDirs, cfg_FirstType[side],
						MUIA_Dirlist_SortHighLow, cfg_SortHighLow[side],
						MUIA_List_Format, cfg_Format[side],
						MUIA_List_ShowDropMarks, FALSE,
						TAG_DONE),
					End,

				Child, st_PathGadget[side] = StringObject,
					StringFrame,
					MUIA_HelpNode, "PathGadget",
					MUIA_ShortHelp, ShortHelp[1],
					MUIA_String_MaxLen, 512,
					MUIA_String_AdvanceOnCR, FALSE,
					End,
				End,

			Child, lv_Buffers[side] = ListviewObject,
				MUIA_Font, MUIV_Font_Fixed,
				MUIA_HelpNode, "Listview",
				MUIA_Listview_Input, TRUE,
				MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
				MUIA_Listview_List,
				ListObject,
					InputListFrame,
					MUIA_List_ConstructHook, MUIV_List_ConstructHook_String,
					MUIA_List_DestructHook, MUIV_List_DestructHook_String,
					End,
				End,

			Child, lv_Volumes[side] = ListviewObject,
				MUIA_Font, MUIV_Font_Fixed,
				MUIA_HelpNode, "Listview",
				MUIA_Listview_Input, TRUE,
				MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
				MUIA_Listview_List, VolumelistObject,
					InputListFrame,
					End,
				End,
			End

);
	else
	return (

		RegisterGroup(Pages),

			Child, VGroup,

				Child, bt_StatusText[side] = TextObject,
					TextFrame,
					MUIA_HelpNode, "DirInformation",
					MUIA_ShortHelp, ShortHelp[0],
					MUIA_Background, MUII_TextBack,
					MUIA_Text_SetMin, FALSE,
					MUIA_Text_Contents, GetCatStr( 1, "No Directory" ),
					MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,

				Child, lv_Directory[side] = ListviewObject,
					MUIA_Font, MUIV_Font_Fixed,
					MUIA_HelpNode, "Listview",
					MUIA_Listview_DragType, 1,
					MUIA_Listview_Input, TRUE,
					MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
					MUIA_Listview_MultiSelect, cfg_MultiSelect,
					MUIA_Listview_List, lv_Dir[side] = NewObject(CL_MyDirList->mcc_Class,NULL,
						InputListFrame,
						MUIA_Dirlist_FilterDrawers, FALSE,
						MUIA_Dirlist_MultiSelDirs, TRUE,
						MUIA_Dirlist_SortType, cfg_SortType[side],
						MUIA_Dirlist_SortDirs, cfg_FirstType[side],
						MUIA_Dirlist_SortHighLow, cfg_SortHighLow[side],
						MUIA_List_Format, cfg_Format[side],
						MUIA_List_ShowDropMarks, FALSE,
						TAG_DONE),
					End,

				Child, st_PathGadget[side] = StringObject,
					StringFrame,
					MUIA_HelpNode, "PathGadget",
					MUIA_ShortHelp, ShortHelp[1],
					MUIA_String_MaxLen, 512,
					MUIA_String_AdvanceOnCR, FALSE,
					End,
				End,

			Child, lv_Buffers[side] = ListviewObject,
				MUIA_Font, MUIV_Font_Fixed,
				MUIA_HelpNode, "Listview",
				MUIA_Listview_Input, TRUE,
				MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
				MUIA_Listview_List,
				ListObject,
					InputListFrame,
					MUIA_List_ConstructHook, MUIV_List_ConstructHook_String,
					MUIA_List_DestructHook, MUIV_List_DestructHook_String,
					End,
				End,

			Child, lv_Volumes[side] = ListviewObject,
				MUIA_Font, MUIV_Font_Fixed,
				MUIA_HelpNode, "Listview",
				MUIA_Listview_Input, TRUE,
				MUIA_Listview_ScrollerPos, MUIV_Listview_ScrollerPos_Default,
				MUIA_Listview_List, VolumelistObject,
					InputListFrame,
					End,
				End,
			End

);

}

/*
**
** StartApplication()
**
*/

BOOL StartApplication ( void )
{
	Pages[0] = GetCatStr( 69, "Directory" );
	Pages[1] = GetCatStr( 120, "Buffers" );
	Pages[2] = GetCatStr( 5, "Volumes" );

	CL_MyDirList = MUI_CreateCustomClass(NULL,MUIC_Dirlist ,NULL,sizeof(struct MyDirList_Data),MyDirList_Dispatcher);
	if (!CL_MyDirList) return(FALSE);

	app_RumorOpus = ApplicationObject,
		MUIA_Application_HelpFile, "RO.guide",
		MUIA_Application_Title         , "RO",
		MUIA_Application_Version       , VERSION,
		MUIA_Application_Copyright     , "Copyright � 1994-2018 by Oliver Rummeyer",
		MUIA_Application_Author        , "Oliver Rummeyer",
		MUIA_Application_Description   , GetCatStr( 0, "MUI-Based FileManager" ),
		MUIA_Application_Base          , "RO",
		MUIA_Application_Commands      , ARexxCommands,
		MUIA_Application_DropObject    , pg_Page[Left_Side],

		SubWindow,
			wi_Main = WindowObject,
			MUIA_Window_Title, "RO 1.28 Beta (19.06.18) -- Copyright � 1994-2018 by Oliver Rummeyer",
			MUIA_Window_ID, 1,
			MUIA_Window_Menustrip, Menu = MUI_MakeObject(MUIO_MenustripNM,MenuData,0),
			MUIA_Window_NeedsMouseObject, TRUE,
			MUIA_Window_AppWindow, TRUE,
			WindowContents, VGroup,

				Child, FoldGroup = HGroup,

					Child, pg_Page[Left_Side] = MakeLV(Left_Side),
					Child, bl_Balance = BalanceObject, End,
					Child, pg_Page[Right_Side] = MakeLV(Right_Side),
					End,

				Child, Row[0] = HGroup, MUIA_Group_SameSize, TRUE,

					MUIA_HelpNode, "DriveButton",
					MUIA_ShortHelp, ShortHelp[3],

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[0] = SimpleButton(""),
						Child, bt_Drive[1] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[2] = SimpleButton(""),
						Child, bt_Drive[3] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[4] = SimpleButton(""),
						Child, bt_Drive[5] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[6] = SimpleButton(""),
						Child, bt_Drive[7] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[8] = SimpleButton(""),
						Child, bt_Drive[9] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[10] = SimpleButton(""),
						Child, bt_Drive[11] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[12] = SimpleButton(""),
						Child, bt_Drive[13] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[14] = SimpleButton(""),
						Child, bt_Drive[15] = SimpleButton(""),
						End,

					End,

				Child, Row[1] = HGroup, MUIA_Group_SameSize, TRUE,

					MUIA_HelpNode, "DriveButton",
					MUIA_ShortHelp, ShortHelp[3],

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[16] = SimpleButton(""),
						Child, bt_Drive[17] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[18] = SimpleButton(""),
						Child, bt_Drive[19] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[20] = SimpleButton(""),
						Child, bt_Drive[21] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[22] = SimpleButton(""),
						Child, bt_Drive[23] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[24] = SimpleButton(""),
						Child, bt_Drive[25] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[26] = SimpleButton(""),
						Child, bt_Drive[27] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[28] = SimpleButton(""),
						Child, bt_Drive[29] = SimpleButton(""),
						End,

					Child, HGroup, MUIA_Group_SameSize, TRUE,
						Child, bt_Drive[30] = SimpleButton(""),
						Child, bt_Drive[31] = SimpleButton(""),
						End,

					End,

				Child, ButtonPage = PageGroup,

					Child, RowGroup(3), MUIA_Group_SameSize, TRUE,

						MUIA_HelpNode, "CommandButton",
						MUIA_ShortHelp, ShortHelp[4],

						Child, bt_Button[0]  = SimpleButton( cfg_ButtonText[0] ),
						Child, bt_Button[1]  = SimpleButton( cfg_ButtonText[1] ),
						Child, bt_Button[2]  = SimpleButton( cfg_ButtonText[2] ),
						Child, bt_Button[3]  = SimpleButton( cfg_ButtonText[3] ),
						Child, bt_Button[4]  = SimpleButton( cfg_ButtonText[4] ),
						Child, bt_Button[5]  = SimpleButton( cfg_ButtonText[5] ),
						Child, bt_Button[6]  = SimpleButton( cfg_ButtonText[6] ),
						Child, bt_Button[7]  = SimpleButton( cfg_ButtonText[7] ),
						Child, bt_Button[8]  = SimpleButton( cfg_ButtonText[8] ),
						Child, bt_Button[9]  = SimpleButton( cfg_ButtonText[9] ),
						Child, bt_Button[10] = SimpleButton( cfg_ButtonText[10] ),
						Child, bt_Button[11] = SimpleButton( cfg_ButtonText[11] ),
						Child, bt_Button[12] = SimpleButton( cfg_ButtonText[12] ),
						Child, bt_Button[13] = SimpleButton( cfg_ButtonText[13] ),
						Child, bt_Button[14] = SimpleButton( cfg_ButtonText[14] ),
						Child, bt_Button[15] = SimpleButton( cfg_ButtonText[15] ),
						Child, bt_Button[16] = SimpleButton( cfg_ButtonText[16] ),
						Child, bt_Button[17] = SimpleButton( cfg_ButtonText[17] ),
						Child, bt_Button[18] = SimpleButton( cfg_ButtonText[18] ),
						Child, bt_Button[19] = SimpleButton( cfg_ButtonText[19] ),
						Child, bt_Button[20] = SimpleButton( cfg_ButtonText[20] ),
						Child, bt_Button[21] = SimpleButton( cfg_ButtonText[21] ),
						Child, bt_Button[22] = SimpleButton( cfg_ButtonText[22] ),
						Child, bt_Button[23] = SimpleButton( cfg_ButtonText[23] ),
						End,

					Child, RowGroup(3), MUIA_Group_SameSize, TRUE,

						MUIA_HelpNode, "CommandButton",
						MUIA_ShortHelp, ShortHelp[4],

						Child, bt_Button[24] = SimpleButton( cfg_ButtonText[24] ),
						Child, bt_Button[25] = SimpleButton( cfg_ButtonText[25] ),
						Child, bt_Button[26] = SimpleButton( cfg_ButtonText[26] ),
						Child, bt_Button[27] = SimpleButton( cfg_ButtonText[27] ),
						Child, bt_Button[28] = SimpleButton( cfg_ButtonText[28] ),
						Child, bt_Button[29] = SimpleButton( cfg_ButtonText[29] ),
						Child, bt_Button[30] = SimpleButton( cfg_ButtonText[30] ),
						Child, bt_Button[31] = SimpleButton( cfg_ButtonText[31] ),
						Child, bt_Button[32] = SimpleButton( cfg_ButtonText[32] ),
						Child, bt_Button[33] = SimpleButton( cfg_ButtonText[33] ),
						Child, bt_Button[34] = SimpleButton( cfg_ButtonText[34] ),
						Child, bt_Button[35] = SimpleButton( cfg_ButtonText[35] ),
						Child, bt_Button[36] = SimpleButton( cfg_ButtonText[36] ),
						Child, bt_Button[37] = SimpleButton( cfg_ButtonText[37] ),
						Child, bt_Button[38] = SimpleButton( cfg_ButtonText[38] ),
						Child, bt_Button[39] = SimpleButton( cfg_ButtonText[39] ),
						Child, bt_Button[40] = SimpleButton( cfg_ButtonText[40] ),
						Child, bt_Button[41] = SimpleButton( cfg_ButtonText[41] ),
						Child, bt_Button[42] = SimpleButton( cfg_ButtonText[42] ),
						Child, bt_Button[43] = SimpleButton( cfg_ButtonText[43] ),
						Child, bt_Button[44] = SimpleButton( cfg_ButtonText[44] ),
						Child, bt_Button[45] = SimpleButton( cfg_ButtonText[45] ),
						Child, bt_Button[46] = SimpleButton( cfg_ButtonText[46] ),
						Child, bt_Button[47] = SimpleButton( cfg_ButtonText[47] ),
						End,

					Child, RowGroup(3), MUIA_Group_SameSize, TRUE,

						MUIA_HelpNode, "CommandButton",
						MUIA_ShortHelp, ShortHelp[4],

						Child, bt_Button[48] = SimpleButton( cfg_ButtonText[48] ),
						Child, bt_Button[49] = SimpleButton( cfg_ButtonText[49] ),
						Child, bt_Button[50] = SimpleButton( cfg_ButtonText[50] ),
						Child, bt_Button[51] = SimpleButton( cfg_ButtonText[51] ),
						Child, bt_Button[52] = SimpleButton( cfg_ButtonText[52] ),
						Child, bt_Button[53] = SimpleButton( cfg_ButtonText[53] ),
						Child, bt_Button[54] = SimpleButton( cfg_ButtonText[54] ),
						Child, bt_Button[55] = SimpleButton( cfg_ButtonText[55] ),
						Child, bt_Button[56] = SimpleButton( cfg_ButtonText[56] ),
						Child, bt_Button[57] = SimpleButton( cfg_ButtonText[57] ),
						Child, bt_Button[58] = SimpleButton( cfg_ButtonText[58] ),
						Child, bt_Button[59] = SimpleButton( cfg_ButtonText[59] ),
						Child, bt_Button[60] = SimpleButton( cfg_ButtonText[60] ),
						Child, bt_Button[61] = SimpleButton( cfg_ButtonText[61] ),
						Child, bt_Button[62] = SimpleButton( cfg_ButtonText[62] ),
						Child, bt_Button[63] = SimpleButton( cfg_ButtonText[63] ),
						Child, bt_Button[64] = SimpleButton( cfg_ButtonText[64] ),
						Child, bt_Button[65] = SimpleButton( cfg_ButtonText[65] ),
						Child, bt_Button[66] = SimpleButton( cfg_ButtonText[66] ),
						Child, bt_Button[67] = SimpleButton( cfg_ButtonText[67] ),
						Child, bt_Button[68] = SimpleButton( cfg_ButtonText[68] ),
						Child, bt_Button[69] = SimpleButton( cfg_ButtonText[69] ),
						Child, bt_Button[70] = SimpleButton( cfg_ButtonText[70] ),
						Child, bt_Button[71] = SimpleButton( cfg_ButtonText[71] ),
						End,

					Child, RowGroup(3), MUIA_Group_SameSize, TRUE,

						MUIA_HelpNode, "CommandButton",
						MUIA_ShortHelp, ShortHelp[4],

						Child, bt_Button[72] = SimpleButton( cfg_ButtonText[72] ),
						Child, bt_Button[73] = SimpleButton( cfg_ButtonText[73] ),
						Child, bt_Button[74] = SimpleButton( cfg_ButtonText[74] ),
						Child, bt_Button[75] = SimpleButton( cfg_ButtonText[75] ),
						Child, bt_Button[76] = SimpleButton( cfg_ButtonText[76] ),
						Child, bt_Button[77] = SimpleButton( cfg_ButtonText[77] ),
						Child, bt_Button[78] = SimpleButton( cfg_ButtonText[78] ),
						Child, bt_Button[79] = SimpleButton( cfg_ButtonText[79] ),
						Child, bt_Button[80] = SimpleButton( cfg_ButtonText[80] ),
						Child, bt_Button[81] = SimpleButton( cfg_ButtonText[81] ),
						Child, bt_Button[82] = SimpleButton( cfg_ButtonText[82] ),
						Child, bt_Button[83] = SimpleButton( cfg_ButtonText[83] ),
						Child, bt_Button[84] = SimpleButton( cfg_ButtonText[84] ),
						Child, bt_Button[85] = SimpleButton( cfg_ButtonText[85] ),
						Child, bt_Button[86] = SimpleButton( cfg_ButtonText[86] ),
						Child, bt_Button[87] = SimpleButton( cfg_ButtonText[87] ),
						Child, bt_Button[88] = SimpleButton( cfg_ButtonText[88] ),
						Child, bt_Button[89] = SimpleButton( cfg_ButtonText[89] ),
						Child, bt_Button[90] = SimpleButton( cfg_ButtonText[90] ),
						Child, bt_Button[91] = SimpleButton( cfg_ButtonText[91] ),
						Child, bt_Button[92] = SimpleButton( cfg_ButtonText[92] ),
						Child, bt_Button[93] = SimpleButton( cfg_ButtonText[93] ),
						Child, bt_Button[94] = SimpleButton( cfg_ButtonText[94] ),
						Child, bt_Button[95] = SimpleButton( cfg_ButtonText[95] ),
						End,

					Child, RowGroup(3), MUIA_Group_SameSize, TRUE,

						MUIA_HelpNode, "CommandButton",
						MUIA_ShortHelp, ShortHelp[4],

						Child, bt_Button[96]  = SimpleButton( cfg_ButtonText[96] ),
						Child, bt_Button[97]  = SimpleButton( cfg_ButtonText[97] ),
						Child, bt_Button[98]  = SimpleButton( cfg_ButtonText[98] ),
						Child, bt_Button[99]  = SimpleButton( cfg_ButtonText[99] ),
						Child, bt_Button[100] = SimpleButton( cfg_ButtonText[100] ),
						Child, bt_Button[101] = SimpleButton( cfg_ButtonText[101] ),
						Child, bt_Button[102] = SimpleButton( cfg_ButtonText[102] ),
						Child, bt_Button[103] = SimpleButton( cfg_ButtonText[103] ),
						Child, bt_Button[104] = SimpleButton( cfg_ButtonText[104] ),
						Child, bt_Button[105] = SimpleButton( cfg_ButtonText[105] ),
						Child, bt_Button[106] = SimpleButton( cfg_ButtonText[106] ),
						Child, bt_Button[107] = SimpleButton( cfg_ButtonText[107] ),
						Child, bt_Button[108] = SimpleButton( cfg_ButtonText[108] ),
						Child, bt_Button[109] = SimpleButton( cfg_ButtonText[109] ),
						Child, bt_Button[110] = SimpleButton( cfg_ButtonText[110] ),
						Child, bt_Button[111] = SimpleButton( cfg_ButtonText[111] ),
						Child, bt_Button[112] = SimpleButton( cfg_ButtonText[112] ),
						Child, bt_Button[113] = SimpleButton( cfg_ButtonText[113] ),
						Child, bt_Button[114] = SimpleButton( cfg_ButtonText[114] ),
						Child, bt_Button[115] = SimpleButton( cfg_ButtonText[115] ),
						Child, bt_Button[116] = SimpleButton( cfg_ButtonText[116] ),
						Child, bt_Button[117] = SimpleButton( cfg_ButtonText[117] ),
						Child, bt_Button[118] = SimpleButton( cfg_ButtonText[118] ),
						Child, bt_Button[119] = SimpleButton( cfg_ButtonText[119] ),
						End,

				End,

				Child, bt_StatusBar = TextObject,
					TextFrame,
					MUIA_HelpNode, "StatusBar",
					MUIA_ShortHelp, ShortHelp[2],
					MUIA_Background, MUII_TextBack,
					MUIA_Text_SetMin, FALSE,
					MUIA_Text_PreParse, "\33c",
					MUIA_InputMode, MUIV_InputMode_RelVerify,
					End,

				End,
			End,

		SubWindow,
			wi_Progress = WindowObject,
			MUIA_Window_Title, GetCatStr( 2, "Working..." ),
			MUIA_Window_ID, 2,
			MUIA_Window_Width, MUIV_Window_Width_Screen(25),
			MUIA_Window_Height, MUIV_Window_Width_MinMax(3),
			WindowContents, VGroup,
				Child, ga_Gauge = GaugeObject,
					GaugeFrame,
					MUIA_Gauge_Horiz, TRUE,
					End,
				Child, ScaleObject,
					MUIA_Scale_Horiz, TRUE,
					End,
				End,
			End,

		SubWindow,
			wi_String = WindowObject,
			MUIA_Window_ID, 3,
			MUIA_Window_Menu, MUIV_Window_Menu_NoMenu,
			WindowContents, VGroup,
				Child, st_StringGadget = StringObject,
					StringFrame,
					End,
				Child, HGroup,
					MUIA_Group_SameSize, TRUE,
					Child, bt_StringOkay = SimpleButton( GetCatStr( 72, "_Okay" ) ),
					Child, HSpace(0),
					Child, bt_StringSkip = SimpleButton( GetCatStr( 119, "_Skip" ) ),
					Child, bt_StringAll = SimpleButton( GetCatStr( 88, "A_ll" ) ),
					Child, HSpace(0),
					Child, bt_StringCancel = SimpleButton( GetCatStr( 73, "_Cancel" ) ),
					End,
				End,
			End,

		End;

	men_UserMenu = MenuObject, MUIA_Menu_Title, GetCatStr(26, "User"),
		End;

	if ( !app_RumorOpus || !men_UserMenu )
		return( FALSE );
	else
		return( TRUE );
}

/*
**
** StopApplication()
**
*/

BOOL StopApplication ( void )
{
	if ( men_UserMenu )
	{
		if( ! ( strlen( cfg_MenuEntryText[0] ) > 0 ) )
			MUI_DisposeObject( men_UserMenu );
	}
	if ( app_RumorOpus )
		MUI_DisposeObject( app_RumorOpus );

	if ( CL_MyDirList )
		MUI_DeleteCustomClass( CL_MyDirList );

	return( TRUE );
}

/*
**
** Methods()
**
*/

void Methods ( void )
{
	int i;

	static const struct Hook AppMsgHook = { { NULL,NULL },(VOID *)AppMsgFunc,NULL,NULL };
	static const struct Hook ButtonHook = { { NULL,NULL },(VOID *)ButtonFunc,NULL,NULL };
	static const struct Hook DriveHook = { { NULL,NULL },(VOID *)DriveFunc,NULL,NULL };

	/*** Listview Notifications ***/

	DoMethod( lv_Directory[Left_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_DoubleClick );
	DoMethod( lv_Directory[Right_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_DoubleClick );
	DoMethod( lv_Buffers[Left_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_BufferLeft );
	DoMethod( lv_Buffers[Right_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_BufferRight );
	DoMethod( lv_Volumes[Left_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_VolumeLeft );
	DoMethod( lv_Volumes[Right_Side], MUIM_Notify, MUIA_Listview_DoubleClick, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_VolumeRight );
	DoMethod( st_PathGadget[Left_Side], MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringEnterLeft );
	DoMethod( st_PathGadget[Right_Side], MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringEnterRight );
	DoMethod( lv_Directory[Left_Side], MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_UpdateTextLeft );
	DoMethod( lv_Directory[Right_Side], MUIM_Notify, MUIA_Dirlist_Status, MUIV_Dirlist_Status_Valid, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_UpdateTextRight );
	DoMethod( pg_Page[Left_Side], MUIM_Notify, MUIA_AppMessage, MUIV_EveryTime, pg_Page[Left_Side], 3, MUIM_CallHook, &AppMsgHook, MUIV_TriggerValue);
	DoMethod( pg_Page[Right_Side], MUIM_Notify, MUIA_AppMessage, MUIV_EveryTime, pg_Page[Right_Side], 3, MUIM_CallHook, &AppMsgHook, MUIV_TriggerValue);
	DoMethod( pg_Page[Left_Side], MUIM_Notify, MUIA_Group_ActivePage, 0, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_PageChange );
	DoMethod( pg_Page[Right_Side], MUIM_Notify, MUIA_Group_ActivePage, 0, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_PageChange );
	DoMethod( lv_Directory[Left_Side], MUIM_Notify, MUIA_Listview_SelectChange, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_LeftChange );
	DoMethod( lv_Directory[Right_Side], MUIM_Notify, MUIA_Listview_SelectChange, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_RightChange );

	get( sg_Scroll[Left_Side], MUIA_Scrollgroup_VertBar, &sb_Vertical[Left_Side] );
	get( sg_Scroll[Left_Side], MUIA_Scrollgroup_HorizBar, &sb_Horizontal[Left_Side] );
	get( sg_Scroll[Right_Side], MUIA_Scrollgroup_VertBar, &sb_Vertical[Right_Side] );
	get( sg_Scroll[Right_Side], MUIA_Scrollgroup_HorizBar, &sb_Horizontal[Right_Side] );

	/*** Window Notifications ***/

	DoMethod( wi_Progress, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, wi_Progress, 3, MUIM_Set, MUIA_Window_Open, FALSE );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_ActiveObject, lv_Directory[0], app_RumorOpus, 2, MUIM_Application_ReturnID, ID_LeftActive );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_ActiveObject, lv_Directory[1], app_RumorOpus, 2, MUIM_Application_ReturnID, ID_RightActive );

	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_ActiveObject, st_PathGadget[0], app_RumorOpus, 2, MUIM_Application_ReturnID, ID_LeftActive );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_ActiveObject, st_PathGadget[1], app_RumorOpus, 2, MUIM_Application_ReturnID, ID_RightActive );

	/*** Keyboard Notifications ***/

	for ( i = 0; i < 50; i++ ) DoMethod(wi_Main, MUIM_Notify, MUIA_Window_InputEvent, cfg_HotKey[i], app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Key_0 + i);
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f1", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_0 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f2", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_1 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f3", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_2 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f4", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_3 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f5", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_4 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f6", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_5 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f7", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_6 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f8", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_7 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f9", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_8 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "f10", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_Hot_9 );
	DoMethod( wi_Main, MUIM_Notify, MUIA_Window_InputEvent, "`", app_RumorOpus, 2, MUIM_Application_ReturnID, ID_TildeCycle );
	DoMethod( wi_Main, MUIM_Window_SetCycleChain, st_PathGadget[Left_Side], st_PathGadget[Right_Side], NULL );

	/*** Button Notifications ***/

	for ( i = 0; i < 32; i++ ) DoMethod( bt_Drive[i], MUIM_Notify, MUIA_Pressed, FALSE, bt_Drive[i], 3, MUIM_CallHook, &DriveHook, MUIV_TriggerValue );
	for ( i = 0; i < 120; i++ )	DoMethod( bt_Button[i], MUIM_Notify, MUIA_Pressed, FALSE, bt_Button[i], 3, MUIM_CallHook, &ButtonHook, MUIV_TriggerValue );
	DoMethod( bt_StatusBar, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StatusBar );
	DoMethod( bt_StatusText[Left_Side], MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StatusLeft );
	DoMethod( bt_StatusText[Right_Side], MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StatusRight );

	/*** String Request Notifications ***/

	DoMethod( wi_String, MUIM_Window_SetCycleChain, st_StringGadget, bt_StringOkay, bt_StringSkip, bt_StringCancel, NULL );
	DoMethod( wi_String, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringCancel );
	DoMethod( st_StringGadget, MUIM_Notify, MUIA_String_Acknowledge, MUIV_EveryTime, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringOkay );
	DoMethod( bt_StringOkay, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringOkay );
	DoMethod( bt_StringSkip, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringSkip );
	DoMethod( bt_StringAll, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringAll );
	DoMethod( bt_StringCancel, MUIM_Notify, MUIA_Pressed, FALSE, app_RumorOpus, 2, MUIM_Application_ReturnID, ID_StringCancel );

}
