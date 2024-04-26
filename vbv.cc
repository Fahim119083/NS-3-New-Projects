#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
    // Enable logging
    LogComponentEnable("RandomWalk2dMobilityModel", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer nodes;
    nodes.Create(10);

    // Set up mobility
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

    // Set the total area (500m x 500m)
    positionAlloc->Add(Vector(0, 0, 0));
    positionAlloc->Add(Vector(500, 0, 0));
    positionAlloc->Add(Vector(0, 500, 0));
    positionAlloc->Add(Vector(500, 500, 0));

    // Assign random positions to nodes
    positionAlloc->Create(10);

    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    // Install mobility to nodes
    mobility.Install(nodes);

    // Create and install Internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IPv4 addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("192.168.1.0", "255.255.255.0");

    Ipv4InterfaceContainer interfaces = ipv4.Assign(devices);

    // Print IP addresses
    for (uint32_t i = 0; i < nodes.GetN(); ++i) {
        Ptr<Node> node = nodes.Get(i);
        Ptr<Ipv4> ipv4 = node->GetObject<Ipv4>();
        Ipv4Address ip = ipv4->GetAddress(1, 0).GetLocal();
        std::cout << "Node " << i << " IP: " << ip << std::endl;
    }
    
    AnimationInterface anim("animation.xml");

    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}

