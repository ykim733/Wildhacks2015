#include <iostream>
#include <time.h>
#include <stdio.h>
#include <math.h> 
using namespace std;

class player{
	public:
		string name;
		int hp;
		int charge;
		int handposition; //0 is no value, 1 is charge, 2 is punch, 3 is megapunch, 4 is shield
};

void chargepunch? (player p){
	if((p.handposition==2 && p.charge==0) || (p.handposition==3 && p.charge<3)){
		p.hp=p.hp-1;
		p.handposition=0;
		cout<<"WARNING: YOU ATTACKED WITHOUT ENOUGH CHARGE, MINUS ONE HP"<<endl;
	}
}

void initializep(player p, int health, string n){
	p.name=n;
	p.hp = health;
	p.charge=0;
	p.handposition=0;
}

void assigndamage (player p1, player p2){
	if(p1.handposition<=1 && p2.handposition==2){
		p1.hp==p1.hp-2;
	}
	else if(p1.handposition==2 && p2.handposition<=1){
		p2.hp=p2.hp-2;
	}
	else if(p1.handposition==3 && p1.handposition<=1){
		p2.hp=p2.hp-8;
	}
	else if(p1.handposition==3 && p1.handposition==4){
		p2.hp=p2.hp-6;
	}
	else if(p2.handposition==3 && p1.handposition<=1){
		p1.hp=p1.hp-8;
	}
	else if(p2.handposition==3 && p1.handposition==4){
		p1.hp=p1.hp-6;
	}
}

void probabilitymove(player p){
	srand(time(NULL));
	if(p.handposition==2 && rand() % 100 +1 >95 ){
		p.handposition=0;
	}
	if(p.handposition==3 && rand() % 100 +1 >85){
		p.handposition=0;
	}
	if(p.handposition==4 && rand() % 100 +1 >95){
		p.handposition==0;
	}
}

void assignhandpos(player p1, player p2, myo myo1, myo myo2){
	//assumes num(myo1) gives you the hand position value
	p1.handposition=num(myo1);
	p2.handposition=num(myo2);
}
void updatecharge(player p){
	if(p.handposition==1){
		p.charge=p.charge+1;
	}
	else if(p.handposition==2){
		p.charge=p.charge-1;
	}
	else if(p.handposition==3){
		p.charge=p.charge-3;
	}
}

player gameloop(double t, player player1, player player2, myo myo1, myo myo2){
	clock_t start;
	start=clock();
	duration = 0;
	while(duration<t||player1.hp<=0||player2.hp<=0){
		//assign handpositions here:
		assignhandpos(player1,player2,myo1,myo2);
		chargepunch?(player1);
		chargepunch?(player2);
		updatecharge(player1);
		updatecharge(player2);
		probabilitymove(player1);
		probabilitymove(player2);
		assigndamage(player1, player2);
		//resets hand positions
		p1.handposition=0;
		p2.handposition=0;
		duration=(clock()-start)/(double)CLOCKS_PER_SEC;
		cout<<"Time Remaining: " << t-duration<<endl;
	}
	if(p1ayer1.hp<player2.hp){
		return player1;
	}
	else if(player2.hp<player1.hp){
		return player2;
	}
	else{
		return NULL;
	}
}

player initializegame(myo1, myo2){
	player player1;
	player player2;
	cout<<"Desired Name Player 1?"<<endl;
	string name1;
	cin>>name1;
	cout<<"Desired Name Player 2?"<<endl;
	string name2;
	cin>>name2;
	cout<<"How much HP do you desire "<<name1<<" ?"<<endl;
	int hpplay1;
	cin>>hpplay1;
	cout<<"How much HP do you desire "<<name1<<" ?"<<endl;
	int hpplay2;
	cin>>hpplay2;
	initializep(player1, hpplay1, name1);
	initializep(player2, hpplay2, name2);
	cout<<"How long do you want to play?"<<endl;
	double t;
	cin>>t;
	return gameloop(t, player1, player2, myo1, myo2);
}

int main{
	myo myo1 = new myo1;
	myo myo2 = new myo2;
	string play=="y";
	while(play.compare("y")==0){
		player winner=initializegame(myo1, myo2);
		if(winner==NULL){
			cout<<"TIE GAME"<<endl;
		}
		else{
			cout<<"THE WINNER IS: "<<winner.name<<endl;
		}
		cout<<"Do you wish to continue? Press y for yes, Press anything else for no"<<endl
		cin>>play;
	}
	return 0;
}