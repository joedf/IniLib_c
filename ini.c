#include "ini.h"

#define _CRLF "\r\n"
#define fputs_win(A,B) fputs(A,B); fputs(_CRLF,B);
#define test() puts("test");

int INI_ERROR = 0;

//      [INI Library Functions]
//------------------------------------
char *ini_read(char *file, char *section, char *key, char *dvalue) {

	FILE* pFile = fopen(file,"rb");
	if (pFile == NULL) {
		INI_ERROR = ini_error(INI_ERROR_FILEREAD,INI_ERROR_VERBOSE);
		return dvalue;
	} else {
		//int slen = 2+strlen(section);
		//char *s = malloc(slen+1); sprintf(s,"[%s]",strlower(section)); s[slen]='\0';
		//char *k = strlower(key); //int klen = strlen(k);
		char line[INI_MAXLINE];
		//char keyname[INI_MAXLINE];
		int in_section = 0, found_section = 0, k_end, no_section=0;
		//int linenum=0;
		if (strlen(section)==0 || section == NULL)
			no_section=1;
		
		while(fgets(line,sizeof(line),pFile) != NULL)
		{
			//linenum+=1;
			int nr, len = strlen(line);
			//if(line[len-1] == '\n') line[len-1] = '\0'; // strip trailing '\n' if it exists
			for (nr=len;nr>1;nr--) {
				if(isspace((unsigned char)line[nr-1])) //if(line[nr-1]=='\r' || line[nr-1]=='\n')
					line[nr-1]='\0';
 				else
				{
					len = strlen(line);
					break;
 				}
			}
			strcpy(line,ini_fix(line)); len = strlen(line);

			if (line[0]=='[' && line[len-1]==']') //if section
			{
				strcpy(line,strlower(line));

				/*
				printf("DEBUG: ini_section(line) = \"%s\" : (%d) original: \"%s\"\n",
							   ini_section(line),
							   (strcmp(strlower(ini_section(line)),strlower(section))==0),
							   strlower(section));
	   						   */

				//if (((strstr(line,s)-line)+1)>=0) {
	   			if (strcmp(strlower(ini_section(line)),strlower(section))==0) { //found section
				   in_section=1;
				   found_section=1;
					if (strlen(key)==0 || key==NULL)
					{
						//free(s);
						//free(k);
						fclose(pFile);
						return section;
					}
				} else {
					in_section=0;
				}
			}
			if (in_section || no_section)
			{
				k_end=((strchr(line,'=')-line)+1);
				if (k_end>0)
				{
					//strcpy(keyname,strlower(line)); keyname[k_end-1]='\0';

					//if (strcmp(keyname,k)==0)
					if ((ini_key(line)!=NULL) && (strcmp(strlower(ini_key(line)),strlower(key))==0)) //found key
					{
						//free(s);
						//free(k);
						fclose(pFile);
						return substr(line,k_end,(strlen(line)-k_end));
					}
				}
				else if (k_end == 0)
				{
					INI_ERROR = ini_error(INI_ERROR_PARSEINVALID,INI_ERROR_VERBOSE);
				}
			}
			else if (found_section && !in_section)
				break; //return section;
		}
		//free(s);
		//free(k);
		fclose(pFile);
	}
	
	return dvalue;
}

int ini_write(char *file, char *section, char *key, char *value) {
/*
	open original file
	open output file
	allocate a line buffer that is large enough
	read a line from the original file
	do
	  return an error if the buffer is too small
	  manipulate the line
	  write the manipulated line to the output file
	  read a line from the original file
	loop until read returns nothing
	then delete the original file with
	  and replace it with the output file
*/
	char *tmp = malloc(strlen(file)+8);
	sprintf(tmp,"%s.tmp",file);

	FILE* pFile = fopen(file,"rb");

	if (pFile != NULL) {
		FILE* pTmp = fopen(tmp,"wb");
		
		if (pTmp != NULL)
		{
			char line[INI_MAXLINE], o_line[INI_MAXLINE];
			char* current_section = malloc(INI_MAXLINE);
			//int linenum=0
			int len, updated=0, nr;
			while(fgets(line,sizeof(line),pFile) != NULL)
			{
				//linenum+=1;
				
				len = strlen(line);
				/*
					if(line[len-1] == '\n') line[len-1] = '\0'; // strip trailing '\n' if it exists
					len = strlen(line);
					if(line[len-1] == '\r') line[len-1] = '\0'; // strip trailing '\r' if it exists
					len = strlen(line);
					if(line[len-1] == '\n') line[len-1] = '\0'; // strip trailing '\n' if it exists
					len = strlen(line);
					if(line[len-1] == '\r') line[len-1] = '\0'; // strip trailing '\r' if it exists
					len = strlen(line);
				*/
				for (nr=len;nr>1;nr--) {
					if(isspace((unsigned char)line[nr-1])) //if(line[nr-1]=='\r' || line[nr-1]=='\n')
						line[nr-1]='\0';
	 				else
					{
						len = strlen(line);
						break;
	 				}
				}
				
				strcpy(o_line,line);
				strcpy(line,ini_fix(line)); len = strlen(line);
				
				//strcpy(o_line,line); //uncomment this line, to trim comments from ini...
				
				//----------------------
				if (!updated)
				{
					if (ini_section(line)!=NULL) //Section Indicator line
					{
						strcpy(current_section,ini_section(line));
						fputs_win(line,pTmp);
					}
					else //parse line: key=value
					{
						//if correct section...
						//printf("%s =x= %s\n", current_section, section);
						if (strcmp(strlower(current_section),strlower(section))==0)
						{
							//putchar('_');
							//puts(ini_key(line));
	  						//putchar('_');
							if ((ini_key(line)!=NULL) && (strcmp(strlower(ini_key(line)),strlower(key))==0)) //found key
							{
								//int k_end = strlen(ini_key(line));
								//value == substr(line,k_end,(strlen(line)-k_end));
								fprintf(pTmp,"%s=%s%s",key,value,_CRLF);
								updated=1;
							}
							else //not found key
							{
								fputs_win(o_line,pTmp);
							}
						}
						else //continue copying...
						{
							fputs_win(o_line,pTmp);
						}
					}
				}
				else //continue copying...
				{
					fputs_win(o_line,pTmp);
				}
			}
			
			if (!updated)
			{
				if (strcmp(strlower(current_section),strlower(section))!=0)
				{
					fputs(_CRLF,pTmp);
					fprintf(pTmp,"[%s]%s",section,_CRLF);
				}
				fprintf(pTmp,"%s=%s%s",key,value,_CRLF);
				updated=1;
			}

			fclose(pTmp);
			fclose(pFile);
			if (updated)
			{
				//delete old one, rename new one to old name...
				if ((remove(file)!=0) && (rename(tmp,file)!=0))
				{
					free(tmp);
					INI_ERROR = ini_error(INI_ERROR_FILE,INI_ERROR_VERBOSE);
					return 0;
				}
				else
				{
					free(tmp);
					return 1;
				}
			}
			else
			{
				free(tmp);
				INI_ERROR = ini_error(INI_ERROR_PARSEINVALID,INI_ERROR_VERBOSE);
				return 0;
			}
		}
		else
		{
			free(tmp);
			fclose(pFile);
			INI_ERROR = ini_error(INI_ERROR_FILEREAD,INI_ERROR_VERBOSE);
			return 0;
		}
		fclose(pFile);
	}
	free(tmp);
	
	INI_ERROR = ini_error(INI_ERROR_FILEREAD,INI_ERROR_VERBOSE);
	return 0;
}

//      [INI Internal Functions]
//------------------------------------
char *ini_fix(char *str) {
	int i=(strchr(str,';')-str)+1; if (i>=0) str[i-1]='\0'; //Support for ;comment lines
	return ini_strip(str);
}

char *ini_section(char *line) {
	if (line==NULL)
		return NULL;
	int len = strlen(line);
	if (line[0]=='[' && line[len-1]==']') //if section
	    #if INI_AUTOTRIM
			return ini_strip(substr(line, 1, len-2));
		#else
		    return substr(line, 1, len-2);
		#endif
	else
		return NULL;
}

char *ini_key(char *line) {
	if (line==NULL)
		return NULL;
	int k_end=(strchr(line,'=')-line)+1;
	if (k_end>0)
	{
		char *keyname = malloc(INI_MAXLINE);
		strcpy(keyname,strlower(line)); keyname[k_end-1]='\0';
		#if INI_AUTOTRIM
			return ini_strip(keyname);
		#else
		    return keyname;
		#endif
	}

	if (k_end==0)
	{
		INI_ERROR = ini_error(INI_ERROR_PARSEINVALID,INI_ERROR_VERBOSE);
	}
	return NULL;
}

int ini_error(int error, int v) {
	if (v) {
		if (error!=INI_ERROR_NONE){fprintf(stderr,"\nIni_Error: 0x%0.2X - ", error);}
		switch(error) {
			case INI_ERROR_NONE         : return INI_ERROR_NONE;
			case INI_ERROR_FILE         : fprintf(stderr,"Generic File Error\n"); break;
			case INI_ERROR_FILEREAD     : fprintf(stderr,"File Read Error\n"); break;
			case INI_ERROR_FILEWRITE    : fprintf(stderr,"File Write Error\n"); break;
			case INI_ERROR_MEMORY       : fprintf(stderr,"Generic Memory Error\n"); break;
			case INI_ERROR_MEMORYREAD   : fprintf(stderr,"Memory Read Error\n"); break;
			case INI_ERROR_MEMORYWRITE  : fprintf(stderr,"Memory Write Error\n"); break;
			case INI_ERROR_MEMORYALLOC  : fprintf(stderr,"Memory Allocation Error\n"); break;
			case INI_ERROR_PARSE        : fprintf(stderr,"Parse Error\n"); break;
			case INI_ERROR_PARSEINVALID : fprintf(stderr,"Parse Invalid Error\n"); break;
			default                     : fprintf(stderr,"Unknown Error\n"); return INI_ERROR_UNKNOWN;
		}
	}
	return error;
}

char *ini_strip(char *str) {
	int i, j, len=strlen(str);
	for (i=0;isspace((unsigned char)str[i]);i++)
		{ /*nothing...*/ }
	int m = strlen(str)-i;
	char *out = malloc(m+1);
	memcpy(out+0,str+i,m+1);
	for (j=strlen(out);isspace((unsigned char)out[j-1]);j--)
		{ /*nothing...*/ }
	out[j]='\0';
	char *new_p = realloc(out,j+1);
	if (new_p == NULL) //...handle error
	{
		ini_error(INI_ERROR_MEMORYALLOC,INI_ERROR_VERBOSE);
		return out;
	}
	return new_p;
}

//      [INI Utility Functions]
//------------------------------------
char *strlower(char *str) {
	#if !(INI_CASESENSITIVE)
		int i, len=strlen(str);
		char *out=malloc(len+1);
		for (i=0;i<len;i++)
			out[i]=tolower(str[i]);
		out[len]='\0';
		//free(out);
		return out;
	#else
		return str;
	#endif
}

char *substr(char *str, int start, int length) {
	int i, len=strlen(str);
	if ((length+start)<=len || start>0 || length>0){
		char *out=calloc(len+1,sizeof(char));
		for (i=start;i<(length+start);i++)
			out[i-start]=str[i];
		//out[len]='\0';
		return out;
	}
	else
	{
		return NULL;
	}
}


