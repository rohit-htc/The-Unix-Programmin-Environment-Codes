#include <stdio.h>
#include <signal.h>

char *progname;
char *ps="ps ax";
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
int strindex(char *s,char *t)
{
	int i,n;
	n=strlen(t);
	for (i = 0; s[i] != '\0'; i++) {
		if(strncmp(s+i,t,n) == 0)
			return i;
	}
	return -1;
}

int main(int argc, char *argv[])
{
	FILE *fin,*popen();
	char buf[BUFSIZ];
	int pid;

	progname=argv[0];
	if((fin=popen(ps,"r"))==NULL){
		fprintf(stderr,"%s can't run %s",progname,ps);
		exit(1);
	}

	fgets(buf, sizeof buf,fin);
	fprintf(stderr,"%s",buf);
	while(fgets(buf,sizeof buf,fin)!=NULL){
		buf[strlen(buf)-1]='\0';
		if(argc==1 || strindex(buf,argv[1])>=0){
			fprintf(stderr,"%s? ",buf);
			if(ttyin()=='y'){
				sscanf(buf,"%d",&pid);
				fprintf(stderr,"%d",pid);
				kill(pid,SIGKILL);

			}
		}
	}
	return 0;
}
