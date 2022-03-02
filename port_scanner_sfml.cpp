#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <thread>

// Made in Turkey by Ahmet Eren Laçinbala and Murat Öztürk
// ahmetlacinbala@gmail.com
// memuratozturk@gmail.com


using std::cout;
using std::string;
using std::cin;

const int portCount = 65535;
const int num_threads = 64;
int portStatus[portCount];

auto start = std::chrono::steady_clock::now();
void is_port_open(const std::string& address, int port, int timeout)
{
    using namespace std::chrono_literals;
    sf::Time Timeout = sf::milliseconds(timeout);
    int check = (sf::TcpSocket().connect(address, port, Timeout));
    switch (check) {
    case 0:
        //open
        portStatus[port] = 1;
        cout << "\nPORT " << port << " is open\n";
        break;
    case 1:
        //closed
        portStatus[port] = 0;
        break;
    case 4:
        //timeout
        portStatus[port] = 2;
        break;
    }
    if (port == portCount) {
        std::cout << "\nALL PORTS ARE CHECKED in ";
        auto end = std::chrono::steady_clock::now();
        cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds\n";
    }
    if (port < portCount)
        is_port_open(address, port + num_threads, timeout);
}

int main()
{
    using std::thread;
    setlocale(LC_ALL, "Turkish");
    cout << "About: \n";
    cout << "Made by Ahmet Eren Laçinbala and Murat Öztürk\n";
    cout << "For contact:\n";
    cout << "ahmetlacinbala@gmail.com\nmemuratozturk@gmail.com\n\n";

    for (int i = 0; i < portCount; i++)
        portStatus[i] = 99;

    thread t([]() {
        while (1) {
            int check;
            cin >> check;
            if (check > 65534) cout << "INPUT IS OUT OF RANGE\n";
            else {
                switch (portStatus[check]) {

                case 1:
                    cout << "OPEN\n\n";
                    break;
                case 0:
                    cout << "CLOSED\n\n";
                    break;
                case 2:
                    cout << "TIMEOUT\n\n";
                    break;
                case 99:
                    cout << "NOT TESTED. ";
                    while (portStatus[check] == 99) {
                        check--;
                    }
                    cout << "Last checked port: " << check << "\n\n";
                }
            }
        }
        });

    cout << "Enter IP: ";
    string ip;
    cin >> ip;
    thread checkThread[num_threads];
    for (int i = 0; i < num_threads-1; i++) {
        checkThread[i] = thread(is_port_open, ip, i, 300);
    }

    for (int i = 0; i < num_threads - 1; i++) {
        checkThread[i].join();
    }

    t.join();  

}