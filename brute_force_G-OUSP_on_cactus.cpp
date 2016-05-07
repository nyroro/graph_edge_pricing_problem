#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
using namespace std;
const int N = 100;
const int M = 100;
int n,m,rn;
bool mat[N][N];
int pmat[N][N];
int ans;
struct edg
{
    int to;
    int nxt;
}e[N*4];
int en;
int head[N*2];
struct request
{

    int from;
    int budget;
    request(){}
    request( int from, int budget):from(from),budget(budget)
    {

    }
}req[M];
void addedge(edg e[], int a, int b)
{
    e[en].to = b;
    e[en].nxt = head[a];
    head[a]=en++;

    e[en].to = a;
    e[en].nxt = head[b];
    head[b]=en++;
}
void input()
{
    cin>>n>>m>>rn;
    en=0;
    memset(head,-1,sizeof(head));
    for(int i=0;i<m;i++)
    {
        int a,b;
        cin>>a>>b;
        addedge(e,a,b);
    }

    for(int i=0;i<rn;i++)
    {
        int from;
        int budget;
        cin>>from>>budget;
        req[i] = request(from,budget);
    }
}

int priceEdge[N*4];
int dis[N];
int que[N*4];
bool vis[N];
const int inf = 0x7fffffff;

int calProfit()
{
    int qn,qi;
    qn=qi=0;

    for(int i=0;i<n;i++)
        dis[i]=inf,
        vis[i]=false;

    dis[0]=0;
    vis[0]=true;

    que[qn++]=0;
    while(qi<qn)
    {

        int t = que[qi++];
        vis[t]=false;
        for(int ei = head[t];ei>=0;ei = e[ei].nxt)
        {
            int pe = priceEdge[ei];
            int to = e[ei].to;

            if(dis[to]>dis[t]+pe)
            {
                dis[to]=dis[t]+pe;
                if(!vis[to])
                {
                    vis[to]=true;
                    que[qn++]=to;
                }
            }
        }
    }
    int ret=0;
    for(int i=0;i<rn;i++)
    {
        if(dis[req[i].from]<=req[i].budget)
        {
            ret+=dis[req[i].from];
        }
    }
    return ret;
}
void dfs(int maxBudget, int ei, int en)
{
    if(ei==en)
    {
        ans=max(ans,calProfit());
        return;
    }
    for(int i=0;i<=maxBudget;i++)
    {
        priceEdge[ei]=i;
        priceEdge[ei+1]=i;
        dfs(maxBudget, ei+2,en);
    }
}
void solve()
{
    int maxBudget = 0;
    for(int i=0;i<rn;i++)
    {
        maxBudget = max(req[i].budget, maxBudget);
    }
    ans = 0;

    dfs(maxBudget,0,en);

    cout<<ans<<endl;
    cout<<"Cactus"<<n<<' '<<m-(n-1)<<endl;
}
int main()
{

    freopen("G-OUSP_on_cactus.txt","r",stdin);
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
