#include<QtWidgets/qapplication.h>
#include "TTFFile.h"
#include "GlyphDrawer.h"

#include <stdio.h>  /* defines FILENAME_MAX */


#include <direct.h>
#define GetCurrentDir _getcwd



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	printf("The current working directory is %s", cCurrentPath);

	GUI::GlyphDrawer window("Resources/ITCBLKAD.TTF");//../../Reading_TTF_files/

	window.show();

	return a.exec();
}
