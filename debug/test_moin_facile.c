#include <stdio.h>

int	une_globale = 40;
static int	une_globale_2;
int i;

void	print()
{
}

int main(void)
{
	 int	une = 90;

	printf("La globale vaut: %d\n", une_globale);
	printf("La globale vaut: %d\n", une_globale_2);
	return (0);
}
