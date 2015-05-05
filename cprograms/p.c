#include <stdio.h>
#define PAGESIZE 22
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
ttyin(){
	char buf[BUFSIZ];
	FILE *efopen();
	static FILE *tty = NULL;

	if(tty == NULL)
		tty = efopen("/dev/tty","r");
	if(fgets(buf,BUFSIZ,tty)== NULL || buf[0] == 'q')
		exit(0);
	else {
		return buf[0];
	}


}
void print(FILE* fp,int pagesize)
{
	static int lines=0;
	char buf[BUFSIZ];
	while(fgets(buf,sizeof buf,fp) != NULL){
		if(++lines < pagesize)
			fputs(buf,stdout);
		else {
			buf[strlen(buf)-1] = '\0';
			fputs(buf,stdout);
			fflush(stdout);
			ttyin();
			lines=0;
		}
	}
}

void main(int argc,char *argv[]){
	int i;
	FILE *fp,*efopen();
	progname=argv[0];

	if (argc==1) {
		print(stdin,PAGESIZE);
	}
	else{
		for (i = 1; i < argc; i++) {
			fp=efopen(argv[i],"r");
			print(fp,PAGESIZE);
			fclose(fp);
		}

	}
	exit(0);

}
