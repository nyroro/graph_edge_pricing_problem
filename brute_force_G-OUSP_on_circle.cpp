#include <iostream>
#include <stdio.h>
#include <time.h>
using namespace std;
const int N = 100;
const int M = 100;
int n,m;
int arr[N];
int sum[N];
int ans;
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

int calProfit(int total)
{
    int ret = 0;
    for(int i=0;i<m;i++)
    {
        int cost;
        cost = min( sum[req[i].from-1], total - sum[req[i].from-1]);
        if(cost<=req[i].budget)
        {
            ret+=cost;
        }

    }
    return ret;
}

void dfs(int maxBudget, int total, int depth)
{
    if(depth == n)
    {
        int t = calProfit(total);
        ans = max(ans,t);
        return;
    }

    for(int i=0;i<=maxBudget;i++)
    {
        arr[depth] = i;
        if(depth == 0)
        {
            sum[0] = arr[0];
        }
        else
        {
            sum[depth] = arr[depth]+sum[depth-1];
        }
        dfs(maxBudget, total+i, depth+1);
    }
}
void solve()
{
    int maxBudget = 0;
    for(int i=0;i<m;i++)
    {
        maxBudget = max(req[i].budget, maxBudget);
    }
    ans = 0;

    dfs(maxBudget,0,0);
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
