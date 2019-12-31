//并查集思路的一道题 
#include<stdio.h>
int father[30001],num[30001],tmp,vis[30001],cnt[30001];
int Find(int x)
{
	if(x != father[x])
		father[x] = Find(father[x]);
	return father[x];
} 

int main()
{
	int N,M,flag,fcur,ans=0;
	scanf("%d%d",&N,&M);
	for(int i=0; i<=M; i++)
		father[i] = i;
	
	for(int i=1; i<=M; i++){
		int cnt,lf;
		scanf("%d",&cnt);
		flag = 1;
		for(int j=1; j<=cnt; j++){
			int cur;
			scanf("%d",&cur);
			if(num[cur] == 0){
				if(j==1){
					num[cur] = cur;
					lf = father[cur];
				}
				num[cur] = cur;
				father[cur] = lf;
			}
			else{
				if(j==1)
					lf = Find(cur);
				int fcur = Find(cur);
				father[fcur] = lf;
			}
		}
	}
	for(int i=1; i<=N; i++){
		int fx = Find(i);
		cnt[fx]++;
		
		if(ans < cnt[fx])
			ans = cnt[fx];
	}
	printf("%d\n",ans);
	return 0;
}
