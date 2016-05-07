#include <iostream>
#include <stdio.h>
#include <time.h>
#include <set>
#include <string.h>
using namespace std;
const int N = 100;
const int M = 100;
int n,m;
int arr[N];
int sum[N];
int ans;
int p[M];
int pn;
struct request
{

    int from;
    int budget;
    request(){}
    request( int from, int budget):from(from),budget(budget)
    {

    }
}req[M];


void input()
{
    cin>>n>>m;

    for(int i=0;i<m;i++)
    {
        int from;
        int budget;
        cin>>from>>budget;
        req[i] = request(from,budget);
    }
}

int dp[N][M];
int countRequest(int x,int p)
{
    int ret = 0;
    for(int i=0;i<m;i++)
    {
        if(req[i].from == x && req[i].budget>=p)
            ret++;
    }
    return ret;
}
void solve()
{
    int maxBudget = 0;
    for(int i=0;i<m;i++)
    {
        maxBudget = max(req[i].budget, maxBudget);
    }
    set<int> P;
    for(int i=0;i<m;i++)
    {
        P.insert(req[i].budget);
    }
    pn = 0;
    p[pn++]=0;
    for(set<int>::iterator si = P.begin();si!=P.end();si++)
    {
        p[pn++]=*si;
    }
    int ans = 0;
    for(int cuti = 0;cuti <n;cuti++)
    {
        memset(dp,0,sizeof(dp));
        for(int pi=0;pi<pn;pi++)
        {
            dp[cuti][pi] = p[pi]*countRequest(cuti,p[pi]);
            dp[(cuti+1)%n][pi] = p[pi]*countRequest((cuti+1)%n,p[pi]);
        }

        for(int pi = pn-2;pi>=0;pi--)
        {
            dp[cuti][pi] = max(dp[cuti][pi],dp[cuti][pi+1]);

            dp[(cuti+1)%n][pi] = max(dp[(cuti+1)%n][pi],dp[(cuti+1)%n][pi+1]);
        }

        for(int i=cuti-1;i>=1;i--)
        {
            for(int j = pn-1;j>=0;j--)
            {
                dp[i][j] = dp[i+1][j]+p[j]*countRequest(i,p[j]);

                if(j<pn-1)
                {
                    dp[i][j]=max(dp[i][j],dp[i][j+1]);
                }
            }
        }
        for(int i=cuti+2;i<n;i++)
        {
            for(int j=pn-1;j>=0;j--)
            {
                dp[i][j]=dp[i-1][j]+p[j]*countRequest(i,p[j]);
                if(j<pn-1)
                {
                    dp[i][j]=max(dp[i][j],dp[i][j+1]);
                }
            }
        }
        int ret = 0;
        if(cuti>0)
        {
            ret+=dp[1][0];
        }
        else
        {
            ret+=dp[0][0];
        }
        if(cuti<n-1)
        {
            ret+=dp[n-1][0];
        }
        else
        {
            ret+=dp[0][0];
        }

        ans=max(ret,ans);
    }

    cout<<ans<<endl;
    cout<<"The graph is C_"<<n<<". The maxBudget is "<<maxBudget<<endl;
}
int main()
{
    freopen("G-OUSP_on_circle.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int caseNum;
    cin>>caseNum;

    for(int _=1;_<=caseNum;_++)
    {
        cout<<"Case #"<<_<<": ";
        input();
        clock_t beg = clock();
        solve();
        clock_t en = clock();
        cout<<"Time use: "<<en-beg<<endl;
    }
    return 0;
}
