#include<stdio.h>
#include<string.h>
int compare(char*find, char*str)
{
	int len = strlen(find);
	int len2 = strlen(str);
	if(len2-len == 0 ||(len2-len == 1 && str[len2-1] == '.')) {
		for(int i=0; i<len; ++i) {
			find[i] |= 1<<5;
			str[i] |= 1<<5;
		}
		if( !strncmp(find,str,len))	return 1;
		else	return 0;
	}
	return 0;
}

int main ()
{
	char a[] = "hello";
	char b[] = "input.txt";
	char str[1000]="";
	long long int count = 0;
	FILE* pf = fopen(b,"r");
	while(fscanf(pf, "%s", str)!=EOF) {
		count += compare(a,str);
	}
	printf("%lld\n",count);
	return 0;
}
