/*
This is a sample script, run automaticly at the startup of the editor.
All scripts in this directory starting with 'autostart' will be run
by the editor at startup. It is possible to add toolbars and menu
commands to the editor by these scripts and extend the editor in this 
way. The language used in the script is named Squirrel, and looks
a bit like C/C++ or Java. For more info about the language, please see
http://www.squirrel-lang.org/
For information on available functions, see the file doc/scripting.html
This script is (c) 2006 by Nikolaus Gebhardt.
*/


function printHello()
{
  print("Hello from the example script.\n\n");
  print("You can edit this script in the 'scripts' directory.\n");
}


// add the hello function to the tool menu
editorRegisterMenuEntry("printHello", "Print Hello into the log (example script)");

// add the hello function as toolbar icon
editorRegisterToolbarEntry("printHello", "Prints 'hello' (example script)",
	 "scripts\\examplescript_icon.bmp");
