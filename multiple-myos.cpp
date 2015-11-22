// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Distributed under the Myo SDK license agreement. See LICENSE.txt for details.

// This sample illustrates how to interface with multiple Myo armbands and distinguish between them.

#include <iostream>
#include <stdexcept>
#include <vector>
#include <math.h> 
#include <time.h>

#include <myo/myo.hpp>
using namespace std;

class player{
public:
	string name;
	int hp;
	int charge;
	int handposition; //0 is no value, 1 is charge, 2 is punch, 3 is megapunch, 4 is shield
};

void chargepunch (player p){
	if ((p.handposition == 2 && p.charge == 0) || (p.handposition == 3 && p.charge<3)){
		p.hp = p.hp - 1;
		p.handposition = 0;
		cout << "WARNING: YOU ATTACKED WITHOUT ENOUGH CHARGE, MINUS ONE HP" << endl;
	}
};

void initializep(player p, int health, string n){
	p.name = n;
	p.hp = health;
	p.charge = 0;
	p.handposition = 0;
}

void assigndamage(player p1, player p2){
	if (p1.handposition <= 1 && p2.handposition == 2){
		p1.hp == p1.hp - 2;
	}
	else if (p1.handposition == 2 && p2.handposition <= 1){
		p2.hp = p2.hp - 2;
	}
	else if (p1.handposition == 3 && p1.handposition <= 1){
		p2.hp = p2.hp - 8;
	}
	else if (p1.handposition == 3 && p1.handposition == 4){
		p2.hp = p2.hp - 6;
	}
	else if (p2.handposition == 3 && p1.handposition <= 1){
		p1.hp = p1.hp - 8;
	}
	else if (p2.handposition == 3 && p1.handposition == 4){
		p1.hp = p1.hp - 6;
	}
}

void probabilitymove(player p){
	srand(time(NULL));
	if (p.handposition == 2 && rand() % 100 + 1 >95){
		p.handposition = 0;
		cout << "Your punch missed" << endl;
	}
	if (p.handposition == 3 && rand() % 100 + 1 >85){
		p.handposition = 0;
		cout << "Your Mega-Punch missed" << endl;
	}
	if (p.handposition == 4 && rand() % 100 + 1 >95){
		p.handposition == 0;
		cout << "Your shield failed" << endl;
	}
}

void updatecharge(player p){
	if (p.handposition == 1){
		p.charge = p.charge + 1;
	}
	else if (p.handposition == 2){
		p.charge = p.charge - 1;
	}
	else if (p.handposition == 3){
		p.charge = p.charge - 3;
	}
}



class PrintMyoEvents : public myo::DeviceListener {
public:
    // Every time Myo Connect successfully pairs with a Myo armband, this function will be called.
    //
    // You can rely on the following rules:
    //  - onPair() will only be called once for each Myo device
    //  - no other events will occur involving a given Myo device before onPair() is called with it
    //
    // If you need to do some kind of per-Myo preparation before handling events, you can safely do it in onPair().
    void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
    {
        // Print out the MAC address of the armband we paired with.

        // The pointer address we get for a Myo is unique - in other words, it's safe to compare two Myo pointers to
        // see if they're referring to the same Myo.

        // Add the Myo pointer to our list of known Myo devices. This list is used to implement identifyMyo() below so
        // that we can give each Myo a nice short identifier.
        knownMyos.push_back(myo);

        // Now that we've added it to our list, get our short ID for it and print it out.
        std::cout << "Paired with " << identifyMyo(myo) << "." << std::endl;
    }

    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
		if (pose.toString() == "fist") {
			std::cout << "Player " << identifyMyo(myo) << " punched!" << std::endl;
			myo->vibrate(myo::Myo::vibrationShort);
		}
		else if (pose.toString() == "fingersSpread") {
			std::cout << "Player " << identifyMyo(myo) << " shielded!" << std::endl;
			myo->vibrate(myo::Myo::vibrationLong);
		}
		else if (pose.toString() == "doubleTap") {
			std::cout << "Player " << identifyMyo(myo) << " is charging!" << std::endl;
			myo->vibrate(myo::Myo::vibrationShort);
			myo->vibrate(myo::Myo::vibrationShort);
			myo->vibrate(myo::Myo::vibrationShort);
		}
		
    }

    void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
    {
        std::cout << "Player " << identifyMyo(myo) << " has connected." << std::endl;
    }

    void onDisconnect(myo::Myo* myo, uint64_t timestamp)
    {
        std::cout << "Player " << identifyMyo(myo) << " has disconnected." << std::endl;
    }

    // This is a utility function implemented for this sample that maps a myo::Myo* to a unique ID starting at 1.
    // It does so by looking for the Myo pointer in knownMyos, which onPair() adds each Myo into as it is paired.
    size_t identifyMyo(myo::Myo* myo) {
        // Walk through the list of Myo devices that we've seen pairing events for.
        for (size_t i = 0; i < knownMyos.size(); ++i) {
            // If two Myo pointers compare equal, they refer to the same Myo device.
            if (knownMyos[i] == myo) {
                return i + 1;
            }
        }

        return 0;
    }

    // We store each Myo pointer that we pair with in this list, so that we can keep track of the order we've seen
    // each Myo and give it a unique short identifier (see onPair() and identifyMyo() above).
    std::vector<myo::Myo*> knownMyos;
};

int main(int argc, char** argv)
{
    try {
        myo::Hub hub("com.example.multiple-myos");

        // Instantiate the PrintMyoEvents class we defined above, and attach it as a listener to our Hub.
        PrintMyoEvents printer;
        hub.addListener(&printer);
		hub.setLockingPolicy(myo::Hub::lockingPolicyNone);
        while (1) {
            // Process events for 10 milliseconds at a time.
            hub.run(10);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }
}
