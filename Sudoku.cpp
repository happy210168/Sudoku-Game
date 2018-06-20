#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<ctime>
using namespace std;
//variable
vector<vector<int> > S,tmpS;
vector<vector<int> > Result;

vector<vector<vector<int> > > pb;	//store the number which can be input  //find candidate
vector<vector<int> > space,Space;//the position can be input //getInitial holeNum 
int pbNum[12][12]={0}; //store the number of numbers which can be input  //depending traversal order (row_list,col_list)
int holeNum = 0;  //the number of holes
int resultNum = 0; //the number of results

//vector<int> candidate // store the number which not exist in this row & column & block
vector<int> row_list; //traversal order
vector<int> col_list; //traversal order
FILE *ifp;
fstream ofp1,ofp2;  //ifp(input) ofp1(Sudoku randomly create) ofp2(Sudoku Answer)

//function prototype
void find_candidate(int row,int col,vector<int>& v,vector< vector<vector<int> > >& PB); //use pb to find candidate
int inVec(int num,vector<int> v); //check num in vector 

void getholeNum();
void getpb();
void getpbNum();
void getlist(); //get the traveral list in row_list and col_list

//clear the candidate number
void clearRow(int value,int row,vector< vector<vector<int> > >& PB);
void clearCol(int value,int col,vector< vector<vector<int> > >& PB);
void clearBlock(int value,int row,int col,vector< vector<vector<int> > >& PB);

//Recover the candidate number
void recoveryRow(int value,int row,vector< vector<vector<int> > >& PB);
void recoveryCol(int value,int col,vector< vector<vector<int> > >& PB);
void recoveryBlock(int value,int row,int col,vector< vector<vector<int> > >& PB);

int checkCol(vector<vector<int> >& v);
int checkRow(vector<vector<int> >& v); 
int checkBlock(vector<vector<int> >& v);
inline int checkSudoku(vector<vector<int> >& v);  //checkSudoku is correct or not 

void print_question(); 
void solve(int cur,vector<vector<int> > v,int hole,vector< vector<vector<int> > > PB);
void getResult();
void print_result();

void mirror(vector<vector<int> >& vec);
void swap3(vector<vector<int> > &v);
void swap1(vector<vector<int> > &v);
void turnCol(vector<vector<int> >&v,int a,int b);
void turnRow(vector<vector<int> >&v,int a,int b);
void changeNum();
void clockwise(vector<vector<int> >&v);
void dighole(vector<vector<int> >&v);

int main(){
	
	int valid = 0;
	srand((unsigned)time(NULL));	
	S.resize(12);
 	for (int k = 0; k < 12; k++){
  	S[k].resize(12);
  }

	space.resize(12);
	for(int k=0; k < 12;k++){
		space[k].resize(12);
	}

	pb.resize(12);
	for (int i=0; i<12;i++){
		pb[i].resize(12);
		for(int j=0;j<12;j++){
			pb[i][j].resize(9);
		}
	}

	char choose;
	while(1){
		cout << "a:Create Sudoku board randomly\nb:Input from file\n";
		cin >> choose;
		if(choose=='a' || choose =='b') break;
	}


	ifp = fopen("input.txt","r");
	ofp1.open("problem.txt",ios::out);
	ofp2.open("answer.txt",ios::out);
	//read file
	if(!ifp || !ofp1 || !ofp2) cerr << "Failed to open" << endl;
	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			fscanf(ifp,"%d",&S[i][j]);
			getc(ifp);
		}
	}

	//Input from file
	if(choose == 'b'){
		if(checkSudoku(S)){
		 	cout << "Valid problem!\n";
 			valid = 1;	
			print_question();
		}
		else{ 
			cout << "Invalid problem!\n";
			valid = 0;
			cout << "0" << endl;
			return 0;
		}
	}


	//Create Sudoku board randomly
	if(choose == 'a'){
		string hole;
		int dignum;
		cout << "Please input the number of holes(40~50 or random(r)): "; 
		cin >> hole;
		if(hole == "r") dignum = rand()%11 + 10;
		else dignum = atoi(hole.c_str()) - 30;

		tmpS = S;
		for(int i=0;i<rand()%3+1;i++) mirror(S);
		getResult(); if(resultNum!=1) S = tmpS; else tmpS = S;
		for(int i=0;i<rand()%3+1;i++) swap3(S);
		getResult(); if(resultNum!=1) S = tmpS; else tmpS = S;
		for(int i=0;i<rand()%3+1;i++)	swap1(S);
		getResult(); if(resultNum!=1) S = tmpS; else tmpS = S;
		for(int i=0;i<rand()%3+1;i++)	clockwise(S);
		getResult(); if(resultNum!=1) S = tmpS; 
		changeNum();
		
		for(int i=0;i<dignum;i++){ 
			dighole(S);
		} //dig holes

		if(checkSudoku(S)){
		 	cout << "Valid problem\n";
 			valid = 1;	
			print_question();	
		}
		else{ 
			cout << "Invalid problem\n";
			cout << "0" << endl;
			valid = 0;
			return 0;
		}
	} //Create Sudoku board randomly
	
		
	while(1){
		cout << "Play(y) / Show answer(n) \n";
		cin >> choose;
		if(choose=='y' || choose =='n') break;
	} //Play or not play and show answer
	

	//Solve it
	if(valid) getResult(); 
	
	Space = space; 

	//Interactive
	if(choose == 'y'){
		cout << "Go!" << endl;
		char ch;
		int row=-1;
		int col=-1;
		int num=0;
		while(1){
			scanf("%c",&ch);
			if(ch == 'i')	scanf("%d %d %d",&row,&col,&num);
			else if(ch == 'd') scanf("%d %d",&row,&col);
			row--; col--;
			switch(ch){
				
				case 's': //Unsolvable / Exactly one Solution / More than one solution
					switch(resultNum){
					case 0:
						cout << "0" << endl; //0 answer
						break;
					case 1:
						cout << "1" << endl; 
						print_result();  //if only one answer, show result.
						break;
					default:
						cout << "2" << endl; //morn than one answer
					}
					return 0; //Show Result and Exit game

				case 'e': //Exit
					return 0; 

				case 'd': //Delete
					if(Space[row][col]==0 || S[row][col]==0){ cout << "The grid can't be deleted.\n"; break;}
					else S[row][col] = 0;
					print_question();
					break;		
 
				case 'i': //Input
					if(Space[row][col]==0 || S[row][col]!=0){ cout << "The grid can't be input.\n"; break;}
			    S[row][col] = num;
					if(!checkSudoku(S)){ cout << "Wrong input!(Duplicate number)\n"; S[row][col] = 0; break;} //duplicated number in block or row or column
					print_question(); 
					break; 
			}
			getholeNum(); 
			if(holeNum==0 && checkSudoku(S)){ cout << "Complete Sudoku!" << endl; break;} //check if complete sudoku or not
		}
	} //Interactive


	if(choose == 'n'){ 
		switch(resultNum){		
			case 0:
				cout << "0" << endl; //0 answer
				break;
			case 1:
				cout << "1" << endl; 
				print_result();  //if only one answer, show result.
				break;
			default:
				cout << "2" << endl; //morn than one answer
		}
	}


	return 0;
}

void getResult(){
	resultNum = 0;
	getholeNum();
	getpb();
	getpbNum();
	getlist();
	solve(0,S,holeNum,pb); 
}

void solve(int cur,vector<vector<int> > v,int hole,vector<vector<vector<int> > > PB){

	if(resultNum>=2) return;

	if(cur == hole){
		if(checkSudoku(v)){	
			resultNum++;
			Result = v;
		}
		return;
	}		
	
	int row = row_list[cur];
	int col = col_list[cur];
	vector<int> candidate;
	find_candidate(row,col,candidate,PB);
	
	if(candidate.size() == 0)	return;	
	

	if(candidate.size() == 1){
		int value = candidate[0];
		v[row][col] = value;
		clearRow(value,row,PB); clearCol(value,col,PB); clearBlock(value,row,col,PB);
	}
	for(int a=0;a<candidate.size();a++){
		int value = candidate[a];
		clearRow(value,row,PB); clearCol(value,col,PB); clearBlock(value,row,col,PB);
		v[row][col] = value;
		solve(cur+1,v,hole,PB);
		v[row][col] = 0;
		recoveryRow(value,row,PB); recoveryCol(value,col,PB); recoveryBlock(value,row,col,PB);
	}
}

void find_candidate(int row,int col,vector<int>& v,vector<vector<vector<int> > >& PB){
	vector<int> tmp;
	vector<int> r,c,b;
	for(int a=1;a<=9;a++){
		if(PB[row][col][a-1] == a) 			
			tmp.push_back(a);
	}
	
	for(int i=0;i<12;i++){
		for(int j=0;j<9;j++){
			if(i==col) continue;
			int n = PB[row][i][j];
			if(n>0)	r.push_back(n);
		}
	}	

	for(int i=0;i<tmp.size();i++){
		int n = tmp[i];
		if(!inVec(n,r)){ 
			v.push_back(n);
			return;
		}
	}

	for(int i=0;i<12;i++){
		for(int j=0;j<9;j++){
			if(i==row) continue;
			int n = PB[i][col][j];
			if(n>0)	c.push_back(n);
		}
	}
	for(int i=0;i<tmp.size();i++){
		int n = tmp[i];
		if(!inVec(n,c)){ 
			v.push_back(n);
			return;
		}
	}

	int ROW = 3*(row/3);
	int COL = 3*(col/3);

	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(ROW+i==row && COL+j==col) continue;
			for(int k=0;k<9;k++){
				int n = PB[ROW+i][COL+j][k];
				if(n>0) b.push_back(n);
			}
		}
	}

	for(int i=0;i<tmp.size();i++){
		int n = tmp[i];
		if(!inVec(n,b)){ 
			v.push_back(n);
			return;
		}
	}
	
	for(int a=1;a<=9;a++){
		if(PB[row][col][a-1] == a) 			
			v.push_back(a);
	}
}

void print_question(){
		cout << "\\===========Sudoku Problem==========" << endl;
		cout << " \\c 1 2 3   4 5 6   7 8 9  10 11 12" << endl;
    cout << " r\\-------+-------+-------+-------+" << endl;
    for (int i = 0; i < 12; i++)
    {
        if(i<9)
					cout << " " << i+1 << "|";
				else
					cout << i+1 << "|";
        for (int j = 0; j < 12; j++)
        {
						if(S[i][j]==-1) cout << S[i][j]; 
						else cout << " " << S[i][j];
            if (j % 3 == 2)
                cout << " |";
        }
        cout << endl;
        if (i % 3 == 2){
            if(i!=11) cout << "--+-------+-------+-------+-------+" << endl;
						if(i==11) cout << "--+-------+-------+-------+-------+" << endl;
				}
				if(i == 11) 
					cout << "<Input: i [r] [c] [num] / Delete: d [r] [c] / Show answer(s) / Exit(e)> \n";
		}
		getholeNum(); cout << "left " << holeNum << " holes!\n";

		for(int i=0;i<12;i++){
			for(int j=0;j<12;j++){
				if(i==11 && j==11) ofp1 << S[i][j];
				else ofp1 << S[i][j] << ",";
			}
			if(i!=11)ofp1 << endl;
		}
}

void print_result(){
		cout << "\\===============Answer===============" << endl;
		cout << " \\c 1 2 3   4 5 6   7 8 9  10 11 12" << endl;
    cout << " r\\-------+-------+-------+-------+" << endl;
    for (int i = 0; i < 12; i++)
    {
        if(i<9)
					cout << " " << i+1 << "|";
				else
					cout << i+1 << "|";
        for (int j = 0; j < 12; j++)
        {
						if(Result[i][j]==-1) cout << Result[i][j];
						else cout << " " << Result[i][j];
            if (j % 3 == 2)
                cout << " |";
        }
        cout << endl;
        if (i % 3 == 2)
        	if(i!=11) cout << "--+-------+-------+-------+-------+" << endl;
				  else if(i==11) cout << "--+-------+-------+-------+-------+" << endl;
		}
		for(int i=0;i<12;i++){
			for(int j=0;j<12;j++){
				if(i==11 && j==11) ofp2 << Result[i][j];
				else ofp2 << Result[i][j] << ",";
			}
			if(i!=11) ofp2 << endl;
		}

}

void getholeNum(){
	holeNum =0;

	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++) space[i][j]=0;
	}

	for(int i=0;i<12;i++)
		for(int j=0;j<12;j++)
			if(S[i][j]==0){ holeNum++; space[i][j]=1; };
}

void clearRow(int value,int row,vector< vector<vector<int> > >& PB){
	for(int a=0;a<12;a++){	
		PB[row][a][value-1] = 0; 
	}
}

void clearCol(int value,int col,vector< vector<vector<int> > >& PB){
	for(int a=0;a<12;a++){	
		PB[a][col][value-1] = 0; 
	}
}

void clearBlock(int value,int row,int col,vector< vector<vector<int> > >& PB){
	int r = 3*(row/3);
	int c = 3*(col/3);
	for(int i=0; i<3;i++){
		for(int j=0;j<3;j++){
			PB[r+i][c+j][value-1]=0;
		}
	}
}

void recoveryRow(int value,int row,vector< vector<vector<int> > >& PB){
	for(int a=0;a<12;a++){	
		PB[row][a][value-1] = value; 
	}
}

void recoveryCol(int value,int col,vector< vector<vector<int> > >& PB){
	for(int a=0;a<12;a++){	
		PB[a][col][value-1] = value; 
	}
}

void recoveryBlock(int value,int row,int col,vector< vector<vector<int> > >& PB){
	int r = 3*(row/3);
	int c = 3*(col/3);
	for(int i=0; i<3;i++){
		for(int j=0;j<3;j++){
			PB[r+i][c+j][value-1]= value;
		}
	}
}

int checkCol(vector<vector<int> >& v){
	for(int col=0;col<12;col++){	
	int a[9] = {0};
	for(int i=0;i<12;i++){
		if(v[i][col]!=-1 && v[i][col]!=0) a[v[i][col]-1]++;
	}	
	for(int j=0;j<9;j++){	
		if(a[j]>1) return 0;
	}

	}

	return 1;
}

int checkRow(vector<vector<int> >& v){
	for(int row=0;row<12;row++){	
	int a[9] = {0};
	for(int i=0;i<12;i++){
		if(v[row][i]!=-1 && v[row][i]!=0) a[v[row][i]-1]++;
	}	
	for(int j=0;j<9;j++){	
		if(a[j]>1) return 0;
	}

	}

	return 1;
}

int checkBlock(vector<vector<int> >& v){
	
	for(int i=0;i<12;i+=3){
		for(int j=0;j<12;j+=3){
			int a[9] = {0};
			if(v[i][j]==-1) continue;

			for(int aa=0;aa<3;aa++){
				for(int b=0;b<3;b++){
					int n = v[i+aa][j+b];
					if(n!=-1 && n!=0)
					a[n-1]++; 
				}
			}
			for(int c=0;c<9;c++){
				if(a[c]>1) return 0;
			}

		}	
	}	

	return 1;
}

inline int checkSudoku(vector<vector<int> >& v){
	return(checkRow(v) && checkCol(v) && checkBlock(v));
}

void getpb(){
	vector<int> rowNum; //store the number which not exist in this row 
	vector<int> colNum;	//store the number which not exist in this column
	vector<int> blkNum;	//store the number which not exist in this block
	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			for(int k=0;k<9;k++) pb[i][j][k] = 0;
		}
	}

	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			rowNum.clear();
			colNum.clear();
			blkNum.clear();
			if(space[i][j]){					
				//find Row Num
				int r[9] = {0};
				for(int a=0;a<12;a++) if(S[i][a]!=-1 && S[i][a]!= 0) r[S[i][a]-1]++;
				for(int a=0;a<9;a++)	if(r[a]==0) rowNum.push_back(a+1);				
				//find Col Num
				int c[9] = {0};
				for(int a=0;a<12;a++) if(S[a][j]!=-1 && S[a][j]!=0) c[S[a][j]-1]++;
				for(int a=0;a<9;a++) if(c[a]==0) colNum.push_back(a+1);
				//find block Num
				int b[9] = {0};
				int ii = 3*(i/3),jj = 3*(j/3);
				for(int a=0;a<3;a++)
					for(int aa=0;aa<3;aa++) if(S[ii+a][jj+aa]!=-1 && S[ii+a][jj+aa]!=0) b[S[ii+a][jj+aa]-1]++;
				for(int a=0;a<9;a++) if(b[a]==0) blkNum.push_back(a+1);

				for(int a=1;a<=9;a++){ 
					if(inVec(a,rowNum) && inVec(a,colNum) && inVec(a,blkNum))
						pb[i][j][a-1] = a; 				
				}

			}
		}
	}
}


int inVec(int num,vector<int> v){
	for(int a=0;a<v.size();a++){
		if(num == v.at(a))	return 1;
	}
	return 0;
}

void getpbNum(){
	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			 pbNum[i][j]= 0;
		}
	}

	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			
			int count =0;
			for(int a=0;a<9;a++){
				if(pb[i][j][a]!=0) count++;
			}
			pbNum[i][j]	= count;
			
		}
	}
}

void getlist(){
	row_list.clear();
	col_list.clear();
	for(int ps=1;ps<=9;ps++){
		for(int i=0;i<12;i++){
			for(int j=0;j<12;j++){
				if(pbNum[i][j]==ps){
					row_list.push_back(i);
					col_list.push_back(j);
				}
			}	
		}
	}	
}


void mirror(vector<vector<int> >& vec){
  int row=12;
	int col=12;
	vector<vector<int> > mirror;
	mirror.resize(12);
	for(int i=0;i< row;i++) mirror[i].resize(12);

 	int ii,jj;

	for(int i=0; i<row; i++)
    {
       for(int j=0; j<col; j++)
       {
          ii = row - 1 - j;
          jj = col - 1 - i;
          mirror[i][j] = vec[ii][jj];
       }
    } 		
		vec = mirror;
}

void turnCol(vector<vector<int> >&v,int a,int b){
	int tmp;
	for(int i=0;i<12;i++){
		tmp = v[i][a];
	 	v[i][a] = v[i][b];
		v[i][b] = tmp;
	}
}
	
void turnRow(vector<vector<int> >&v,int a,int b){
	int tmp;
	for(int j=0;j<12;j++){
		tmp = v[a][j];
	 	v[a][j] = v[b][j];
		v[b][j] = tmp;
	}
}

void swap1(vector<vector<int> >&v){	
	int n = rand()%24;
	switch(n){
		case 0: turnCol(v,0,1);	break;
		case 1:	turnCol(v,0,2);	break;
		case 2:	turnCol(v,1,2);	break;

		case 3:	turnCol(v,3,4);	break;
		case 4:	turnCol(v,3,5);	break;
		case 5:	turnCol(v,4,5);	break;

		case 6:	turnCol(v,6,7);	break;
		case 7:	turnCol(v,6,8);	break;
		case 8:	turnCol(v,7,8);	break;

		case 9:	turnCol(v,9,10); break;
		case 10: turnCol(v,9,11); break;
		case 11: turnCol(v,10,11); break;

		case 12:	turnRow(v,0,1);	break;
		case 13:	turnRow(v,0,2);	break;
		case 14:	turnRow(v,1,2);	break;

		case 15:	turnRow(v,3,4);	break;
		case 16:	turnRow(v,3,5);	break;
		case 17:	turnRow(v,4,5);	break;

		case 18:	turnRow(v,6,7);	break;
		case 19:	turnRow(v,6,8);	break;
		case 20:	turnRow(v,7,8);	break;

		case 21:	turnRow(v,9,10); break;
		case 22: turnRow(v,9,11); break;
		case 23: turnRow(v,10,11); break;
	}
}

void swap3(vector<vector<int> >&v){	
	int n = rand()%12;
	switch(n){
		case 0: 
			turnCol(v,0,3);
			turnCol(v,1,4);
			turnCol(v,2,5);
			break;
		case 1:
			turnCol(v,3,6);
			turnCol(v,4,7);
			turnCol(v,5,8);
			break;
		case 2:
			turnCol(v,6,9);
			turnCol(v,7,10);
			turnCol(v,8,11);
			break;
		case 3:
			turnCol(v,0,6);
			turnCol(v,1,7);
			turnCol(v,2,8);
			break;
		case 4:
			turnCol(v,0,9);
			turnCol(v,1,10);
			turnCol(v,2,11);
			break;
		case 5:
			turnCol(v,3,9);
			turnCol(v,4,10);
			turnCol(v,5,11);
			break;
		case 6:
			turnRow(v,0,3);
			turnRow(v,1,4);
			turnRow(v,2,5);
			break;
		case 7:
			turnRow(v,3,6);
			turnRow(v,4,7);
			turnRow(v,5,8);
			break;
		case 8:
			turnRow(v,6,9);
			turnRow(v,7,10);
			turnRow(v,8,11);
			break;
		case 9:
			turnRow(v,0,6);
			turnRow(v,1,7);
			turnRow(v,2,8);
			break;
		case 10:
			turnRow(v,0,9);
			turnRow(v,1,10);
			turnRow(v,2,11);
			break;
		case 11:
			turnRow(v,3,9);
			turnRow(v,4,10);
			turnRow(v,5,11);
			break;
	}
	
}

void clockwise(vector<vector<int> >& v){
	vector<vector<int> > tmp;
	tmp.resize(12);
	for(int i=0;i<12;i++) tmp[i].resize(12);
	
	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			tmp[i][j] = v[j][11-i];
		}
	}
	v = tmp;
}

void changeNum(){	
	int num[3][9] = {{1,3,4,5,2,6,9,8,7},
									 {2,3,5,6,9,8,1,7,4},
									 {9,8,7,6,4,3,2,1,5}};
	int k=rand()%3;

	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			int tmp = S[i][j];
			switch(tmp){
				case 1: S[i][j] = num[k][0]; break;
				case 2: S[i][j] = num[k][1]; break;
				case 3: S[i][j] = num[k][2]; break;
				case 4: S[i][j] = num[k][3]; break;
				case 5: S[i][j] = num[k][4]; break;
				case 6: S[i][j] = num[k][5]; break;
				case 7: S[i][j] = num[k][6]; break;
				case 8: S[i][j] = num[k][7]; break;
				case 9: S[i][j] = num[k][8]; break;
			}
		}
	}
}

void dighole(vector<vector<int> > &v){
	int count=0;
	vector<int> row_lst;
	vector<int> col_lst;
	for(int i=0;i<12;i++){
		for(int j=0;j<12;j++){
			if(v[i][j]>0){
				row_lst.push_back(i);
				col_lst.push_back(j);
			}
		}
	}	//get the position list which can dig

	vector<int> list; //the position which have been used;

	while(1){  //try to dig hole until the Sudoku problem left one answer
		if(count>=row_lst.size()) {cerr << "Failed to create Sudoku Problem\n";  exit(1);  }
		int n = rand()%row_lst.size();
		while(inVec(n,list)){ n+=7; if(n>=row_lst.size()) n=rand()%row_lst.size(); }
		list.push_back(n);

			
		int row,col;
		row = row_lst.at(n);	
		col = col_lst.at(n);



		int tmp = v[row][col]; 

		v[row][col] = 0;
		getResult();

		if(resultNum == 1){
			return;
		}
		v[row][col] = tmp; //cannot dig 
	}
	count++;
}

