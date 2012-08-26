#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
#define REP(i,n) for(int i = 0; i < (int)(n); i++)
#define FOR(it,c) for(__typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)
#define ALLOF(c) (c).begin(), (c).end()
using namespace std;
string pattern[16]={"RR","RG","RY","RB",
		    "GR","GG","GY","GB",
		    "BR","BG","BY","BB",
		    "YR","YG","YY","YB"
};
string twocolor="RRRRRRRRRRRRGGGGGGGG";
string threecolor="RRRRRRRBBBBBBBGGGGGR";
int main()
{
  int N=20;
  FILE *fp = fopen("maketumo.ini","r");
  char buf[256];
  REP(r,13) {
    fgets(buf,256,fp);
    cout<<buf;
  }
  fgets(buf,256,fp);
  cout<<buf<<endl;
  N = atoi(buf);
  srand(time(NULL));
  REP(i,N){
    cout<<pattern[rand()%16]<<endl;
  }
  return 0;
}
