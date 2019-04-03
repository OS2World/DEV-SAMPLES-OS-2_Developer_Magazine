##########################################################
#  WASTEBSK.CSC  Written by: Dave Hock                   #
#                            UCANDU Software, Inc.       #
#                            P.O Box 336                 #
#                            Cary, NC 27512              #
##########################################################
#
# Include the class definition file for the parent class
#
include <wpfolder.sc>

#
#   Define the new class
#
class: WPWasteBskt,
       file stem = wastebsk,
       external prefix = wpwastebskt_,
       class prefix = wpwastebsktcls_,
       major version = 1,
       minor version = 1,
       local;
-- PWFolder is a replacement for the shredder
-- It is derived as follows:
--        SOMOject
--          - WPObject
--             - WPFileSystem
--                - WPFolder
--                   - WPWasteBskt
#
# Specify the parent class
#
parent: WPFolder;

#
# Specify the release order of new methods
#
release order:SetAutoEmptyFlag, QueryAutoEmptyFlag;
# passthru to C compiler
passthru: C.ih;
#define IDKEY_AUTOEMPTY 9000
endpassthru;
#
#
# Define instance data for the class
#
data:
BOOL bAutoEmpty;
#
# Define new methods
#
methods:

BOOL QueryAutoEmptyFlag(), private;
--
--  METHOD:   QueryAutoEmptyFlag                                   PRIVATE
--  PURPOSE:  Returns flag
--  INVOKED:  From AutoEmptyDlgProc
--

BOOL SetAutoEmptyFlag(BOOL bAutoEmpty), private;
--
--  METHOD:   SetAutoEmptyFlag                                     PRIVATE
--  PURPOSE:  Sets instance data to value.
--  INVOKED:  From AutoEmptyDlgProc
--

#
# Specify methods being overridden
#
override wpInitData;
--
--  METHOD:   wpInitData                                  PUBLIC
--  PURPOSE:  Initializes instance data
--  INVOKED:  By Workplace Shell, upon instantiation of the object instance.
--

override wpModifyPopupMenu;
--
--  METHOD:   wpModifyPopupMenu                           PUBLIC
--  PURPOSE:  Adds an additional "Empty now" item to the object's context menu.
--  INVOKED:  By Workplace Shell, upon instantiation of the object instance.
--

override wpMenuItemSelected;
--
--  METHOD:   wpMenuItemSelected                          PUBLIC
--  PURPOSE:  Processes the user's selections from the context menu.  The
--            overridden method processes only the added "Empty now" item, before
--            invoking the parent's default processing to handle other items.
--  INVOKED:  By Workplace Shell, upon selection of a menu item by the user.
--

override wpSetup;
--
--  METHOD:   wpSetup                                     PUBLIC
--  PURPOSE:  Sets folder properties based upon a setup string passed by the
--            object's creator as part of the WinCreateObject() call.  The
--            overridden method simply processes the AUTOEMPTY keyword to set
--            the folder's autoempty status and period, before
--            invoking the parent's default processing to handle all other
--            keywords.
--  INVOKED:  By the Workplace Shell, upon instantiation of the object
--            instance.
--

override wpSaveState;
--
--  METHOD:   wpSaveState                                 PUBLIC
--  PURPOSE:  Saves the object instance's persistent state data.  The
--            overridden method simply saves the autoempty info , then invokes
--            the parent's default processing to handle any other instance
--            data defined by ancestor classes.
--  INVOKED:  By the Workplace Shell, when the object becomes dormant.

override wpRestoreState;
--
--  METHOD:   wpRestoreState                              PUBLIC
--  PURPOSE:  Restores the object instance's persistent state data.  The
--            overridden method simply restores the autoempty data, then
--            invokes the parent's default processing to handle any other
--            instance data defined by ancestor classes.
--  INVOKED:  By the Workplace Shell, when the object becomes awake.
--

override wpQueryStyle;
--
--  METHOD:   wpQueryStyle                                 PUBLIC
--  PURPOSE: Allows us to force a style such as OBJSYTLE_NOCOPY or NODELETE
--  INVOKED:  By the Workplace Shell, when object(s) are dragged over the wastebasket
--

override wpDragOver;
--
--  METHOD:   wpDragOver                                 PUBLIC
--  PURPOSE: To detect if the user is trying to drag permanent/undeletable objects to
--              the wastebasket.  If they are, we will put up the no-drop sign.
--  INVOKED:  By the Workplace Shell, when object(s) are dragged over the wastebasket
--

override wpAddFolderIncludePage;
--
--  METHOD:   wpAddFolderIncludePage                                 PUBLIC
--  PURPOSE:  Allows us to remove the include page so the user can't hide any of the
--              items in the waste basket.  This is important for we want the user to
--              always see the items so that something important is not accidentally
--              emptied.
--  INVOKED:  By the Workplace Shell, when the include page is added to the settings view.
--

override wpAddSettingsPages;
--
--  METHOD:   wpAddSettingsPages                              PUBLIC
--  PURPOSE:  Allows us to add the AutoEmpty page to the settings view.
--  INVOKED:  By the Workplace Shell, when the settings view is openned.
--

override wpclsQueryTitle, classmethod;
--
--  METHOD:   wpclsQueryTitle                             PUBLIC
--  PURPOSE:  This class method returns the default folder title for any
--            instance of the password protected folder class.  This title
--            is used if a title is not supplied in the WinCreateObject()
--            call.
--  INVOKED:  By the Workplace Shell, upon instantiation of the object
--            instance.
--

override wpclsInitData, classmethod;
--
--  METHOD:   wpclsInitData                               PUBLIC
--  PURPOSE:  This class method allows the initialization of any class data
--            items.  The overridden method simply obtains a module handle
--            to be used when accessing Presentation Manager resources, then
--            invokes the parent's default processing.
--  INVOKED:  By the Workplace Shell, upon loading the class DLL.
--

override wpclsQueryIcon, classmethod;
--
--  METHOD:   wpclsQueryIcon                                 PUBLIC
--  PURPOSE:  This class method returns the handle to the default icon for
--            the class. Allows setting of the icon to a wastebasket.
--  INVOKED:  By the Workplace Shell, upon instantiation of the object
--            instance.
--

override wpclsUnInitData, classmethod;
--
--  METHOD:   wpclsUnInitData                             PUBLIC
--
--  PURPOSE:  This class method allows the release of any class data items
--            or resources.  The overridden method releases the module handle
--            obtained by wpclsInitData, then invokes the parent's default
--            processing.
--  INVOKED:  By the Workplace Shell, upon unloading the class DLL.
--
