/*  */
Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

'@echo off'

'copy wastebsk.dll c:\os2\dll 1>nul: 2>nul:'

if rc then do
   say 'Error DLL could not be updated please re-boot'
   /* Remove bad entry */
   RetCode = SysDeregisterObjectClass( "WPWasteBskt");
   'pause'
   exit(1)
   end

RetCode = SysRegisterObjectClass( "WPWasteBskt", "wastebsk")

if RetCode then
   say 'WPWasteBskt Class registered'
else do
   say 'Error WPWasteBskt Class failed to register'
   /* Remove false entry */
   RetCode = SysDeregisterObjectClass( "WPWasteBskt");
   exit(1)
   end


RetCode = SysCreateObject( "WPWasteBskt", "Waste Basket", "<WP_DESKTOP>", "OBJECTID=<MyFolder>")

if RetCode then
   say 'WPWasteBskt Object created'
else do
   say 'Error creating object'
   exit(1)
   end

say 'A Waste Basket has been created'
