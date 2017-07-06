#include "nm.h"

/*void	handle_64(char *ptr)*/
/*{*/
	/*struct load_command		*lc;*/
	/*struct symtab_command	*sym;*/
	/*struct mach_header_64	*header;*/
	/*uint32_t				i;*/
	/*uint32_t				ncmds;*/

	/*i = 0;*/
	/*header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)*/
	/*ncmds = header->ncmds; // ncmds contient le nombre de load_command*/
	/*lc = (void *)ptr + sizeof(struct mach_header_64); // lc pointe sur le debut de la zone des load commands (juste apres le header)*/
	/*while (i < ncmds) // on iter autant de fois qu'il y a de load commands*/
	/*{*/
		/*if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB*/
		/*{*/
			/*sym = (struct symtab_command *)lc;*/
			/*[>printf("cmdsize =					%d\n", sym->cmdsize);<]*/
			/*[>printf("symbol tab offset =				%d\n", sym->symoff);<]*/
			/*[>printf("number of symbol table entries =		%d\n", sym->nsyms);<]*/
			/*[>printf("string table offset =				%d\n", sym->stroff);<]*/
			/*[>printf("string table size in byte =			%d\n", sym->strsize);<]*/
			/*[>RC;<]*/
			/*print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);*/
			/*break ;*/
		/*}*/
		/*lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize*/
		/*++i; // on incremente i*/
	/*}*/
/*}*/

void	handle_64(char *ptr)
{
	struct load_command		*lc;
	struct symtab_command	*sym;
	struct mach_header_64	*header;
	uint32_t				i;
	uint32_t				ncmds;

	i = 0;
	header = (void *)ptr; // header pointe sur ptr (octet 0 du binaire)
	ncmds = header->ncmds; // ncmds contient le nombre de load_command
	lc = (void *)ptr + sizeof(struct mach_header_64); // lc pointe sur le debut de la zone des load commands (juste apres le header)
	while (i < ncmds) // on iter autant de fois qu'il y a de load commands
	{
		if (lc->cmd == LC_SYMTAB) // si la cmd est egal a LC_SYMTAB
		{
			sym = (struct symtab_command *)lc;
			/*printf("cmdsize =					%d\n", sym->cmdsize);*/
			/*printf("symbol tab offset =				%d\n", sym->symoff);*/
			/*printf("number of symbol table entries =		%d\n", sym->nsyms);*/
			/*printf("string table offset =				%d\n", sym->stroff);*/
			/*printf("string table size in byte =			%d\n", sym->strsize);*/
			/*RC;*/
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break ;
		}
		lc = (void *)lc + lc->cmdsize; // on incremente de la taille d'une cmdsize
		++i; // on incremente i
	}
}
