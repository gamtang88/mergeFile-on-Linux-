#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define KB 1024

int sumFile (char *argv[],int);
void sepFile (char *argv[],int,int,int);
char tot[] = "totalFileName";

int main(int argc,char *argv[])
{
	int i,fd,inputNum,outputNum,sscanfNum1,sscanfNum2,totalByte ;
	char BUF[KB],test[10];
	char c;
	for(i=1;i<3;i++)
	{
		if(0<(sscanf(argv[i],"%*d%s",test))) //입력받는 데이터가 숫자아닐때
		{
			puts("insult data again please.");exit(1);
		}
	}
	sscanfNum1=sscanf(argv[1],"%d",&inputNum);
	sscanfNum2=sscanf(argv[2],"%d",&outputNum);
	
	
	printf("Return_sscanf.argv[1]=%d, Return_sscanf.argv[2]=%d \n",sscanfNum1,sscanfNum2);

	if(!sscanfNum1 || sscanfNum1==EOF || !sscanfNum2 || sscanfNum2==EOF || argc!=inputNum+outputNum+3 ) //inputNum에 이상있을경우
	{
		printf("insert data again\n");exit(1);
	}
	for(i=0;i<inputNum;i++)  //합칠파일이 존재하지 않을경우
        {
                if(0<(fd=open(argv[i+3],O_RDONLY | O_CREAT | O_EXCL)))
		{
			close(fd);
                	puts("Inputfilename does not exist.");exit(1);
		}
        }

	for(i=0;i<outputNum-1;i++)  //출력파일이 같은이름일때,출력파일이 존재할때
	{
		if(!strcmp(argv[inputNum+3+i],argv[inputNum+4+i]))
	 	{
 			printf("Can't seperate to same filename. Try again.\n");
			exit(1);
		}

		else if(0>(fd=open(argv[inputNum+3+i],O_RDONLY | O_CREAT | O_EXCL, 0664))) //나누려는 파일이름이 이미 존재할경우
		{
			printf("Same filename already exist. overwrite?(y/n)");
			c=getchar();
			if(c=='y')break;
			else
			printf("Try again.\n");exit(1);

		}
	}
	
	totalByte = sumFile(argv,inputNum);
	sepFile(argv,totalByte,outputNum,inputNum);
}

int sumFile(char *argv[],int inputNum)
{
	char buf[KB];
	int i,totalFD,subFD,readNum,totalByte=0;

	totalFD=open(tot,O_WRONLY | O_CREAT |O_TRUNC, 0664);
	for(i=0;i<inputNum;i++)
	{
		subFD=open(argv[i+3],O_RDONLY);
		while(0<(readNum = read(subFD,buf,KB)))
		{
			write(totalFD,buf,readNum);
			totalByte += readNum;
		}
		close(subFD);
	}
	close(totalFD);
	return totalByte;	
}

void sepFile(char *argv[],int totalByte, int outputNum,int inputNum)
{
	int a,i,totalFD,subFD,readNum,readByte,writeNum;
	int sepByte= totalByte/outputNum;
	char *sepBuf=(char*)malloc(sizeof(char)*sepByte+1); //버퍼크기는 나머지 여유분포함
	

	totalFD=open(tot,O_RDONLY); //토탈파일을 연다

	a=0;
	for(i=0;i<outputNum;i++) //출력수 만큼 반복
	{
		if(a<totalByte%outputNum) //나머지가있을경우,나머지개수만큼 실행
		{
			readNum=read(totalFD,sepBuf,sepByte+1); //1byte더읽는다.
			a++;
		}
		else //나머지수만큼 1씩분배하고 그이외는 몫만큼 읽어들인다.
		readNum=read(totalFD,sepBuf,sepByte);

		subFD=open(argv[inputNum+i+3],O_WRONLY | O_CREAT | O_TRUNC,0664); //출력파일을 연다
		write(subFD,sepBuf,readNum); //해당 바이트수만큼 쓴다.
		close(subFD);
	}

	close(totalFD);

	puts("The operation completed successfully!");
}

