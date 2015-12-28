#include "csoko_thinker/csoko_solver.h"


namespace csoko_thinker{

using namespace std;


/***************************************
 *
 *Functions
 *
 ****************************************/

short heur_fun(std::vector<short> &sit) {
	short fx=0;
	for(int i=0;i<Nbox;i++)
		fx+= ( abs( dirxy[sit[i]][0]-dirxy[box_dest[i]][0] )+
				abs( dirxy[sit[i]][1]-dirxy[box_dest[i]][1] ));
	return fx;
};

short SokoCanPush( short dx, short dy, short sx, short sy) {
	std::queue<short> cola; memset(amp,-1,sizeof(amp));
	cola.push(sx); cola.push(sy); cola.push(0);
	while( !cola.empty() ) {
		short x= cola.front(); cola.pop();
		short y= cola.front(); cola.pop();
		short cost= cola.front(); cola.pop();
		if(x==dx && y==dy) { amp[x][y]=cost; return cost; }
		if( amp[x][y]!=-1 ) continue;
		amp[x][y]=cost;
		for(int i=0;i<4;i++)
			if( mapa[x+moves[i][0]][y+moves[i][1]]==' ' ) {
				cola.push(x+moves[i][0]); cola.push(y+moves[i][1]);
				cola.push(cost+1);
			}
	};
	return -1;
};

void backtrack_sol( int fin ) {
	std::stack<int> sol,minp;
	while( path[fin].back!=-1 ) {
		int finback= path[fin].back;
		int d= path[fin].dir;
		int od= path[finback].dir;
		int x= dirxy[path[fin].xy][0]+2*moves[d][0];
		int y= dirxy[path[fin].xy][1]+2*moves[d][1];
		int bx= dirxy[path[finback].xy][0]+moves[od][0];
		int by= dirxy[path[finback].xy][1]+moves[od][1];

		int mox= dirxy[path[fin].xy][0];
		int moy= dirxy[path[fin].xy][1];
		int mx= dirxy[path[fin].xy][0]+moves[d][0];
		int my= dirxy[path[fin].xy][1]+moves[d][1];
		int mbx= dirxy[path[fin].xy][0]+2*moves[d][0];
		int mby= dirxy[path[fin].xy][1]+2*moves[d][1];
		mapa[mox][moy]=' '; mapa[mx][my]=' ';
		mapa[mx][my]='$'; mapa[mbx][mby]='@';
		mapa[mbx][mby]=' ';
		SokoCanPush(bx,by,x,y);
		int ix=bx,iy=by,nval=amp[bx][by];
		while( nval>0 ) {
			for(int i=0;i<4;i++)
				if( amp[ix+moves[i][0]][iy+moves[i][1]]==nval-1 ) {
					ix+=moves[i][0]; iy+=moves[i][1];
					minp.push(i);
					break;
				}
			nval--;
		};
		minp.push((d+2)%4);
		while(!minp.empty()) { sol.push(minp.top()); minp.pop(); }
		fin= path[fin].back;
	};

	int d= path[fin].dir;
	int x= dirxy[path[fin].xy][0]+2*moves[d][0];
	int y= dirxy[path[fin].xy][1]+2*moves[d][1];
	int bx= dirxy[sokop][0];
	int by= dirxy[sokop][1];

	int mox= dirxy[path[fin].xy][0];
	int moy= dirxy[path[fin].xy][1];
	int mx= dirxy[path[fin].xy][0]+moves[d][0];
	int my= dirxy[path[fin].xy][1]+moves[d][1];
	int mbx= dirxy[path[fin].xy][0]+2*moves[d][0];
	int mby= dirxy[path[fin].xy][1]+2*moves[d][1];
	mapa[mox][moy]=' '; mapa[mx][my]=' ';
	mapa[mx][my]='$'; mapa[mbx][mby]='@';
	mapa[mbx][mby]=' ';
	SokoCanPush(bx,by,x,y);
	int ix=bx,iy=by,nval=amp[bx][by];
	while( nval>0 ) {
		for(int i=0;i<4;i++)
			if( amp[ix+moves[i][0]][iy+moves[i][1]]==nval-1 ) {
				ix+=moves[i][0]; iy+=moves[i][1];
				minp.push(i);
				break;
			}
		nval--;
	};
	minp.push((d+2)%4);
	while(!minp.empty()) { sol.push(minp.top()); minp.pop(); }
	fin= path[fin].back;

	std::ofstream filesol("solution.txt");
	while(!sol.empty()) { filesol<<sol.top()<<endl; sol.pop(); }
	filesol.close();
};

bool deathlock() {
	for(int i=1;i<pcont;i++) {
		int x= dirxy[i][0];
		int y= dirxy[i][1];
		if( bmapa[x][y]!='.'&& mapa[x][y]!='@' ) {
			if( ( (mapa[x][y]!=' '&& mapa[x][y]!='@') &&
					(mapa[x+1][y]!=' '&& mapa[x+1][y]!='@') ) &&
					( (mapa[x][y+1]!=' '&& mapa[x][y+1]!='@') &&
							(mapa[x+1][y+1]!=' '&& mapa[x+1][y+1]!='@') ) ) {
				return 1; }
			if( ( (mapa[x][y]!=' '&& mapa[x][y]!='@') &&
					(mapa[x+1][y]!=' '&& mapa[x+1][y]!='@') ) &&
					( (mapa[x][y-1]!=' '&& mapa[x][y-1]!='@') &&
							(mapa[x+1][y-1]!=' '&& mapa[x+1][y-1]!='@') ) ) {
				return 1; }
			if( ( (mapa[x][y]!=' '&& mapa[x][y]!='@') &&
					(mapa[x-1][y]!=' '&& mapa[x-1][y]!='@') ) &&
					( (mapa[x][y+1]!=' '&& mapa[x][y+1]!='@') &&
							(mapa[x-1][y+1]!=' '&& mapa[x-1][y+1]!='@') ) ) {
				return 1; }
			if( ( (mapa[x][y]!=' '&& mapa[x][y]!='@') &&
					(mapa[x-1][y]!=' '&& mapa[x-1][y]!='@') ) &&
					( (mapa[x][y-1]!=' '&& mapa[x][y-1]!='@') &&
							(mapa[x-1][y-1]!=' '&& mapa[x-1][y-1]!='@') ) ) {
				return 1; }
			/*if(x-2>=0 && y-2>=0) {
       if( ( ( (mapa[x-1][y]!=' '&& mapa[x-1][y]!='@') &&
               (mapa[x-1][y]!=' '&& mapa[x-1][y]!='@') ) &&
             ( (mapa[x][y-1]!=' '&& mapa[x][y-1]!='@') &&
               (mapa[x-2][y]!=' '&& mapa[x-2][y]!='@') ) ) &&
           ( ( (mapa[x][y-2]!=' '&& mapa[x][y-2]!='@') &&
               (mapa[x-2][y-1]!=' '&& mapa[x-2][y-1]!='@') ) &&
             ( (mapa[x-1][y-2]!=' '&& mapa[x-1][y-2]!='@') &&
               (mapa[x-2][y-2]!=' '&& mapa[x-2][y-2]!='@') ) ) ) {
            return 1; }
       }*/
		}
	}
	return 0;
};

void Astar() {
	int nstate=0,spec=0;
	boxes.push_back(sokop);
	std::priority_queue< node > OpenList;
	OpenList.push( node(0,heur_fun(boxes),-1,0,boxes) );
	ClosedList[boxes]=1;
	while( !OpenList.empty() ) {
		node CN = OpenList.top(); OpenList.pop();

		for(int i=0;i<Nbox;i++)
			mapa[ dirxy[ CN.config[i] ][0] ][ dirxy[ CN.config[i] ][1] ]='$';
		mapa[ dirxy[ CN.config[Nbox] ][0] ][ dirxy[ CN.config[Nbox] ][1] ]='@';

		if(CN.placedbox==Nbox) { printf("solution\n");
		printf("No estados: %d %d  apilados: %d \n",path.size(),nstate,OpenList.size());
		for(int i=0;i<maxx;i++) puts(mapa[i]);
		while( !OpenList.empty() ) OpenList.pop();
		backtrack_sol( CN.npath );
		break; }

		spec++;
		if(spec==20000) {
			printf("No estados: %d %d  apilados: %d \n",path.size(),nstate,OpenList.size());
			for(int i=0;i<maxx;i++) puts(mapa[i]); spec=0;
			//getchar();
		}

		for(int i=0;i<Nbox;i++) {
			int x=dirxy[ CN.config[i] ][0], y= dirxy[ CN.config[i] ][1];
			for(int j=0;j<4;j++)
				if( (mapa[ x+ moves[j][0] ][ y+ moves[j][1] ]==' ' ||
						mapa[ x+ moves[j][0] ][ y+ moves[j][1] ]=='@')&&
						bmapa[ x+ moves[j][0] ][ y+ moves[j][1] ]!='?' ) {
					int findway = SokoCanPush( x+moves[(j+2)%4][0],y+moves[(j+2)%4][1],
							dirxy[CN.config[Nbox]][0],dirxy[CN.config[Nbox]][1] );
					if( findway==-1 ) continue;

					std::vector<short> tcon= CN.config;
					ite= lower_bound(tcon.begin(),--tcon.end(),
							nummapa[ x ][ y ],point_sort());
					tcon.erase(ite);
					ite= lower_bound(tcon.begin(),--tcon.end(),
							nummapa[ x+moves[j][0] ][ y+moves[j][1] ],point_sort());
					tcon.insert(ite,nummapa[ x+moves[j][0] ][ y+moves[j][1] ]);
					tcon[Nbox]=nummapa[ x ][ y ];

					if(ClosedList[tcon]) continue;
					ClosedList[tcon]=1;

					for(int i=0;i<Nbox;i++) // Borrar
						mapa[ dirxy[ CN.config[i] ][0] ][ dirxy[ CN.config[i] ][1] ]=' ';
					mapa[ dirxy[ CN.config[Nbox] ][0] ][ dirxy[ CN.config[Nbox] ][1] ]=' ';
					for(int i=0;i<Nbox;i++) // Grabar tcon
						mapa[ dirxy[ tcon[i] ][0] ][ dirxy[ tcon[i] ][1] ]='$';
					mapa[ dirxy[ tcon[Nbox] ][0] ][ dirxy[ tcon[Nbox] ][1] ]='@';
					bool death= deathlock();

					for(int i=0;i<Nbox;i++) // Borrar tcon
						mapa[ dirxy[ tcon[i] ][0] ][ dirxy[ tcon[i] ][1] ]=' ';
					mapa[ dirxy[ tcon[Nbox] ][0] ][ dirxy[ tcon[Nbox] ][1] ]=' ';
					for(int i=0;i<Nbox;i++) // Restaurar
						mapa[ dirxy[ CN.config[i] ][0] ][ dirxy[ CN.config[i] ][1] ]='$';
					mapa[ dirxy[ CN.config[Nbox] ][0] ][ dirxy[ CN.config[Nbox] ][1] ]='@';
					if(death) continue;

					int cpb=0;
					for(int i=0;i<Nbox;i++)
						if( bmapa[ dirxy[tcon[i]][0] ][ dirxy[tcon[i]][1] ]=='.') cpb++;
					OpenList.push( node(CN.cost+1+findway,heur_fun(tcon),nstate,cpb,tcon) );
					path.push_back( path_node( nummapa[ x+moves[j][0] ][ y+moves[j][1] ],
							(j+2)%4,CN.npath ) );
					nstate++;
				}
		}

		for(int i=0;i<Nbox;i++)
			mapa[ dirxy[ CN.config[i] ][0] ][ dirxy[ CN.config[i] ][1] ]=' ';
		mapa[ dirxy[ CN.config[Nbox] ][0] ][ dirxy[ CN.config[Nbox] ][1] ]=' ';
	};
};

bool BlockedPath(short fx, short fy,short fwall, short fdir, short ite) {
	//printf("ite: %d  x: %d  y: %d\n",ite,fx,fy);
	if( mapa[fx][fy]=='#' ) return 1;
	if( mapa[fx][fy]!=' ' || bmapa[fx][fy]=='.' ) return 0;
	if( mapa[fx+moves[fwall][0]][fy+moves[fwall][1]]!='#' ) return 0;
	if( BlockedPath( fx+moves[fdir][0],fy+moves[fdir][1],fwall,fdir,ite+1 ) ) {
		bmapa[fx][fy]='?'; return 1;
	}
	return 0;
};




/***************************************
 *
 *main
 *
 ****************************************/
int solver(string pathToFile) {
	in = ifstream(pathToFile.c_str());

	clock_t start, end; start= clock();
	std::string line;
	memset(nummapa,-1,sizeof(nummapa));
	while( getline(in,line) ) {
		strcpy(mapa[maxx],line.c_str()); maxx++;
	} maxy= line.size();

	for(int i=0;i<maxx;i++)
		for(int j=0;j<maxy;j++)
			if(mapa[i][j]!='#' && mapa[i][j]!=',') {
				if(mapa[i][j]=='$' || mapa[i][j]=='*') Nbox++;
				nummapa[i][j]=pcont;
				dirxy[pcont][0]=i; dirxy[pcont][1]=j;
				pcont++;
			}

	boxes.reserve( Nbox+2 ); box_dest.reserve( Nbox+2 );
	for(int i=0;i<maxx;i++)
		for(int j=0;j<maxy;j++) {
			if(mapa[i][j]=='$' || mapa[i][j]=='*') {
				ite= lower_bound(boxes.begin(),boxes.end(),nummapa[i][j],point_sort());
				boxes.insert(ite,nummapa[i][j]);
				if(mapa[i][j]=='*') mapa[i][j]='.';
				else mapa[i][j]=' ';
			}
			if(mapa[i][j]=='@' || mapa[i][j]=='+' ) {
				sokop= nummapa[i][j];
				if(mapa[i][j]=='+') mapa[i][j]='.';
				else mapa[i][j]=' ';
			}
			if(mapa[i][j]=='.') {
				ite= lower_bound(box_dest.begin(),box_dest.end(),nummapa[i][j],point_sort());
				box_dest.insert(ite,nummapa[i][j]); mapa[i][j]=' '; bmapa[i][j]='.'; }
		}

	for(int i=0;i<maxx;i++)
		for(int j=0;j<maxy;j++)
			for(int k=0;k<4;k++)
				if( (mapa[i][j]!='#' && mapa[i][j]!=',') && bmapa[i][j]!='.' )
					if(mapa[i+moves[k][0]][j+moves[k][1]]=='#' &&
							mapa[i+moves[(k+1)%4][0]][j+moves[(k+1)%4][1]]=='#') {
						BlockedPath(i,j,k,(k+3)%4,0);
						BlockedPath(i,j,(k+1)%4,(k+2)%4,0);
						bmapa[i][j]='?';
					}

	for(int i=0;i<maxx;i++) {
		printf("\n");
		for(int j=0;j<maxy;j++) printf("%c", ((bmapa[i][j]=='?')? '?': mapa[i][j]) );
	}
	/*for(int i=0;i<maxx;i++) {
  puts(mapa[i]);
  } getchar(); getchar();*/
	Astar(); end=clock();
	double dif= end-start;
	printf("\t EXECUTION TIME: %.5lf seg",(double)( dif/1000.0 ));
	getchar();
}



}
