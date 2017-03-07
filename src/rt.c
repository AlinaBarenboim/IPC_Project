#include "despatcher.h"
#include "initializer.h"
#include <stdio.h>


initStatus main()
{
	printf ("\n RT Is running\n");
	return InitializeSystem ();
}
