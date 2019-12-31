#include<stdio.h>
#include<stdlib.h>

struct a
{
	int a;
	int score;
}b[30];

int main()
{
	int i,j;
	i=j=0;
	int flag,ans=0;
	while(1)
	{
		scanf("%d",&b[i].a);
		b[i].score=0;
		if(b[i].a==0) break;
		i++;	
 	} 
 	flag=i;
 	if(b[0].a==2) 
	{
		ans += 2;
		b[0].score=2;
	}
 	else if(b[0].a==1)
	{
		ans++;
		b[0].score=1;
	} 
	else if(b[0].a==0) 
	{
	 	printf("0");
	 	exit(0);
	}
 	for(i=1;i<flag;i++)
 	{
 		if(b[i].a==1)
 		{
 			ans++;
 			b[i].score=1;
		}
		if(b[i].a==2&&b[i-1].a==1)
		{
			ans+=2;
			b[i].score=2;
		}
		if(b[i].a==2&&b[i-1].a==2)
		{
			j=b[i-1].score+2;
			ans+=j;
			b[i].score=j;
		}
	 }
	printf("%d",ans);
}
 
