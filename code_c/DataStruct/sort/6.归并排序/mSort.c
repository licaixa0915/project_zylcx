#include <stdio.h>

#define MAX 6
void merge(int sr[],int tr[],int i,int m,int n);
void msort(int sr[],int tr1[],int s,int t)
{
	int m;
	int tr2[MAX + 1] = {0}; //下标为0的位置不填充数据
	if(s == t)
	{
		tr1[s] = sr[s];	
	}
    else
    {
		m = (s + t) / 2;
		msort(sr,tr2,s,m);  //把数组分为1-m左边的部分
		msort(sr,tr2,m + 1,t); //把数组分为m + 1 –t右边的部分
		merge(tr2,tr1,s,m,t); //对划分好的组进行排序。
	}
}

void merge(int sr[],int tr[],int i,int m,int n)
{
	 int j,k,l;
	//找到较小的填充到tr数组中
    for(j = m + 1,k = i;i <= m && j <= n;)
	{
		if(sr[i] < sr[j])
		{
		    tr[k++] = sr[i++];	
		}
        else
        {
			tr[k++] = sr[j++];	
		}
	}		

	if(i <= m)
	{
		for(l = 0;l <= m - i;l++)
		{
			tr[k++] = sr[i + l];	
		}
	}
				
	if(j <= n)
	{
		for(l = 0;l <= n - j;l++)
		{
			tr[k++] = sr[j + l];	
		}
	}
}

int main()
{
	int sr[ ] = {-1,14,12,15,13,11,16};
	int tr1[7] = {0};
    int i = 0;

	msort(sr,tr1,1,6);
    
    for (i = 1; i < 7; i++)
    {
        printf("%d ", tr1[i]);
    }
    printf("\n");
	return 0;	
}

