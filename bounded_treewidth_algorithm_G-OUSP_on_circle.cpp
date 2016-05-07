#include <iostream>
#include <stdio.h>
#include <time.h>
#include <set>
#include <string.h>
#include <vector>
#include <map>
#include <assert.h>
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

struct bag
{
    int dep[4];
    int pri[4];
    int profit;

    bool valid(int a,int b)
    {
        return dep[a]+1==dep[b]&&pri[a]<=pri[b];
    }
    bool valid(int i)
    {
        return valid(i-1,i)||valid(i+1,i);
    }
    bool valid()
    {
        return valid(1)&&valid(2);
    }
    void right()
    {
        for(int i=2;i>=0;i--)
        {
            dep[i+1]=dep[i];
            pri[i+1]=pri[i];
        }
        dep[0]=pri[0]=0;
    }



    void asst()
    {
        bool flag1 = (dep[1]==dep[0]+1);
        bool flag2 = (dep[2]==dep[3]+1);
        bool flag3 = (dep[1]==dep[2]+1);
        bool flag4 = (dep[2]==dep[1]+1);

        bool flag5 = (pri[1]>=pri[0]);
        bool flag6 = (pri[1]>=pri[2]);
        bool flag7 = (pri[2]>=pri[1]);
        bool flag8 = (pri[2]>=pri[3]);
        assert(flag1||flag2||flag3||flag4||flag5||flag6||flag7||flag8);
    }
}tmp;
bool operator<(const bag &a, const bag &b)
{
    for(int i=0;i<3;i++)
    {
        if(a.dep[i]<b.dep[i])return true;
        else if(a.dep[i]>b.dep[i])return false;
        else if(a.pri[i]<b.pri[i])return true;
        else if(a.pri[i]>b.pri[i])return false;
    }
    return false;
}
map<bag,int> bags[N];
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
void gao(int index,int profit)
{
    if(!tmp.valid())
        return;

    tmp.asst();



    if(index == n-2)
    {
        profit = tmp.pri[1]*countRequest(index,tmp.pri[1])+tmp.pri[2]*countRequest(index+1,tmp.pri[2]);

    }
    else
    {
        profit = profit+tmp.pri[1]*countRequest(index,tmp.pri[1]);

    }
    int t = bags[index][tmp];
    bags[index][tmp] = max(profit,t );

}
void dfs(int index, int x,int xn, int profit)
{
    if(x==xn)
    {
        gao(index,profit);
        return;
    }
    for(int i=1;i<=n-1;i++)
    {
        tmp.dep[x]=i;
        for(int j=0;j<pn;j++)
        {
            tmp.pri[x]=p[j];
            dfs(index,x+1,xn,profit);
        }
    }
}
template<class T>
void STLClearObject(T* obj)
{
    T tmp;
    tmp.swap(*obj);
}
void createBags(int index)
{
    if(index == n-2)
    {
        tmp.dep[3]=0;
        tmp.pri[3]=0;
        dfs(index,0,3,0);
    }
    else
    {
        for(map<bag,int>::iterator mit = bags[index+1].begin();mit!=bags[index+1].end();mit++)
        {
            tmp = mit->first;
            int profit = mit->second;
            tmp.right();
            if(index>1)
                dfs(index,0,1,profit);
            else if(index==1)
            {
                tmp.dep[0]=tmp.pri[0]=0;
                gao(1,profit);
            }
        }
    }
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
    for(int i=n-2;i>=1;i--)
    {
        bags[i].clear();
        createBags(i);
    }

    for(map<bag,int>::iterator mit = bags[1].begin();mit!=bags[1].end();mit++)
    {
        ans=max(ans,mit->second);
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
