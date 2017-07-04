#include <stdio.h>
#include <string.h>

void	sort_param(char **arguments)
{
	char	*tmp;
	int	find;
	int	i;

	find = 1;
	while (find)
	{
		find = 0;
		i = 0;
		while (arguments[i + 1])
		{
			if (strcmp(arguments[i], arguments[i + 1]) > 0)
			{
				tmp = arguments[i];
				arguments[i] = arguments[i + 1];
				arguments[i + 1] = tmp;
				find = 1;
			}
			++i;
		}
	}
}

int	main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
		printf("%s\n", argv[i]);
	printf("\n\n\n\n");
	sort_param(argv);
	for (int i = 1; i < argc; ++i)
		printf("%s\n", argv[i]);
}
