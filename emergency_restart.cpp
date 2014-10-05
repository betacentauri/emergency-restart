#include <linux/input.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <thread>

using namespace std;

vector<string> devices;

void find_device()
{
	string line;
	string event;
	long ev = 0;
	size_t pos;
	ifstream f("/proc/bus/input/devices");
	if (f.is_open())
	{
		while(getline(f, line))
		{
			if (line == "")
			{
				// start of new device -> reset read data
				event = "";
				ev = 0;
			}
			if (line.find("Handlers=") != string::npos)
			{
				pos = line.find("event");
				if (pos != string::npos && pos + 5 < line.size())
				{
					event = line.substr(pos);
					event.erase(event.find_last_not_of(" ") + 1);
				}
			}
			if ((pos = line.find("EV=")) != string::npos)
			{
				ev = strtol(line.substr(pos+3).c_str(), NULL, 16);
				if (!((ev & 0x1) == 0x1 && (ev & 0x2) == 0x2 && (ev & 0x10) == 0x10 && (ev & 0x100000) == 0x100000))
				{
					ev = 0;
				}
			}
			if (event != "" && ev != 0)
			{
				devices.push_back("/dev/input/" + event);
				ev = 0; // don't add device more than once
			}
		}
		f.close();
	}
	else
	{
		cout << "Unable to open file /proc/bus/input/devices -> exit";
		exit(0);
	}
	cout << "Using devices: " << endl;
	for (unsigned i = 0; i < devices.size(); i++)
		cout << "  " << devices[i] << endl;
}

void readDevice(string device)
{
	struct input_event ev;
	int key3Times = KEY_OK;
	int keyE2Restart = KEY_MENU;
	int keyBoxRestart = KEY_EXIT;
	vector<input_event> keyVector;

	ifstream dev(device.c_str(), ios::in | ios::binary);
	if (dev.is_open())
	{
		while(1)
		{
			dev.read((char*)&ev, sizeof(ev));
			if(ev.type == EV_KEY && ev.value == 0)
			{
				if (ev.code == key3Times)
				{
					keyVector.push_back(ev);
					if (keyVector.size() > 3)
						keyVector.erase(keyVector.begin());
				}
				else if ((ev.code == keyE2Restart || ev.code == keyBoxRestart) && keyVector.size() == 3)
				{
					if (ev.time.tv_sec - keyVector.front().time.tv_sec < 3)
					{
						if (ev.code == keyE2Restart)
						{
							cout << "Restarting E2 ..." << endl;
							system("init 4; sleep 2; init 3");
						}
						else
						{
							cout << "Restarting box ..." << endl;
							system("shutdown -r now");
						}
					}
					keyVector.clear();
				}
				else
				{
					if (keyVector.size() > 0)
						keyVector.clear();
				}
			}
		}
	}
	else
	{
		cout << "Cannot open device " << device << endl;
	}
}

int main()
{
	vector<std::thread> threads;

	find_device();
	for (unsigned i = 0; i < devices.size(); i++)
	{
		threads.push_back(thread(readDevice, devices[i]));
		cout << "Starting thread for " << devices[i] << endl;
	}

	for(auto& thread : threads)
	{
		thread.join();
	}
}