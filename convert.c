#include <stdio.h>
#include <stdlib.h>

/*int main(int argc, char** argv)*/
/*{*/
/*FILE* input = NULL;*/
/*FILE* output = NULL;*/
/*unsigned long size = 0;*/
/*unsigned long itr = 0;*/
/*unsigned char* buffer;*/
/*uint32_t b0,b1,b2,b3;*/
/*uint32_t res;*/

/*if(argc != 3)*/
/*return 0;*/
/*input = fopen(argv[1],"rb");*/
/*output = fopen(argv[2],"wb");*/

/*if(!input || !output)*/
/*fprintf(stderr,"could not open file");*/
/*fseek(input,0,SEEK_END);*/
/*size = ftell(input);*/
/*fseek(input,0,SEEK_SET);*/

/*buffer = (unsigned char*)malloc(size);*/

/*fread(buffer,1,size,input);*/
/*fclose(input);*/
/*for(itr = 0;itr<size;itr++)*/
/*{*/
/*b0 = (buffer[itr] & 0x000000ff) << 24u;*/
/*b1 = (buffer[itr] & 0x0000ff00) << 8u;*/
/*b2 = (buffer[itr] & 0x00ff0000) >> 8u;*/
/*b3 = (buffer[itr] & 0xff000000) >> 24u;*/
/*res = b0 | b1 | b2 | b3;*/
/*printf("%" PRIX32 "\n", res);*/

/*fwrite(&res,1,1,output);*/
/*}*/
/*fclose(output);*/
/*return 0;*/
/*}*/
#include<stdio.h>
int convert(int );
int main()
{
	unsigned int num,r;
	char *str = "Bonjour les gens";
	char str1[17] = {0};
	int i = 0;

	while (str[i])
	{
		printf("%x ", str[i]);
		++i;
	}
	printf("\n");
	i = 0;
	while (str[i])
	{
		str1[i] = convert(str[i]);
		++i;
	}
	i = 0;
	while ( str1[i])
	{
		printf("%x ", str1[i]);
		++i;
	}
}
int convert(int a)
{
	uint32_t	b;

	b = ((a >> 24) & 0xff) | ((a << 8) & 0xff0000) |
		((a >> 8) & 0xff00) |
		((a << 24) & 0xff000000);
	return (b);
	/*int b0,b1,b2,b3;*/
	/*b0= (num & 0x000000FF)>>0;*/
	/*b1= (num & 0x0000FF00)>>8;*/
	/*b2= (num & 0x00FF0000)>>16;*/
	/*b3= (num & 0xFF000000)>>24;*/

	/*num= (b0<<24) | (b1<<16) | (b2<<8) | (b3<<0) ;*/

	/*printf("%x ",num);*/
	/*return num;*/
}
