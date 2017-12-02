#include<stdio.h>
#include<string.h>
int compare(char*find, char*str)
{
	int len = strlen(find);
	for(int i=0; i<len; ++i) {
		find[i] |= 1<<5;
		str[i] |= 1<<5;
	}
	if( !strcmp(find,str))	return 1;
	else	return 0;
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
