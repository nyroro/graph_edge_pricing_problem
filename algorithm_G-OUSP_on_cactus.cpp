#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <set>
using namespace std;
const int N = 100;
const int M = 100;
int n,m,rn;
int p[N*4];
int pn;
int ans;
struct edg
{
    int from;
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

int calProfit(int x,int p)
{
    int ret = 0;
    for(int i=0;i<rn;i++)
    {
        if(req[i].from == x && req[i].budget>=p)
            ret++;
    }
    return ret*p;
}

void addedge(edg e[], int &en,int head[], int a, int b)
{
    e[en].from = a;
    e[en].to = b;
    e[en].nxt = head[a];
    head[a]=en++;

    e[en].from = b;
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
        addedge(e,en,head,a,b);
    }

    for(int i=0;i<rn;i++)
    {
        int from;
        int budget;
        cin>>from>>budget;
        req[i] = request(from,budget);
    }
}

int pre[N*2];
bool vis[N*2];
bool visE[N*4];
int circleNum;
int inCircle[N*2][3];
int circleRoot[N];
int circleEnd[N];
void dfs_find_circle(int px,int x)
{
    pre[x]=px;
    vis[x]=true;
    for(int ei = head[x];ei>=0;ei = e[ei].nxt)
    {
        int nxt = e[ei].to;
        if(nxt == px)continue;
        if(visE[ei])continue;
        if(!vis[nxt])
        {
            dfs_find_circle(x,nxt);
        }
        else
        {

            //Record circle root and end
            circleRoot[circleNum] = nxt;
            circleEnd[circleNum] = x;

            //Record each point belong to the circle
            int tmp = x;
            while(tmp!=nxt)
            {
                inCircle[tmp][0]++;
                inCircle[tmp][inCircle[tmp][0]] = circleNum;
                tmp=pre[tmp];
            }
            inCircle[tmp][0]++;
            inCircle[tmp][inCircle[tmp][0]] = circleNum;


            circleNum++;
        }
        visE[ei]=visE[ei^1]=true;
    }
}

edg te[N*4];
int ten;
int thead[N*2];

bool inSameCircle(int x, int y)
{
    for(int i=0;i<inCircle[x][0];i++)
    {
        for(int j=0;j<inCircle[y][0];j++)
        {
            if(inCircle[x][i+1]==inCircle[y][j+1])
                return true;
        }
    }
    return false;
}
void construct_tree()
{
    ten = 0;
    memset(thead,-1,sizeof(thead));
    for(int i=0;i<n;i++)
    {
        if(inCircle[i][0]==2)
        {
            int f = inCircle[i][1];
            int t = inCircle[i][2];
            addedge(te,ten,thead,n+f,n+t);
        }
    }
    for(int ei=0;ei<en;ei+=2)
    {
        int f = e[ei].from;
        int t = e[ei].to;
        if(!inSameCircle(f,t))
        {
            if(inCircle[f][0]>0)f = n+inCircle[f][1];
            if(inCircle[t][0]>0)t = n+inCircle[t][1];
            addedge(te,ten,thead,f,t);
        }
    }
}
int dp[N*4][N*4];
int cdp[N*4][N*4];
int circleArr[N];
bool isInCircle(int x,int circleIndex)
{
    for(int i=0;i<inCircle[x][0];i++)
    {
        if(inCircle[x][i+1]==circleIndex)
            return true;
    }
    return false;
}
void calculateInCircle(int circleArr[], int n)
{
    int ret[N];
    memset(ret,0,sizeof(ret));
    for(int cuti = 0;cuti<n;cuti++)
    {
        //Copy dp in circle to cdp
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<pn;j++)
            {
                cdp[i][j]=dp[circleArr[i]][j];
            }
        }
        //Calculate the cut line

        for(int i=cuti-1;i>=1;i--)
        {
            int maxProfit = 0;
            for(int j=pn-1;j>=0;j--)
            {
                maxProfit = max(maxProfit,cdp[i+1][j]);
                cdp[i][j]+=maxProfit;
            }
        }

        for(int i=cuti+2;i<n;i++)
        {
            int maxProfit = 0;
            for(int j=pn-1;j>=0;j--)
            {
                maxProfit = max(maxProfit,cdp[i-1][j]);
                cdp[i][j]+=maxProfit;
            }
        }

        if(cuti>0)
        {
            int maxProfit = 0;
            for(int j=pn-1;j>=0;j--)
            {
                maxProfit = max(maxProfit,cdp[1][j]);
                cdp[0][j]+=maxProfit;
            }
        }
        if(cuti<n-1)
        {
            int maxProfit = 0;
            for(int j=pn-1;j>=0;j--)
            {
                maxProfit = max(maxProfit,cdp[n-1][j]);
                cdp[0][j]+=maxProfit;
            }
        }



        //Update cdp[root] to ret[]
        for(int j=0;j<pn;j++)
            ret[j]=max(ret[j],cdp[0][j]);
    }
    //Update ret to dp[root]
    for(int j=0;j<pn;j++)
    {
        dp[circleArr[0]][j] = ret[j];
    }

}
void dfs_calculate(int px,int x)
{

    for(int ei = thead[x];ei>=0;ei = te[ei].nxt)
    {
        int nxt = te[ei].to;
        if(nxt==px)continue;

        dfs_calculate(x,nxt);
        if(nxt>=n)
        {
            //nxt is circle
            nxt = circleRoot[nxt-n];
            if(x>=n&&isInCircle(nxt,x-n))
            {
                //it is a dummy edge
                //Do nothing
                continue;
            }
        }
        //Update dp[nxt] to dp[pre[nxt]]
        int maxProfit = 0;

        for(int i=pn-1;i>=0;i--)
        {
            maxProfit = max(maxProfit, dp[nxt][i]);
            dp[pre[nxt]][i]+=maxProfit;
        }

    }

    if(x<n)
    {
        return;
    }
    else
    {
        //A Circle Node
        int cn=0;
        int tmp = circleEnd[x-n];
        circleArr[cn++]=tmp;
        while(tmp!=circleRoot[x-n])
        {
            tmp = pre[tmp];
            circleArr[cn++]=tmp;
        }
        //Reverse
        for(int i=0;i<cn/2;i++)
        {
            swap(circleArr[i],circleArr[cn-1-i]);
        }
        calculateInCircle(circleArr, cn);
    }
}
void solve()
{
    int maxBudget = 0;
    for(int i=0;i<rn;i++)
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

    ans = 0;

    //Find circle
    circleNum = 0;
    memset(vis,false,sizeof(vis));
    memset(visE,false,sizeof(visE));
    for(int i=0;i<n;i++)
    {
        inCircle[i][0]=0;
    }
    dfs_find_circle(-1,0);
    //Construct Character Tree
    construct_tree();

    //Calculate
    int root = 0;
    if(inCircle[root][0]>0)
    {
        root = inCircle[root][1]+n;
    }
    memset(dp,0,sizeof(dp));

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<pn;j++)
        {
            dp[i][j]=calProfit(i,p[j]);
        }
    }
    dfs_calculate(-1,root);

    ans = dp[0][0];
    cout<<"ANS "<<ans<<endl;
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
