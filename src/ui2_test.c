#include "despatcher.h"
#include "initializer.h"
#include "ui2.h"
#include <stdio.h>
#include "intern.h"


int main()
{
	printf ("\n UI Is running \n");
	UI* ui;
	ui = UserInterfaceCreate ("config.text");
	UserRun (ui);

	return 1;
}
