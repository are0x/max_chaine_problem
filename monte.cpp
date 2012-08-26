#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cstring>
#include<set>
#include<cmath>
#include<fstream>
#include<cmath>

#define REP(i,n) for(int i = 0; i < (int)(n); i++)
#define FOR(it,c) for(__typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)
#define ALLOF(c) (c).begin(), (c).end()
using namespace std;
#define MAXR 13
#define MAXC 6
vector<string> MAP;
//"RB"¼´¤×¤è¤¬º¸¤Î
vector<string> TUMO;
int ChainBonus[19]={0,8,16,32,64,96,128,160,192,224,
		    256,288,320,352,384,416,448,480,512};
int ConnectBonus[11]={0,0,0,0,2,3,4,5,6,7,10};
int ColorBonus[4]={0,3,6,12};
int IntegralScore[19]={40,320,640,1280,2560,3840,5120,6400,7680,8960,10240,11520,
		       12800,14080,15360,16640,17920,19200,20480};
//¼´¤×¤è¤Ç¤Ê¤¤¤×¤è¤¬¾å0:º¸1:²¼2:±¦3
int PatternCol[22]={0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5};
int PatternDir[22]={0,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2};
bool visited[MAXR][MAXC];
int dr[4]={0,0,1,-1};
int dc[4]={1,-1,0,0};
ofstream ofs("monte.out");
inline bool validfield(int r,int c){
  return 0<=r && r<MAXR-1 && 0<=c && c<MAXC;
}

int CntConnect(int r,int c){
  int ret = 1;
  visited[r][c] = true;
  REP(i,4){
    int nr = r+dr[i];
    int nc = c+dc[i];
    if(validfield(nr,nc) && MAP[r][c] == MAP[nr][nc] && !visited[nr][nc]){
      ret+=CntConnect(nr,nc);
    }
  }
  return ret;
}

void DisplayMap(int tumon){
  if(tumon+2 < (int)TUMO.size())printf("%s %s\n",TUMO[tumon+1].c_str(),TUMO[tumon+2].c_str());
  else if(tumon+1 < (int)TUMO.size())printf("%s XX\n",TUMO[tumon+1].c_str());
  else printf("XX XX\n");
  REP(r,MAXR){
    cout<<MAP[MAXR-r-1]<<endl;
  }
  cout<<endl;
  if(tumon+2 < (int)TUMO.size())ofs<<TUMO[tumon+1]<<" "<<TUMO[tumon+2]<<endl;
  else if(tumon+1 < (int)TUMO.size())ofs<<TUMO[tumon+1]<<" XX"<<endl;
  else ofs<<"XX XX"<<endl;
  REP(r,MAXR){
    ofs<<MAP[MAXR-r-1]<<endl;
  }
  ofs<<endl;
}

inline void Drop(vector<string> &tmap){
  REP(r,MAXR+3){
    REP(c,MAXC){
      if(tmap[r][c]=='.'){
	int tr=r;
	for(int ir=r+1;ir<MAXR+3;ir++){
	  if(tmap[ir][c]!='.'){
	    tr = ir;
	    break;
	  }
	}
	swap(tmap[tr][c],tmap[r][c]);
      }
    }
  }
}

int GetScore(bool displayVal,int tumon){
  int chain = 1;
  int ret = 0;
  while(1){
    bool vanish = false;
    int connectbonus=0;
    int sumconnect = 0;
    set<char> color;
    REP(r,MAXR-1){
      REP(c,MAXC){
	if(MAP[r][c]=='.') continue;
	memset(visited,false,sizeof(visited));
	int t=CntConnect(r,c);
	if(t>=4){
	  color.insert(MAP[r][c]);
	  REP(tr,MAXR){
	    REP(tc,MAXC){
	      if(visited[tr][tc]) MAP[tr][tc]='.';
	    }
	  }
	  sumconnect += t;
	  if(t>=11)t=11;
	  connectbonus+=ConnectBonus[t-1];
	  vanish=true;
	}
      }
    }
    if(!vanish) break;
    else{
      int t = 10*sumconnect*(connectbonus+ChainBonus[chain-1]+ColorBonus[color.size()-1]); 
      if(t==0) t=40;
      ret+=t;
      chain++;
      Drop(MAP);
      if(displayVal) DisplayMap(tumon);
    }
  }
  return ret;
}

pair<vector<int>,int> CreateMoves(vector<string> &initmap,vector<string> &tumo){
  vector<int> ret;
  int score = 0;
  MAP = initmap;
  REP(i,tumo.size()){
    int n = rand()%22;
    int cc=PatternCol[n];
    int cr=MAXR+1;
    ret.push_back(n);
    MAP[cr][cc]=tumo[i][0];
    MAP[cr+dr[PatternDir[n]]][cc+dc[PatternDir[n]]] = tumo[i][1];
    Drop(MAP);
    score+=GetScore(false,i);
  }
  return pair<vector<int>,int>(ret,score);
}

pair<vector<int>,int> Neigh(vector<int> &move,vector<string> &initmap,vector<string> &tumo){
  int idx = rand()%move.size();
  vector<int> ret = move;
  ret[idx] = rand()%22;
  MAP = initmap;
  int score = 0;
  REP(i,tumo.size()){
    int cc=PatternCol[ret[i]];
    int cr=MAXR+1;
    MAP[cr][cc]=tumo[i][0];
    MAP[cr+dr[PatternDir[ret[i]]]][cc+dc[PatternDir[ret[i]]]] = tumo[i][1];
    Drop(MAP);
    score+=GetScore(false,i);
  }
  return pair<vector<int>,int>(ret,score);
}
pair<int,vector<int> > SimulatedAnnealing(vector<string> &initmap,vector<string> &tumo,int m){
  double T=1;
  const double EPS = 1e-8;
  pair<vector<int>,int> p = CreateMoves(initmap,tumo);
  vector<int> curmove = p.first;
  int curscore = p.second;
  REP(i,m){
    if(T<EPS) break;
    p=Neigh(curmove,initmap,tumo);
    if(p.second>curscore){
      curmove = p.first;
      curscore = p.second;
    }
    else{
      double r = 1.0*rand()/RAND_MAX;
      if(r<exp((p.second-curscore)/T)){
	curmove = p.first;
	curscore = p.second;
      }
    }
    T*=0.99;
  }
  return pair<int,vector<int> >(curscore,curmove);
}
int Simulated(vector<int> &move,vector<string> &initmap,vector<string> &tumo,bool displayflag){
  int dr[4]={1,0,-1,0};
  int dc[4]={0,-1,0,1};
  MAP = initmap;
  int score = 0;
  REP(i,tumo.size()){
    int cc=PatternCol[move[i]];
    int cr=MAXR+1;
    MAP[cr][cc]=tumo[i][0];
    MAP[cr+dr[PatternDir[move[i]]]][cc+dc[PatternDir[move[i]]]] = tumo[i][1];
    Drop(MAP);
    if(displayflag) DisplayMap(i);
    score+=GetScore(displayflag,i);
  }
  if(displayflag)DisplayMap(tumo.size());
  if(displayflag)cout<<"score:"<<score<<endl;
  if(displayflag)ofs<<"score:"<<score<<endl;
  return score;
}

vector<string> CreateRandTumo(int n){
  vector<string> ret;
  string pattern[16]={"RR","RG","RY","RB",
		    "GR","GG","GY","GB",
		    "BR","BG","BY","BB",
		    "YR","YG","YY","YB"
  };
  REP(i,n) ret.push_back(pattern[rand()%16]);
  return ret;
}

vector<int> CreateRandMove(int n){
  vector<int> ret;
  REP(i,n) ret.push_back(rand()%22);
  return ret;
}

int MCmethod(vector<string> &initmap,int m,int n,string cur,string next,double alpha){
  vector<string> nextmaps[22];
  int cnts[22];
  double expscore[22];
  REP(i,22){
    nextmaps[i] = initmap;
    int cc=PatternCol[i];
    int cr=MAXR+1;
    nextmaps[i][cr][cc]=cur[0];
    nextmaps[i][cr+dr[PatternDir[i]]][cc+dc[PatternDir[i]]] = cur[1];
    Drop(nextmaps[i]);
  }
  memset(cnts,0,sizeof(cnts));
  memset(expscore,0,sizeof(expscore));
  REP(i,m){
    int maxj = 0;
    double maxval=0;
    REP(j,22){
      if(cnts[j]>0){
	double ucb = expscore[j]+alpha*sqrt(log(i)/cnts[j]);
	//printf("%.4f ",ucb);
	if(maxval<ucb){
	  maxval = ucb;
	  maxj = j;
	}
      }
      else{
	maxj = j;
	break;
      }
    }
    //cout<<endl;
    vector<int> moves = CreateRandMove(n-1);
    vector<string> tumo = CreateRandTumo(n-1);
    tumo[0] = next;
    int score = SimulatedAnnealing(nextmaps[maxj],tumo,1000).first;
    if(cnts[maxj]==0) expscore[maxj] = score;
    else{
      expscore[maxj] = expscore[maxj]/(cnts[maxj]+1)*cnts[maxj]+1.0*score/(cnts[maxj]+1);
    }
    cnts[maxj]++;
  }
  int ret = 0;
  double maxval = 0;
  REP(i,22){
    if(cnts[i]>0){
      double ucb = expscore[i]+alpha*sqrt(log(m)/cnts[i]);
      cout<<ucb<<" ";
      if(maxval<ucb){
	maxval = ucb;
	ret = i;
      }
    }
    else{
      ret = i;
      break;
    }
  }
  cout<<endl;
  cout<<"selected:"<<ret<<endl;
  return ret;
}
int main()
{
  int n;
  srand(time(NULL));
  MAP.assign(MAXR+3,"......");
  vector<string> initmap(MAXR+3,"......");
  REP(r,MAXR){
    string t;
    cin>>t;
    initmap[MAXR-r-1]=t;
  }
  cin>>n;
  REP(i,n){
    string t;
    cin>>t;
    TUMO.push_back(t);
  }
  vector<string> tmpmap = initmap;
  vector<int> bestmove;
  REP(i,n-1){
    int move =  MCmethod(tmpmap,100,n-i,TUMO[i],TUMO[i+1],1.3);
    bestmove.push_back(move);
    int cc=PatternCol[i];
    int cr=MAXR+1;
    tmpmap[cr][cc]=TUMO[i][0];
    tmpmap[cr+dr[PatternDir[i]]][cc+dc[PatternDir[i]]] = TUMO[i][1];
    Drop(tmpmap);
  }
  int maxval = 0;
  int maxi = 0;
  REP(i,22){
    vector<int> move(1,i);
    vector<string> tumo(1,TUMO[n-1]);
    int score = Simulated(move,tmpmap,tumo,false);
    if(score < maxval){
      maxi = i;
      maxval = score;
    }
  }
  bestmove.push_back(maxi);
  Simulated(bestmove,initmap,TUMO,true);
  return 0;
}
