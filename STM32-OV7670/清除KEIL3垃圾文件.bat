::bat文件相当于在CMD中顺序执行文件中的命令,双冒号"::"相当于注释(REM 命令)
:: 
::
 @echo off
 echo 清除KEIL3工程的垃圾文件→
 pause
 del *. 	
 del *.lnp 
 del *.lst  
 del *.m51  
 del *.obj  
::del *.opt  
 del *.plg  
 del *.bak  
 del *.orc
 del *.map
 del *.dep
 @echo off
 del obj/q
