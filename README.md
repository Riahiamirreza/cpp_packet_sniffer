# Packet sniffer

it's a very simple packet sniffer.
for run follow these simple steps:

  - install libpcap and libjson-cpp
  - run command `chmod +x complile.sh`
  - run command `./compile`
  - then run `g++ list_all_devices.cpp -lpcap -o list.out`
  - for seeing at available devices on network run `./list.out`
  - the run `sudo ./pcap.out <deivice-name> <number-of-packets-to-capture> <log-level>`
for example:
    `sudo ./pcap enp3s0 50 3`
