#include <stdio.h>

char *progname;
FILE *efopen(char *file,char *mode){
	FILE *fp,*fopen();
	extern char* progname;

	if ((fp=fopen(file,mode)) != NULL) {
		return fp;
	}
	fprintf(stderr,"%s: can't open file %s mode %s\n",progname,file,mode);
	exit(1);
}
char ttyin(){
	char buf[BUFSIZ];
	FILE *efopen();
	static FILE *tty = NULL;

	for(;;){
		if(tty == NULL)
			tty = efopen("/dev/tty","r");
		if(fgets(buf,BUFSIZ,tty)== NULL || buf[0] == 'q')
			exit(0);
		else {
			return buf[0];
		}
	}
}
void pick(char *s){
	//send it to standard error so that it doesnt go to stdout
	fprintf(stderr,"%s? ",s);
	if(ttyin()=='y')
		printf("%s\n",s);

}
int main(int argc, char *argv[])
{
	int i;
	char buf[BUFSIZ];
	progname=argv[0];
	if(argc == 2 && strcmp(argv[1],"-")==0){
		while (fgets(buf,sizeof buf,stdin) != NULL) {
			buf[strlen(buf)-1]='\0';
			pick(buf);
		}
	}
	else {
		for (i = 1; i < argc; i++) {
			pick(argv[i]);
		}
	}
	return 0;
}
