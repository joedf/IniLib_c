#include <stdio.h>
#include "ini.h"
#include "benchmark.h"

void item_print(char *name, char *str);

int main(int argc, char **argv) {
	
	#ifdef BENCHMARK_INI
		double start = get_time();
	#endif

	if (argc==3)
	{
		item_print(argv[2],ini_read(argv[1],"",argv[2],NULL));
	}
	else if (argc>=4)
	{
		item_print(argv[3],ini_read(argv[1],argv[2],argv[3],NULL));
	}
	else
	{
		printf("Inic (prenounced \"Inn-nik\") for Ini CLI\n");
		printf("Usage: inic FileName.ini [Section (optional)] [Key]\n");
	}
	
	#ifdef BENCHMARK_INI
		printf("time: %f",get_time()-start);
	#endif
	
	return INI_ERROR; //ini_error(INI_ERROR,1);
}

void item_print(char *name, char *str) {
	if (name!=NULL)
	{
		if (str!=NULL)
			printf("%s is \"%s\"\n", name, str);
		else
			printf("%s was not found.\n", name);
	}
	else
	{
		puts("Invalid Request.");
	}
}

