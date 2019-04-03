/*  */
Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

'@echo off'

RetCode = SysDeregisterObjectClass( "WPWasteBskt");

if RetCode then
    say 'Uninstall successfully completed for WPWasteBskt class'

say 'Re-boot NOW in order to release DLL'
'pause'
