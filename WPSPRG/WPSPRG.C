 #--------------------------------------------------------------------------
 # Program name:  Find1.csc      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
##
## CLASS: Find
##
## CLASS HIERARCHY:
##      SOMObject
##         WPObject
##           WPAbstract
##             Find
## DESCRIPTION:
##    This is the find object class.
##    An instance of this class can be created as a
Workplace
##    object.
##
##
include <wpabs.sc>

class: Find,
        external stem = find,
        local,
        external prefix = find_,
        classprefix     = findM_,
        major version = 1,
        minor version = 1;

parent: WPAbstract;
passthru: C.h, after;
    #define DebugBo(title, text) \
         WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
(PSZ) text , \
                      (PSZ) title, 20, MB_OK ³ MB_INFORMATION )
endpassthru;
data:
 HAB          hab;
 HMQ          hmq;
 QMSG         qmsg;
 HWND         Frame;

methods:
        override wpInitData;
        override wpDelete;
        override wpModifyPopupMenu;
        override wpMenuItemSelected;

 #--------------------------------------------------------------------------
 # Program name:  Find1.mak      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #    To Compile:
 #         C:\FIND>nmake -f find.mak all
 #--------------------------------------------------------------------------

#*******************************************
#  Dot directive definition area (usually just suffixes)
#*******************************************

 .SUFFIXES: .c .obj .dll .csc .sc .h .ih .ph .psc .rc .res

#*******************************************
*******
#  Environment Setup for the component(s).
#*******************************************
*******

SOMTEMP=.\somtemp
SCPATH  = d:\toolkt20\sc
HPATH   =d:\toolkt20\c\os2h
LIBPATH =d:\toolkt20\os2lib

!if ›set SMINCLUDE=.;$(SCPATH);| ³³ \
    ›set SMTMP=$(SOMTEMP)| ³³ \
    ›set SMEMIT=ih;h;ph;psc;sc;c|
!endif

!if ›cd $(SOMTEMP)|
!  if ›md $(SOMTEMP)|
!    error error creating $(SOMTEMP) directory
!  endif
!else
!  if ›cd ..|
!    error - Couldn't cd .. from $(SOMTEMP) directory
!  endif
!endif

#
# Compiler/tools Macros
#

CC      = icc /c /Ge- /Gd- /Se /Sp2 /Re /Kb /ss /Ms /Gm+
LINK    = link386
LDFLAGS = /noi /map /nol /nod /exepack /packcode
/packdata /align:16
LIBS    = som.lib os2386.lib dde4mbs.lib dde4nbs.lib

#*******************************************
*******
# Set up Macros that will contain all the different
# dependencies for the executables and dlls etc. that
# are generated.
#*******************************************
*******

OBJS       =  find.obj finddlg.obj initterm.obj

#*******************************************
*******
#   Setup the inference rules for compiling source code
to
#   object code.
#*******************************************
*******

 .c.obj:
        $(CC) -I$(HPATH) -c $<

 .csc.ih:
        sc -v -r $*.csc

all: find.ih find.dll

#
# Specific Process Tag
#

find.ih:   $*.csc $(HPATH)\wpdataf.h

find.obj: $*.ih $*.c $*.h  $*.sc $(HPATH)\wpdataf.h

findlg.obj: $*.c $*.h

find.dll: $*.def $(OBJS) find.res
         $(LINK) $(LDFLAGS) $(OBJS),$@,,$(LIBS),$*;
         rc $*.res $*.dll
         mapsym find.map
         implib find.lib find.def

find.res: find.rc
          rc -r $*.rc $*.res

#INCLUDES -C=ih

 #--------------------------------------------------------------------------
 # Program name:  Find1.C        Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
/********************************************
******
 * NAME: find.c
 *
 * DESCRIPTION:
 *
 * Copyright (c) 1991                        IBM Corporation
\********************************************
******/

#define Find_Class_Source

#define INCL_GPI                    /* Selectively include
*/
#define INCL_WINFRAMEMGR            /* Frame-manager
function       */
#define INCL_WINSYS                 /* System value & color
function*/
#define INCL_WINERRORS              /* WIN functions error
constants*/
#define INCL_WINDIALOGS             /* Dialog-box
functions         */
#define INCL_WINMESSAGEMGR          /* Message-Mgr.
functions       */
#define INCL_DOSPROCESS             /* Process & thread
support func*/
#define INCL_WIN
#define INCL_DOS
#define INCL_GPIBITMAPS
#define INCL_WPCLASS
#define INCL_WPOBJECT

#include <os2.h>
#include <pmwp.h>  /* eventually will be #define
INCL_WINWORKPLACE */


#include "finddef.h"
#include "find.ih"
#include "find.ph"
#include "finddat.h"


extern VOID     SOMLINK SOMInitModule( VOID );
extern int      InitializeFind(VOID);
extern BOOL     Findmain(Find *,int,char **);


/***SOMEXTERN int SOM_TraceLevel = 1; ***/

/*-------------------------------------------------
* NAME: SOMInitModule( VOID )
*
* DESCRIPTION:
*  This is the class initialization entry point.
*  This routine will be called during the module
initialization.
*  Applications must add a call to initialize each object
class being
*  furing in the module.  The class initialization call is in
the
*   format:
*
*  <ClassName>NewClass(integer4 majorVersion,
integer4minorVersion)
*
*  where <ClassName> is the name of the new class being
defined.
*
* NOTES:
*  Do NOT call any methods on any of the classes being
instanciated
*  during the processing of SOMInitModule().
*
*  SOMInitModule must be exported in the module
definition file (.DEF)
*  in the format:
*
*      EXPORTS
*          _SOMInitModule
*
\*-------------------------------------------------
*/
VOID SOMLINK SOMInitModule()
{
   FindNewClass(Find_MajorVersion,Find_MinorVersion);
}


int InitializeFind()
{CHAR   ErrorBuffer›100|;

  if (DosLoadModule((PSZ) ErrorBuffer,
                     sizeof(ErrorBuffer),
                    "FIND", &hmodule) )
      return FALSE;
  FindIcon = WinLoadPointer( HWND_DESKTOP, hmodule,
101 );
  if (FindIcon==(HPOINTER)0)
      DebugBox("FAILURE","!!!load Icon failed");	
 return TRUE;
}

/*-------------------------------------------------
 *
 *  OVERRIDE: wpInitData
 *
 *  PURPOSE:
 *    Initialize our state variables. Allocate any extra
memory that
 *    we might need.
 *
---------------------------------------------------
*/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMInstance
SOM_Scope void   SOMLINK find_wpInitData(Find
*somSelf)
{

    /*FindData *somThis = FindGetData(somSelf); */
    FindMethodDebug("Find","Find_wpInitData");

    parent_wpInitData(somSelf);
    if (FindIcon)
       _wpSetIcon(somSelf,FindIcon);
    argc   = 1;
    argv›0|="find";

    Findmain(somSelf,1,argv);
}


SOM_Scope BOOL   SOMLINK
find_wpModifyPopupMenu(Find *somSelf,
                                                HWND hwndMenu,
                                                HWND hwndCnr,
                                                ULONG iPosition)
{
    /* FindData *somThis = FindGetData(somSelf); */
    FindMethodDebug("Find","find_wpModifyPopupMenu");

    _wpInsertPopupMenuItems(somSelf,        /*this cases
adds to menu*/
                            hwndMenu,
                            iPosition,
                            hmodule,
                            RC_IDD_FINDMENU,
                            0);
   return
(parent_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr
,
                                    iPosition));
}


SOM_Scope BOOL   SOMLINK
find_wpMenuItemSelected(Find *somSelf,
                                                 HWND hwndFrame,
                                                 ULONG MenuId)
{FindData *somThis = FindGetData(somSelf);
/*required*/


    FindMethodDebug("Find","find_wpMenuItemSelected");

    switch (MenuId) {
       case RC_MI_FIND:
             WinPostMsg(_Frame,WM_COMMAND,
                     (MPARAM)MAKEULONG(FIND,FIND),
                     (MPARAM)0L);
              break;
        default:

return(parent_wpMenuItemSelected(somSelf,hwndFrame,
MenuId));
      }
  return(TRUE);
}


SOM_Scope ULONG   SOMLINK find_wpDelete(Find
*somSelf,
                                        ULONG fConfirmations)
{
    FindData *somThis = FindGetData(somSelf);
    FindMethodDebug("Find","find_wpDelete");

    WinPostMsg(_Frame,WM_QUIT,(MPARAM)0,
(MPARAM)0);   /*For
Now Okay*/
    return (parent_wpDelete(somSelf,fConfirmations));
}

 #--------------------------------------------------------------------------
 # Program name:  Finddlg1.C     Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
/********************************************
******
 * NAME: finddlg.c
 *
 * DESCRIPTION:
 *
 * Copyright (c) 1991 IBM Corporation
*********************************************
******/
#define INCL_GPI                        /* Selectively include
*/
#define INCL_WIN                        /* Selectively include
*/
#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finddef.h"
#include "finddat.h"

#include "find.ih"


extern void  FindString(char *filename_p,
                       char *string,
                       unsigned fileattr,
                       HWND list);
extern void  SearchFile(char *filename,char
*string,HWND list);
extern void _Optlink ObjectWindowThread(void *arg);

extern MRESULT EXPENTRY DialogProc(HWND   hwndDlg,
                                   USHORT msg,
                                   MPARAM pm1,
                                   MPARAM pm2);
extern MRESULT EXPENTRY ObjectWndProc(
                                      HWND   hWnd,
                                      USHORT msg,
                                      MPARAM pm1,
                                      MPARAM pm2);


void  SearchFile(
    char          *filename,
    char          *string,
    HWND           list)
{FILE *fp;
 char buffer›256|;
 char data›256|;
 char *p;
 BOOL flag=1;

 if ( (fp=fopen(filename,"r"))==(FILE *)0)
    return;

 while (!feof(fp)){
       fgets(buffer,256,fp);
       if ((p=strchr(buffer,'\n'))!=(char *)0)
           *p=0;
       if (strstr(buffer,string)!=(char *)0)
         {
          if (flag)
             {
              sprintf(data,"####%s####",filename);
              WinSendMsg(list,
                         (ULONG)LM_INSERTITEM,
                         (MPARAM)LIT_END,
                         (MPARAM)data);
              flag=0;
             }
          WinSendMsg(list,
                     (ULONG)LM_INSERTITEM,
                     (MPARAM)LIT_END,
                     (MPARAM)buffer);
         }
 }
 fclose(fp);
}

void FindString(
    char          *filename_p,
    char          *string,
    unsigned       fileattr,
    HWND           list)

{HDIR            dirhandle=0xFFFF;
 FILEFINDBUF3    resultbuf;
 ULONG           lcount=1;
 char            *p;
 char             buffer›256|;
 char             path›256|;


 strcpy(path,filename_p);
 if ( (p=strrchr(path,'\\'))!=(char *)0)
    *(++p)=0;
 if (p==(char *)0)
   if ( (p=strchr(path,':'))!=(char *)0)
      *(++p)=0;


 if (DosFindFirst((PSZ)   filename_p,
                  (PHDIR) &dirhandle,
                  (ULONG) fileattr,
                  (PVOID) &resultbuf,
                  (ULONG) sizeof(resultbuf),
                  (PULONG) &lcount,
                  (ULONG) 1)==0)
      {
       do {
          sprintf(buffer,"%s%s",path,resultbuf.achName);
          SearchFile(buffer,string,list);
          }while (DosFindNext((HDIR)  dirhandle,
                              (PVOID) &resultbuf,
                              (ULONG) sizeof(resultbuf),
                              (PULONG) &lcount)==0);


       DosFindClose(dirhandle);
      }
}


MRESULT EXPENTRY DialogProc(
       HWND   hwndDlg,
       USHORT msg,
       MPARAM pm1,
       MPARAM pm2)
{char filename›256|;
 char string›256|;
 HWND list;

  switch (msg) {
     case WM_COMMAND:
          switch (LOUSHORT(pm1)) {
              case IDD_ENTER:
                    list =WinWindowFromID(hwndDlg,IDD_LIST);
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_FILENAME,
                                        255,
                                        (PSZ)filename
                                       );
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_STRING,
                                        255,
                                        (PSZ)string
                                       );
                    FindString(filename,string,(unsigned)0,list);
                    break;
              case IDD_CANCEL:
              default:
                    WinDismissDlg( hwndDlg, FALSE );
                    break;
             }
          break;
     default:
         return(WinDefDlgProc(hwndDlg, msg, pm1, pm2));
    }
  return(MRFROMLONG(NULL));
}


MRESULT EXPENTRY ObjectWndProc(
             HWND   hWnd,
             USHORT msg,
             MPARAM pm1,
             MPARAM pm2)
{Find     *somSelf;
 FindData *somThis;


  switch (msg)  {
   case WM_COMMAND:
        somSelf=(Find
*)WinQueryWindowULong(hWnd,QWL_USER);
        somThis = FindGetData(somSelf);
        switch (SHORT1FROMMP(pm1)) {
             case FIND:
                 WinDlgBox((HWND)  HWND_DESKTOP,
                           (HWND)  HWND_DESKTOP,
                           (PFNWP) DialogProc,
                           hmodule,
                           (ULONG) IDD_DLG,
                           (PVOID) 0);
             default: break;
           }
         break;
    default:
         return ((MRESULT)WinDefWindowProc (hWnd, msg,
pm1,
pm2));
   }
  return((MRESULT)0);
}


void _Optlink ObjectWindowThread(void *arg)
{Find     *somSelf = (Find *) arg;
 FindData *somThis;


    somThis = FindGetData(somSelf);

   _hab = WinInitialize((ULONG)0L);
   _hmq = WinCreateMsgQueue(_hab, (LONG)0L);
    WinRegisterClass(_hab,
                     (PSZ)"FindObjectClass",
                     (PFNWP)ObjectWndProc,
                     (ULONG)0L,
                     (ULONG)4L);
    _Frame = WinCreateWindow(HWND_OBJECT,
                            (PSZ)   "FindObjectClass",
                            (PSZ)   "Object Window",
                            0L,

(ULONG)0L,(ULONG)0L,(ULONG)0L,(ULONG)0L,
                            (HWND)0,
                            HWND_TOP,
                            101,
                            (ULONG)0,
                            (PVOID)0);
    WinSetWindowULong(_Frame,             /*For Access
needed*/
                      QWL_USER,
                      (ULONG)somSelf);
    while (WinGetMsg(_hab,
                     (PQMSG)&_qmsg,
                     (HWND)NULL,
                     (ULONG)0L,
                     (ULONG)0L ) ){
        if (_qmsg.msg==WM_QUIT)
            break;
        WinDispatchMsg( _hab, (PQMSG)&_qmsg );
       }
    WinDestroyWindow( _Frame );
    WinDestroyMsgQueue( _hmq );
    WinTerminate( _hab );
}


BOOL Findmain(
   Find *somSelf,
   int          argc,
   char       **argv)

{
 if (_beginthread(ObjectWindowThread,
              (void *)0,
              (unsigned)4*4096,
              (void *)somSelf)==(-1))
     DosBeep(1000,1000);
 return(TRUE);
}

 #--------------------------------------------------------------------------
 # Program name:  Initterm1.C    Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------

/********************************************
******
  SPEC:
      DLL init and term module for the Toronto Compiler
*********************************************
******/

#define  INCL_DOSPROCESS

#include <stdlib.h>
#include <stdio.h>
#include <os2.h>


extern BOOL DLLInit( void );
extern BOOL DLLUninit( void );

#define DebugBox(title, text) \
         WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
(PSZ) text , \
                      (PSZ) title, 20, MB_OK ³ MB_INFORMATION )


/*-------------------------------------------------
 SPEC:
  This routine is called once for this class and not once
for each
  instance
  of this class.  This is for initialization which covers all
instances,
  for example loading strings or icons out of the resource
module.
  DLLUninit should be used to free any resources
allocated during this
  routine.
---------------------------------------------------
*/
BOOL DLLInit()
{
 InitializeFind();
 return(TRUE);
}


/*-------------------------------------------------
 NAME: DLLUninit( VOID )

 DESCRIPTION:
  This is the dynalink module exit routine.
  This routine will be called right before the module is
being unloaded.

 NOTES:
  This routine is called once for this class and not once
for each
  instance
  of this class.  This is for cleanup of initialization
which covers all
  instances, for example destroying string or icon
resources that were
  allocated during DLLInit processing.
---------------------------------------------------
*/
BOOL DLLUninit()
{
 return(TRUE);
}


/*
 * _edcinit is the C-runtime environment init. function.
 * Returns 0 to indicate success and -1 to indicate
failure
 */
  int _CRT_init(void);

/*
 * _edcterm is the C-runtime termination function.
 */
  int _CRT_term(unsigned long);


/*
 * _DLL_InitTerm is the function that tets called by the
operation
 * system
 * loader when it lads and frees the dll for each process
that accesses
 * the dll.
 */
#pragma linkage( _DLL_InitTerm, system )

unsigned long _DLL_InitTerm(unsigned long modhandle,
unsigned long
flag)
{
   switch (flag)
     {
     case 0:
        /*
         * Call the C run-time init function before any thing
else.
         */
         if (_CRT_init() == -1)
           return 0UL;
         DLLInit();

         break;

     case 1:
        /*
         * Call the C run-time termination function.
         */
         DLLUninit();
         _CRT_term(0UL);
         break;
     } /* endswitch */

  return 1UL;

}

APIRET DosGetThreadInfo(PTIB *pptib,PPIB *pppib)
{
     return(DosGetInfoBlocks(pptib,pppib) );
}


 #--------------------------------------------------------------------------
 # Program name:  Finddat1.C     Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
int          argc;
char        *argv›3|;
HMODULE     hmodule;
HPOINTER    FindIcon;

FINDDEF1.H

#define FIND                  100

#define IDD_DLG               200
#define IDD_FILENAME          201
#define IDD_STRING            202
#define IDD_ENTER             203
#define IDD_CANCEL            204
#define IDD_LIST              205

#define MENUID_USER_BASE      0
#define RC_IDD_FINDMENU       (MENUID_USER_BASE+1)
#define RC_MI_FIND            100

FIND1.RC

#define INCL_WIN
#include <os2.h>

#include "finddef.h"

POINTER 101 find.ico


MENU RC_IDD_FINDMENU LOADONCALL MOVEABLE
DISCARDABLE
BEGIN
   MENUITEM "Find string", RC_MI_FIND
END

DLGTEMPLATE IDD_DLG LOADONCALL MOVEABLE
DISCARDABLE
BEGIN
    DIALOG  "Find dialogbox", IDD_DLG, 58, -17, 165, 106,
WS_VISIBLE,
            FCF_SYSMENU ³ FCF_TITLEBAR
    BEGIN
        LTEXT           "Filename", 101, 9, 87, 44, 8
        LTEXT           "String", 102, 23, 72, 30, 8
        ENTRYFIELD      "", IDD_FILENAME, 54, 88, 101,
8,ES_MARGIN
        ENTRYFIELD      "", IDD_STRING, 54, 71, 101, 8,
ES_MARGIN
        PUSHBUTTON      "Enter", IDD_ENTER, 13, 8, 40, 14
        PUSHBUTTON      "Cancel", IDD_CANCEL, 109, 8, 40,
14
        LISTBOX         IDD_LIST, 13, 25, 142, 40
    END
END

FIND1.DEF

;  FileName: find.def.
;  Generated using:
;      SOM Compiler sc: 1.10
;      SOM Emitter emitdef: 1.10
; @(#)emit.c 1.10 10/25/91 14:09:30 ›10/31/91| (c)IBM
Corp.1991
LIBRARY Find INITINSTANCE
DESCRIPTION 'Find Class Library - (c) Copyright IBM
1991'
PROTMODE
DATA MULTIPLE NONSHARED LOADONCALL
EXPORTS
   SOMInitModule
   FindCClassData
   FindClassData
   FindNewClass
   DialogProc
   ObjectWndProc

 #--------------------------------------------------------------------------
 # Program name:  Find2.csc      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
## CLASS: Find
##
## CLASS HIERARCHY:
##      SOMObject
##         WPObject
##           WPAbstract
##             Find
## DESCRIPTION:
##    This is the find object class.
##    An instance of this class can be created as a
Workplace object.
##
include <wpabs.sc>

class: Find,
        external stem = find,
        local,
        external prefix = find_,
        classprefix     = findM_,
        major version = 1,
        minor version = 1;

parent: WPAbstract;
passthru: C.h, after;
   #define DebugBox(title, text) \
          WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
(PSZ) text , \
                       (PSZ) title, 20, MB_OK ³ MB_INFORMATION
)
endpassthru;
##
##Instance Data Statement
##
data:
 HWND         *Frame;

methods:
        override wpInitData;
        override wpDelete;
        override wpModifyPopupMenu;
        override wpMenuItemSelected;

 #--------------------------------------------------------------------------
 # Program name:  Find2.mak      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
#*******************************************
*******
#  Dot directive definition area (usually just suffixes)
#*******************************************
*******

 .SUFFIXES: .c .obj .dll .csc .sc .h .ih .ph .psc .rc .res

#*******************************************
*******
#  Environment Setup for the component(s).
#*******************************************
*******

SOMTEMP=.\somtemp
SCPATH  = d:\toolkt20\sc
HPATH   =d:\toolkt20\c\os2h
LIBPATH =d:\toolkt20\os2lib

!if ›set SMINCLUDE=.;$(SCPATH);| ³³ \
    ›set SMTMP=$(SOMTEMP)| ³³ \
    ›set SMEMIT=ih;h;ph;psc;sc;c|
!endif

!if ›cd $(SOMTEMP)|
!  if ›md $(SOMTEMP)|
!    error error creating $(SOMTEMP) directory
!  endif
!else
!  if ›cd ..|
!    error - Couldn't cd .. from $(SOMTEMP) directory
!  endif
!endif

#
# Compiler/tools Macros
#

CC      = icc /c /Ge- /Gd- /Se /Sp2 /Re /Kb /ss /Ms /Gm+
LINK    = link386
LDFLAGS = /noi /map /nol /nod /exepack /packcode
/packdata /align:16
LIBS    = som.lib os2386.lib dde4mbs.lib dde4nbs.lib

#*******************************************
*******
# Set up Macros that will contain all the different
dependencies for
# the executables and dlls etc. that are generated.
#*******************************************
*******

OBJS       =  find.obj initterm.obj

#*******************************************
*******
#   Setup the inference rules for compiling source code
to
#   object code.
#*******************************************
*******

 .c.obj:
        $(CC) -I$(HPATH) -c $<

 .csc.ih:
        sc -v -r $*.csc

all: find.ih find.dll

#
# Specific Process Tag
#

find.ih:   $*.csc $(HPATH)\wpdataf.h

find.obj: $*.ih $*.c $*.h  $*.sc $(HPATH)\wpdataf.h

findlg.obj: $*.c $*.h

find.dll: $*.def $(OBJS) find.res
         $(LINK) $(LDFLAGS) $(OBJS),$@,,$(LIBS),$*;
         rc $*.res $*.dll
         mapsym find.map
         implib find.lib find.def

find.res: find.rc
          rc -r $*.rc $*.res

#INCLUDES -C=ih

 #--------------------------------------------------------------------------
 # Program name:  Find2.C        Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
/********************************************
******
 * NAME: find.c
 *
 * DESCRIPTION:
 *
 * Copyright (c) 1991 IBM Corporation
\********************************************
*******/

#define Find_Class_Source

#define INCL_GPI                    /* Selectively include
*/
#define INCL_WINFRAMEMGR            /* Frame-manager
function       */
#define INCL_WINSYS                 /* System value & color
function*/
#define INCL_WINERRORS              /* WIN functions error
constants*/
#define INCL_WINDIALOGS             /* Dialog-box
functions         */
#define INCL_WINMESSAGEMGR          /* Message-Mgr.
functions       */
#define INCL_DOSPROCESS             /* Process & thread
support func*/
#define INCL_WIN
#define INCL_DOS
#define INCL_GPIBITMAPS
#define INCL_WPCLASS
#define INCL_WPOBJECT

#include <os2.h>
#include <pmwp.h>  /* eventually will be #define
INCL_WINWORKPLACE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "finddef.h"
#include "find.ih"
#include "find.ph"
#include "finddat.h"


extern VOID     SOMLINK SOMInitModule( VOID );
extern int      InitializeFind(VOID);
extern BOOL     Findmain(Find *,int,char **);


/***SOMEXTERN int SOM_TraceLevel = 1; ***/

/*-------------------------------------------------
* NAME: SOMInitModule( VOID )
*
* DESCRIPTION:
*  This is the class initialization entry point.
*  This routine will be called during the module
initialization.
*  Applications must add a call to initialize each object
class being
*  defined in the module.  The class initialization call is
in the
*  format:
*
* <ClassName>NewClass(integer4 majorVersion,
integer4minorVersion)
*
*  where <ClassName> is the name of the new class being
defined.
*
* NOTES:
*  Do NOT call any methods on any of the classes being
instanciated
*  during the processing of SOMInitModule().
*
*  SOMInitModule must be exported in the module
definition file (.DEF)
*  in the format:
*
*      EXPORTS
*          _SOMInitModule
*
-------------------------------------------------*/
VOID SOMLINK SOMInitModule()
{
   FindNewClass(Find_MajorVersion,Find_MinorVersion);
}


int InitializeFind()
{CHAR   ErrorBuffer›100|;

  if (DosLoadModule((PSZ) ErrorBuffer,
                     sizeof(ErrorBuffer),
                    "FIND", &hmodule) )
      return FALSE;
  FindIcon = WinLoadPointer( HWND_DESKTOP, hmodule,
101 );
  if (FindIcon==(HPOINTER)0)
      DebugBox("FAILURE","!!!load Icon failed");
 return TRUE;
}

/*-------------------------------------------------
 *
 *  OVERRIDE: wpInitData
 *
 *  PURPOSE:
 *    Initialize our state variables. Allocate any extra
memory that
 *    we might need.
 *
---------------------------------------------------
*/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMInstance
SOM_Scope void   SOMLINK find_wpInitData(Find
*somSelf)
{char        sharename›256|;
 char        Errorbuffer›60|;
 char        parms›256|;
 RESULTCODES result;
 FindData   *somThis = FindGetData(somSelf);


    FindMethodDebug("Find","Find_wpInitData");

    parent_wpInitData(somSelf);
    if (FindIcon)
       _wpSetIcon(somSelf,FindIcon);

    _Frame = (HWND *)0;                /*initialize*/

sprintf(sharename,"\\SHAREMEM\\%lX.FND",(ULONG)somS
elf);
    DosAllocSharedMem((PPVOID)&_Frame,
                       sharename,
                       4096,
                       PAG_COMMIT³PAG_READ³PAG_WRITE);

    memset(&parms,0,sizeof(parms));   /*parms format*/
    strcpy(parms,"find");
    sprintf(parms+5,"%s",sharename);
    DosExecPgm(Errorbuffer,
               sizeof(Errorbuffer),
               EXEC_ASYNC,
               parms,                     /*somSelf addr arg */
               0,
               &result,
               "FINDDLG.EXE");
}


SOM_Scope BOOL   SOMLINK
find_wpModifyPopupMenu(Find *somSelf,
                                                HWND hwndMenu,
                                                HWND hwndCnr,
                                                ULONG iPosition)
{
    /* FindData *somThis = FindGetData(somSelf); */
    FindMethodDebug("Find","find_wpModifyPopupMenu");

    _wpInsertPopupMenuItems(somSelf,        /*this cases
adds to menu*/
                            hwndMenu,
                            iPosition,
                            hmodule,
                            RC_IDD_FINDMENU,
                            0);

     return(parent_wpModifyPopupMenu(somSelf,
                                     hwndMenu,
                                     hwndCnr,
                                     iPosition));
}


SOM_Scope BOOL   SOMLINK
find_wpMenuItemSelected(Find *somSelf,
                                                 HWND hwndFrame,
                                                 ULONG MenuId)
{FindData *somThis = FindGetData(somSelf);
/*required*/


    FindMethodDebug("Find","find_wpMenuItemSelected");

    switch (MenuId) {
       case RC_MI_FIND:
             if (_Frame!=(HWND)0)
                WinPostMsg(*_Frame,WM_COMMAND,
                          (MPARAM)MAKEULONG(FIND,FIND),
                          (MPARAM)0L);
             break;
        default:

return(parent_wpMenuItemSelected(somSelf,hwndFrame,
MenuId));
      }
 return(TRUE);
}


SOM_Scope ULONG   SOMLINK find_wpDelete(Find
*somSelf,
                                        ULONG fConfirmations)
{
    FindData *somThis = FindGetData(somSelf);
    FindMethodDebug("Find","find_wpDelete");

    if (_Frame!=(HWND)0)
       WinPostMsg(*_Frame,WM_QUIT,(MPARAM)0,
(MPARAM)0);/*For Now Okay*/
    return (parent_wpDelete(somSelf,fConfirmations));
}

 #--------------------------------------------------------------------------
 # Program name:  Find2.def      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
;  FileName: find.def.
;  Generated using:
;      SOM Compiler sc: 1.10
;      SOM Emitter emitdef: 1.10
; @(#)emit.c 1.10 10/25/91 14:09:30 ›10/31/91| (c)IBM
Corp.1991
LIBRARY Find INITINSTANCE
DESCRIPTION 'Find Class Library - (c) Copyright IBM
1991'
PROTMODE
DATA MULTIPLE NONSHARED LOADONCALL
EXPORTS
   SOMInitModule
   FindCClassData
   FindClassData
   FindNewClass

 #--------------------------------------------------------------------------
 # Program name:  Finddlg2.mak   Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
 .SUFFIXES: .c .obj .dll .csc .sc .h .ih .ph .psc .rc .res

#*******************************************
*******
#  Environment Setup for the component(s).
#*******************************************
*******

SOMTEMP=.\somtemp
SCPATH  = d:\toolkt20\sc
HPATH   =d:\toolkt20\c\os2h
LIBPATH =d:\toolkt20\os2lib


#
# Compiler/tools Macros
#

CC     = icc /c /Ti /G3 /Ge /Gs /Kb
LINK   = link386
LDFLAGS= /co /noi /map /nol /nod /exepack /packcode
/packdata /align:16
LIBS   = os2386.lib dde4sbs.lib dde4nbs.lib


OBJS       =  finddlg.obj


 .c.obj:
        $(CC) -I$(HPATH) -c $<

all: finddlg.exe


findlg.obj: $*.c $*.h

finddlg.exe: $*.def $(OBJS) find.res
         $(LINK) $(LDFLAGS) $(OBJS),$@,,$(LIBS),$*;
         rc find.res $*.exe

find.res: find.rc
          rc -r find.rc find.res

#INCLUDES -C=ih

 #--------------------------------------------------------------------------
 # Program name:  Finddlg2.C     Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
/********************************************
******
 * NAME: finddlg.c
 *
 * DESCRIPTION:
 *
 * Copyright (c) 1991 IBM Corporation
*********************************************
******/
#define INCL_GPI                        /* Selectively include
*/
#define INCL_WIN                        /* Selectively include
*/
#define INCL_DOS


#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finddef.h"
#include "finddat.h"


extern void  FindString(char *filename_p,
                       char *string,
                       unsigned fileattr,
                       HWND list);
extern void  SearchFile(char *filename,char
*string,HWND list);
extern MRESULT EXPENTRY DialogProc(HWND   hwndDlg,
                                   USHORT msg,
                                   MPARAM pm1,
                                   MPARAM pm2);
extern MRESULT EXPENTRY ObjectWndProc(
                                      HWND   hWnd,
                                      USHORT msg,
                                      MPARAM pm1,
                                      MPARAM pm2);

HAB          _hab;
HMQ          _hmq;
QMSG         _qmsg;
HWND         _Frame;



void  SearchFile(
    char          *filename,
    char          *string,
    HWND           list)
{FILE *fp;
 char buffer›256|;
 char data›256|;
 char *p;
 BOOL flag=1;

 if ( (fp=fopen(filename,"r"))==(FILE *)0)
    return;

 while (!feof(fp)){
       fgets(buffer,256,fp);
       if ((p=strchr(buffer,'\n'))!=(char *)0)
           *p=0;
       if (strstr(buffer,string)!=(char *)0)
         {
          if (flag)
             {
              sprintf(data,"####%s####",filename);
              WinSendMsg(list,
                         (ULONG)LM_INSERTITEM,
                         (MPARAM)LIT_END,
                         (MPARAM)data);
              flag=0;
             }
          WinSendMsg(list,
                     (ULONG)LM_INSERTITEM,
                     (MPARAM)LIT_END,
                     (MPARAM)buffer);
         }
 }
 fclose(fp);
}


void FindString(
    char          *filename_p,
    char          *string,
    unsigned       fileattr,
    HWND           list)

{HDIR            dirhandle=0xFFFF;
 FILEFINDBUF3    resultbuf;
 ULONG           lcount=1;
 char            *p;
 char             buffer›256|;
 char             path›256|;


 strcpy(path,filename_p);
 if ( (p=strrchr(path,'\\'))!=(char *)0)
    *(++p)=0;
 if (p==(char *)0)
   if ( (p=strchr(path,':'))!=(char *)0)
      *(++p)=0;


 if (DosFindFirst((PSZ)   filename_p,
                  (PHDIR) &dirhandle,
                  (ULONG) fileattr,
                  (PVOID) &resultbuf,
                  (ULONG) sizeof(resultbuf),
                  (PULONG) &lcount,
                  (ULONG) 1)==0)
      {
       do {
          sprintf(buffer,"%s%s",path,resultbuf.achName);
          SearchFile(buffer,string,list);
          }while (DosFindNext((HDIR)  dirhandle,
                              (PVOID) &resultbuf,
                              (ULONG) sizeof(resultbuf),
                              (PULONG) &lcount)==0);


       DosFindClose(dirhandle);
      }
}

MRESULT EXPENTRY DialogProc(
       HWND   hwndDlg,
       USHORT msg,
       MPARAM pm1,
       MPARAM pm2)
{char filename›256|;
 char string›256|;
 HWND list;

  switch (msg) {
     case WM_COMMAND:
          switch (LOUSHORT(pm1)) {
              case IDD_ENTER:
                    list =WinWindowFromID(hwndDlg,IDD_LIST);
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_FILENAME,
                                        255,
                                        (PSZ)filename
                                       );
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_STRING,
                                        255,
                                        (PSZ)string
                                       );
                    FindString(filename,string,(unsigned)0,list);
                    break;
              case IDD_CANCEL:
              default:
                    WinDismissDlg( hwndDlg, FALSE );
                    break;
             }
          break;
     default:
         return(WinDefDlgProc(hwndDlg, msg, pm1, pm2));
    }
  return(MRFROMLONG(NULL));
}

MRESULT EXPENTRY ObjectWndProc(
             HWND   hWnd,
             USHORT msg,
             MPARAM pm1,
             MPARAM pm2)
{

  switch (msg)  {
   case WM_COMMAND:
        switch (SHORT1FROMMP(pm1)) {
             case FIND:
                 WinDlgBox((HWND)  HWND_DESKTOP,
                           (HWND)  HWND_DESKTOP,
                           (PFNWP) DialogProc,
                           0L,
                           (ULONG) IDD_DLG,
                           (PVOID) 0);
             default: break;
           }
         break;
    default:
         return ((MRESULT)WinDefWindowProc (hWnd, msg,
pm1,
pm2));
   }
  return((MRESULT)0);
}


int main(int    argc,
         char **argv)
{HWND  *shared;
 int    rc;

    DosBeep(100,100);         /*Indicate that it has been
started*/

    if (argc<2)
       exit(-1);

    if (rc=DosGetNamedSharedMem((PPVOID)&shared,
                                argv›1|,
                                PAG_READ³PAG_WRITE))
       exit(-1);

   _hab = WinInitialize((ULONG)0L);
   _hmq = WinCreateMsgQueue(_hab, (LONG)0L);
    WinRegisterClass(_hab,
                     (PSZ)"FindObjectClass",
                     (PFNWP)ObjectWndProc,
                     (ULONG)0L,
                     (ULONG)4L);
    *shared=
    _Frame = WinCreateWindow(HWND_OBJECT,
                            (PSZ)   "FindObjectClass",
                            (PSZ)   "Object Window",
                            0L,

(ULONG)0L,(ULONG)0L,(ULONG)0L,(ULONG)0L,
                            (HWND)0,
                            HWND_TOP,
                            101,
                            (ULONG)0,
                            (PVOID)0);

    while (WinGetMsg(_hab,
                     (PQMSG)&_qmsg,
                     (HWND)NULL,
                     (ULONG)0L,
                     (ULONG)0L ) ){
        if (_qmsg.msg==WM_QUIT)
            break;
        WinDispatchMsg( _hab, (PQMSG)&_qmsg );
       }
DosBeep(1000,1000);                    /*indicate that it has
ended*/
    WinDestroyWindow( _Frame );
    WinDestroyMsgQueue( _hmq );
    WinTerminate( _hab );
}

 #--------------------------------------------------------------------------
 # Program name:  Finddlg2.def   Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
NAME    FINDDLG WINDOWAPI

DESCRIPTION 'Winthorn Application'

STUB    'OS2STUB.EXE'

CODE    MOVEABLE
DATA    MOVEABLE MULTIPLE

HEAPSIZE  8192
STACKSIZE 8192

EXPORTS
   DialogProc
   ObjectWndProc

 #--------------------------------------------------------------------------
 # Program name:  Find3.csc      Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
##
## Note: Any comments to go public should be preceeded
with '--'
##       Any comments to remain private should be
preceeded with '#'
##
##
## CLASS: Find
##
## CLASS HIERARCHY:
##      SOMObject
##         WPObject
##           WPAbstract
##             Find
## DESCRIPTION:
##    This is the find object class.
##    An instance of this class can be created as a
Workplace object.
##
##
include <wpabs.sc>

class: Find,
        external stem = find,
        local,
        external prefix = find_,
        classprefix     = findM_,
        major version = 1,
        minor version = 1;

parent: WPAbstract;
passthru: C.h, after;
    #define DebugBox(title, text) \
      WinMessageBox(HWND_DESKTOP,HWND_DESKTOP,
(PSZ) text ,\
                   (PSZ) title, 20, MB_OK ³ MB_INFORMATION )
endpassthru;
data:
 HWND          Frame;
 CHAR          instancename›256|;

methods:
        override wpInitData;
        override wpDelete;
        override wpModifyPopupMenu;
        override wpMenuItemSelected;

 #--------------------------------------------------------------------------
 # Program name:  Find3.c        Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------
/********************************************
******
 * NAME: find.c
 * DESCRIPTION:
 *
 * Copyright (c) 1991 IBM Corporation
\********************************************
******/


#define Find_Class_Source

#define INCL_GPI                    /* Selectively include
*/
#define INCL_WINFRAMEMGR            /* Frame-manager
function       */
#define INCL_WINSYS                 /* System value & color
function*/
#define INCL_WINERRORS              /* WIN functions error
constants*/
#define INCL_WINDIALOGS             /* Dialog-box
functions         */
#define INCL_WINMESSAGEMGR          /* Message-Mgr.
functions       */
#define INCL_DOSPROCESS             /* Process & thread
support func*/
#define INCL_WIN
#define INCL_DOS
#define INCL_GPIBITMAPS
#define INCL_WPCLASS
#define INCL_WPOBJECT

#include <os2.h>
#include <pmwp.h>  /* eventually will be #define
INCL_WINWORKPLACE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "finddef.h"
#include "find.ih"
#include "find.ph"
#include "finddat.h"


extern VOID     SOMLINK SOMInitModule( VOID );
extern int      InitializeFind(VOID);
extern BOOL     Findmain(Find *,int,char **);
extern BOOL     GetObjectWindow(Find *somSelf);  /*new
code*/


/***SOMEXTERN int SOM_TraceLevel = 1; ***/

/*-------------------------------------------------
*
* NAME: SOMInitModule( VOID )
*
* DESCRIPTION:
*  This is the class initialization entry point.
*  This routine will be called during the module
initialization.
*  Applications must add a call to initialize each object
class being
*  defined in the module.  The class initialization call is
in the
*  format:
*
* <ClassName>NewClass(integer4 majorVersion,
integer4minorVersion)
*
*  where <ClassName> is the name of the new class being
defined.
*
* NOTES:
*  Do NOT call any methods on any of the classes being
instanciated
*  during
*  the processing of SOMInitModule().
*
*  SOMInitModule must be exported in the module
definition file (.DEF)
*  in the format:
*
*      EXPORTS
*          _SOMInitModule
*
\*-------------------------------------------------
*/
VOID SOMLINK SOMInitModule()
{
   FindNewClass(Find_MajorVersion,Find_MinorVersion);
}


int InitializeFind()
{CHAR   ErrorBuffer›100|;

  if (DosLoadModule((PSZ) ErrorBuffer,
                     sizeof(ErrorBuffer),
                    "FIND", &hmodule) )
      return FALSE;
  FindIcon = WinLoadPointer( HWND_DESKTOP, hmodule,
101 );
  if (FindIcon==(HPOINTER)0)
      DebugBox("FAILURE","!!!load Icon failed");
 return TRUE;
}


BOOL GetObjectWindow(Find *somSelf)  /*new code*/
{HENUM henum;
 HWND  hwnd;
 CHAR  name›256|;
 FindData   *somThis = FindGetData(somSelf);


 if (_Frame!=(HWND)0)
    return(TRUE);
 henum = WinBeginEnumWindows(HWND_OBJECT);
 while (hwnd= WinGetNextWindow(henum)) {
    WinQueryClassName(hwnd,sizeof(name),name);
    if (strcmp(name,_instancename)==0)
       {
        _Frame=hwnd;
        WinEndEnumWindows(henum);
        return(TRUE);
       }
 }
 WinEndEnumWindows(henum);
 return(FALSE);
}


/*-------------------------------------------------
 *
 *  OVERRIDE: wpInitData
 *
 *  PURPOSE:
 *    Initialize our state variables. Allocate any extra
memory that
 *    we might need.
 *
---------------------------------------------------
*/
#undef SOM_CurrentClass
#define SOM_CurrentClass SOMInstance
SOM_Scope void   SOMLINK find_wpInitData(Find
*somSelf)
{char        Errorbuffer›60|;
 char        parms›256|;
 RESULTCODES result;

 FindData   *somThis = FindGetData(somSelf);


    FindMethodDebug("Find","Find_wpInitData");

    parent_wpInitData(somSelf);
    if (FindIcon)
       _wpSetIcon(somSelf,FindIcon);

    _Frame       = (HWND)0;                         /*new code*/
     sprintf(_instancename,"FindClass%d",somSelf);
/*new code*/

    memset(&parms,0,sizeof(parms));   /*parms format*/
    strcpy(parms,"find");
    sprintf(parms+5,"%s",_instancename);
    DosExecPgm(Errorbuffer,
               sizeof(Errorbuffer),
               EXEC_ASYNC,
               parms,                     /*somSelf addr arg */
               0,
               &result,
               "FINDDLG.EXE");
}


SOM_Scope BOOL   SOMLINK
find_wpModifyPopupMenu(Find *somSelf,
	               HWND hwndMenu,
	               HWND hwndCnr,
	               ULONG iPosition)
{
    /* FindData *somThis = FindGetData(somSelf); */
    FindMethodDebug("Find","find_wpModifyPopupMenu");

    _wpInsertPopupMenuItems(somSelf,        /*this cases
adds to menu*/
                            hwndMenu,
                            iPosition,
                            hmodule,
                            RC_IDD_FINDMENU,
                            0);
  return (parent_wpModifyPopupMenu(somSelf,
                                   hwndMenu,
                                   hwndCnr,
                                   iPosition));
}


SOM_Scope BOOL   SOMLINK
find_wpMenuItemSelected(Find *somSelf,
                HWND hwndFrame,
                ULONG MenuId)
{FindData *somThis = FindGetData(somSelf);
/*required*/


    FindMethodDebug("Find","find_wpMenuItemSelected");

    switch (MenuId) {
       case RC_MI_FIND:
             if (GetObjectWindow(somSelf))        /*new
code*/
                WinPostMsg(_Frame,WM_COMMAND,
                          (MPARAM)MAKEULONG(FIND,FIND),
                          (MPARAM)0L);
             break;
        default:
               return(parent_wpMenuItemSelected(somSelf,
                                                hwndFrame,
                                                MenuId));
      }
   return(TRUE);
}


SOM_Scope ULONG   SOMLINK find_wpDelete(Find
*somSelf,
   ULONG fConfirmations)
{
    FindData *somThis = FindGetData(somSelf);
    FindMethodDebug("Find","find_wpDelete");

   if (GetObjectWindow(somSelf))                      /*new
code*/
      WinPostMsg(_Frame,WM_QUIT,(MPARAM)0,
(MPARAM)0);/*ForNow Okay*/
   return (parent_wpDelete(somSelf,fConfirmations));
}

 #--------------------------------------------------------------------------
 # Program name:  Finddlg3.c     Title: WPS Programming using Mult Proc.
 # OS/2 Developer Magazine       Issue:  Spring '93, page 60
 #
 # Article:  "Workplace Shell Programming using Multiple Processes"
 # Author:  Richard Redpath, Sue Henshaw, Joe Coulombe
 # Phone:  n/a                   Fax:  n/a
 #
 # Description:  The following is a component of the complete source to the
 # above article in the specified issue of OS/2 Developer Magazine.  This
 # source is provided AS-IS without any implied or expressed warranty.
 #
 #--------------------------------------------------------------------------

/********************************************
******
 * NAME: finddlg.c
 *
 * DESCRIPTION:
 *
 * Copyright (c) 1991 IBM Corporation
*********************************************
*******/
#define INCL_GPI                        /* Selectively include
*/
#define INCL_WIN                        /* Selectively include
*/
#define INCL_DOS


#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finddef.h"
#include "finddat.h"


extern void  FindString(char *filename_p,
                       char *string,
                       unsigned fileattr,
                       HWND list);
extern void  SearchFile(char *filename,char
*string,HWND list);
extern MRESULT EXPENTRY DialogProc(HWND   hwndDlg,
                                   USHORT msg,
                                   MPARAM pm1,
                                   MPARAM pm2);
extern MRESULT EXPENTRY ObjectWndProc(
                                      HWND   hWnd,
                                      USHORT msg,
                                      MPARAM pm1,
                                      MPARAM pm2);

HAB          _hab;
HMQ          _hmq;
QMSG         _qmsg;
HWND         _Frame;


void  SearchFile(
    char          *filename,
    char          *string,
    HWND           list)
{FILE *fp;
 char buffer›256|;
 char data›256|;
 char *p;
 BOOL flag=1;

 if ( (fp=fopen(filename,"r"))==(FILE *)0)
    return;

 while (!feof(fp)){
       fgets(buffer,256,fp);
       if ((p=strchr(buffer,'\n'))!=(char *)0)
           *p=0;
       if (strstr(buffer,string)!=(char *)0)
         {
          if (flag)
             {
              sprintf(data,"####%s####",filename);
              WinSendMsg(list,
                         (ULONG)LM_INSERTITEM,
                         (MPARAM)LIT_END,
                         (MPARAM)data);
              flag=0;
             }
          WinSendMsg(list,
                     (ULONG)LM_INSERTITEM,
                     (MPARAM)LIT_END,
                     (MPARAM)buffer);
         }
 }
 fclose(fp);
}


void FindString(
    char          *filename_p,
    char          *string,
    unsigned       fileattr,
    HWND           list)

{HDIR            dirhandle=0xFFFF;
 FILEFINDBUF3    resultbuf;
 ULONG           lcount=1;
 char            *p;
 char             buffer›256|;
 char             path›256|;


 strcpy(path,filename_p);
 if ( (p=strrchr(path,'\\'))!=(char *)0)
    *(++p)=0;
 if (p==(char *)0)
   if ( (p=strchr(path,':'))!=(char *)0)
      *(++p)=0;


 if (DosFindFirst((PSZ)   filename_p,
                  (PHDIR) &dirhandle,
                  (ULONG) fileattr,
                  (PVOID) &resultbuf,
                  (ULONG) sizeof(resultbuf),
                  (PULONG) &lcount,
                  (ULONG) 1)==0)
      {
       do {
          sprintf(buffer,"%s%s",path,resultbuf.achName);
          SearchFile(buffer,string,list);
          }while (DosFindNext((HDIR)  dirhandle,
                              (PVOID) &resultbuf,
                              (ULONG) sizeof(resultbuf),
                              (PULONG) &lcount)==0);


       DosFindClose(dirhandle);
      }
}


MRESULT EXPENTRY DialogProc(
       HWND   hwndDlg,
       USHORT msg,
       MPARAM pm1,
       MPARAM pm2)
{char filename›256|;
 char string›256|;
 HWND list;

  switch (msg) {
     case WM_COMMAND:
          switch (LOUSHORT(pm1)) {
              case IDD_ENTER:
                    list =WinWindowFromID(hwndDlg,IDD_LIST);
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_FILENAME,
                                        255,
                                        (PSZ)filename
                                       );
                    WinQueryDlgItemText(hwndDlg,
                                        IDD_STRING,
                                        255,
                                        (PSZ)string
                                       );
                    FindString(filename,string,(unsigned)0,list);
                    break;
              case IDD_CANCEL:
              default:
                    WinDismissDlg( hwndDlg, FALSE );
                    break;
             }
          break;
     default:
         return(WinDefDlgProc(hwndDlg, msg, pm1, pm2));
    }
  return(MRFROMLONG(NULL));
}


MRESULT EXPENTRY ObjectWndProc(
             HWND   hWnd,
             USHORT msg,
             MPARAM pm1,
             MPARAM pm2)
{

  switch (msg)  {
   case WM_COMMAND:
        switch (SHORT1FROMMP(pm1)) {
             case FIND:
                 WinDlgBox((HWND)  HWND_DESKTOP,
                           (HWND)  HWND_DESKTOP,
                           (PFNWP) DialogProc,
                           0L,
                           (ULONG) IDD_DLG,
                           (PVOID) 0);
             default: break;
           }
         break;
    default:
      return
((MRESULT)WinDefWindowProc(hWnd,msg,pm1,pm2));
   }
  return((MRESULT)0);
}


int main(int    argc,
         char **argv)
{int    rc;

    DosBeep(100,100);          /*Indicate that it has been
started*/

    if (argc<2)
       exit(-1);

   _hab = WinInitialize((ULONG)0L);
   _hmq = WinCreateMsgQueue(_hab, (LONG)0L);
    WinRegisterClass(_hab,
                     (PSZ)argv›1|,                    /*new code*/
                     (PFNWP)ObjectWndProc,
                     (ULONG)0L,
                     (ULONG)4L);
    _Frame = WinCreateWindow(HWND_OBJECT,
                            (PSZ)   argv›1|,           /*new code*/
                            (PSZ)   "Object Window",
                            0L,

(ULONG)0L,(ULONG)0L,(ULONG)0L,(ULONG)0L,
                            (HWND)0,
                            HWND_TOP,
                            101,
                            (ULONG)0,
                            (PVOID)0);

    while (WinGetMsg(_hab,
                     (PQMSG)&_qmsg,
                     (HWND)NULL,
                     (ULONG)0L,
                     (ULONG)0L ) ){
        if (_qmsg.msg==WM_QUIT)
            break;
        WinDispatchMsg( _hab, (PQMSG)&_qmsg );
       }
    DosBeep(1000,1000);             /*indicate that it has
ended*/
    WinDestroyWindow( _Frame );
    WinDestroyMsgQueue( _hmq );
    WinTerminate( _hab );
}


