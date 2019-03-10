#include<iostream>
#include<ctime>
using namespace std;

#define mod 1000000007
#define lld long long
#define mp make_pair
#define pb push_back
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
#define ff first
#define ss second
lld power(lld x,lld y,lld MOD=0)	{if(MOD==0) MOD=mod; lld ans=1LL; while(y){ if(y&1) ans=(ans*x)%MOD; x=(x*x)%MOD; y>>=1LL;}return ans;}
lld iMOD(lld x, lld MOD=0)	{if(MOD==0) MOD=mod;return power(x,MOD-2,MOD);}

int main(){
	srand(time(NULL));
	string temp;
	string a="a";
	string op[]={"+","-","*","/"};
	cout<<"a+= a"<<' ';
	for(int i=1;i<300;i++){
		cout<<op[rand()%4]<<' '<<a<<' ';
	}
	cout<<";";
}

