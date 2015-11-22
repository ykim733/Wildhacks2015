#include <iostream>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

class player{
	public:
		string name;
		int hp;
		int charge;
		int handposition; //0 is no value, 1 is charge, 2 is punch, 3 is megapunch, 4 is shield
    void initializep(int health, string n){
        name=n;
        hp = health;
        charge=0;
        handposition=0;
    }
    void assignhandpos(){
        //assumes num(myo1) gives you the hand position value
        cout<<"What move do you want to select?"<<endl;
        cin>>handposition;
    }

    void chargepunch (){
        if((handposition==2 && charge==0) || (handposition==3 && charge<3)){
            hp=hp-1;
            handposition=0;
            cout<<"WARNING: YOU ATTACKED WITHOUT ENOUGH CHARGE, MINUS ONE HP"<<endl;
        }
    }
    void updatecharge(){
        if(handposition==1){
            charge=charge+1;
        }
        else if(handposition==2){
            charge=charge-1;
        }
        else if(handposition==3){
            charge=charge-3;
        }
    }
    void assigndamage (player p){
        if(handposition<=1 && p.handposition==2){
            hp=hp-2;
        }
        else if(handposition<=1 && p.handposition==3){
            hp=hp-8;
        }
        else if(handposition==4 && p.handposition==3){
            hp=hp-6;
        }
    }
    void resethand(){
        handposition=0;
    }
};

/*void probabilitymove(player p){
	srand(time(NULL));
	if(p.handposition==2 && rand() % 100 +1 >95 ){
		p.handposition=0;
		cout<<"You're punch missed"<<endl;
	}
	if(p.handposition==3 && rand() % 100 +1 >85){
		p.handposition=0;
		cout<<"You're Mega-Punch Missed"<<endl;
	}
	if(p.handposition==4 && rand() % 100 +1 >95){
		p.handposition==0;
		cout<<"You're shield failed"<<endl;
	}
}*/

void dispscore(player player1, player player2){
	cout<<"Player1 hp: "<< player1.hp<<endl<<"Player2 hp: "<< player2.hp<<endl;
}

void gameloop(double t, player player1, player player2){
	clock_t start;
	start=clock();
	int duration = 0;
	cout<<"player 1 hp " << player1.hp<<endl;
	cout<<"player 2 hp " << player2.hp<<endl;
	while(duration<t&&player1.hp>0&&player2.hp>0){
		//assign handpositions here:
		player1.assignhandpos();
		player2.assignhandpos();
		player1.chargepunch();
		player2.chargepunch();
		player1.updatecharge();
		player2.updatecharge();
//		probabilitymove(player1);
//		probabilitymove(player2);
		player1.assigndamage(player2);
		player2.assigndamage(player1);
		dispscore(player1, player2);
		player1.resethand();
		player2.resethand();
		duration=(clock()-start)/(double)CLOCKS_PER_SEC;
		if(t-duration>0){
            cout<<"Time Remaining: " << t-duration<<endl;
		}
		else{
            cout<<"Times Up!"<<endl;
		}
	}
	if(player1.hp<player2.hp){
		cout<<"The winner is: " <<player2.name<<endl;
	}
	else if(player2.hp<player1.hp){
		cout<<"The winner is: " <<player1.name<<endl;
	}
	else{
	    cout<<"TIE GAME"<<endl;
	}
}

void initializegame(){
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
	cout<<"How much HP do you desire "<<name2<<" ?"<<endl;
	int hpplay2;
	cin>>hpplay2;
	player1.initializep(hpplay1, name1);
	player2.initializep(hpplay2, name2);
	cout<<"How long do you want to play?"<<endl;
	double t;
	cin>>t;
	gameloop(t, player1, player2);
}

int main()
{
	string play = "y";
	while(play.compare("y")==0){
		initializegame();
		cout<<"Do you wish to continue? Press y for yes, Press anything else for no"<<endl;
		cin>>play;
	}
	return 0;
}
