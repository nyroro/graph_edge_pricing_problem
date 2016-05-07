#include <iostream>
#include <stdio.h>
#include <time.h>
using namespace std;
const int N = 100+4;
const int M = 100+4;
const int W = 100+4;
int n,m;
int arr[N];
int sum[N];
int ans;
struct request
{
    int from;
    int budget;
    request(){}
    request(int from, int budget):from(from),budget(budget)
    {

    }
}A[M],B[M];

int dp[N][W][W];
int mdp[W][W];
int an,bn;
void input()
{
    cin>>n>>m;
    an = bn = 0;
    for(int i=0;i<m;i++)
    {
        char type;
        int from;
        int budget;
        cin>>type>>from>>budget;
        if(type == 'A')
        {
            A[an++] = request(from,budget);
        }
        else
        {
            B[bn++] = request(from,budget);
        }
    }
}

void solve()
{
    int aMaxBudget, bMaxBudget;
    aMaxBudget = bMaxBudget = 0;
    for(int i=0;i<an;i++)
    {
        aMaxBudget = max(aMaxBudget, A[i].budget);
    }
    for(int i=0;i<bn;i++)
    {
        bMaxBudget = max(bMaxBudget, B[i].budget);
    }
    int w = aMaxBudget + bMaxBudget;

    for(int i=0;i<w;i++)
    {
        for(int j=0;j<w;j++)
        {
            dp[0][i][j] = 0;
            mdp[i][j] = 0;
        }
    }
    for(int i=1;i<=n;i++)
    {
        for(int total=0;total<w;total++)
        {
            for(int j=0;j<=total;j++)
            {
                int x = j;
                int y = total - j;
                int cntA = 0,cntB = 0;
                for(int k = 0;k<an;k++)
                {
                    if(A[k].from == i && A[k].budget >= x)
                    {
                        cntA++;
                    }
                }

                for(int k=0;k<bn;k++)
                {
                    if(B[k].from == i && B[k].budget >= y)
                    {
                        cntB++;
                    }
                }
                dp[i][x][y] = mdp[total][x]+x*cntA+y*cntB;
            }
        }
        for(int x=0;x<w;x++)
        {
            for(int y=0;x+y<w;y++)
            {
                mdp[x+y][x] = max(mdp[x+y][x-1],dp[i][x][y]);
            }
        }
    }
    int ans = 0;
    for(int x=0;x<w;x++)
    {
        for(int y=0;x+y<w;y++)
        {
            ans=max(ans,mdp[x+y][y]);
        }
    }
    cout<<ans<<endl;
    cout<<"The graph is C_"<<n<<". The maxBudget is "<<max(aMaxBudget,bMaxBudget)<<endl;
}
int main()
{
    freopen("G-SUSP_on_circle.txt","r",stdin);
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
