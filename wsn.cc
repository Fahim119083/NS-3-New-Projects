#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/config-store-module.h"
#include "ns3/packet-sink.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MobileSinkExample");

void AdvancePosition(Ptr<Node> mobileSink, const std::vector<Vector> &positions, uint32_t &currentPos, Time interval, Ipv4InterfaceContainer &nodeInterfaces, AnimationInterface &anim)
{
    currentPos = (currentPos + 1) % positions.size();
    Ptr<MobilityModel> mobility = mobileSink->GetObject<MobilityModel>();
    if (mobility != nullptr)
    {
        mobility->SetPosition(positions[currentPos]);

        // Update the position of the mobile sink in NetAnim
        anim.SetConstantPosition(mobileSink, positions[currentPos].x, positions[currentPos].y);

        // Print the current position of the mobile sink node
        Vector pos = mobility->GetPosition();
        std::cout << "Mobile sink moved to position: (" << pos.x << ", " << pos.y << ")" << std::endl;

        // If the mobile sink is at the base station (node 0), simulate data collection
        if (currentPos == positions.size() - 1)
        {
            Ptr<PacketSink> sink = DynamicCast<PacketSink>(mobileSink->GetApplication(0));
            if (sink != nullptr)
            {
                for (uint32_t i = 1; i < nodeInterfaces.GetN(); ++i)
                {
                    uint64_t totalBytes = sink->GetTotalRx();
                    std::cout << "Collected data from node " << i << ": " << totalBytes << " bytes" << std::endl;
                }
            }
        }
    }

    Simulator::Schedule(interval, &AdvancePosition, mobileSink, positions, std::ref(currentPos), interval, std::ref(nodeInterfaces), std::ref(anim));
}

int main(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // Create 4 nodes
    NodeContainer nodes;
    nodes.Create(4);

    // Set positions of the nodes
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(14.0, 30.0, 0.0));  // Node 0 (base station)
    positionAlloc->Add(Vector(33.0, 20.0, 0.0)); // Node 1
    positionAlloc->Add(Vector(23.0, 39.0, 0.0)); // Node 2
    positionAlloc->Add(Vector(37.0, 33.0, 0.0)); // Node 3

    MobilityHelper mobility;
    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    // Create a mobile sink node
    Ptr<Node> mobileSink = CreateObject<Node>();
    Ptr<ListPositionAllocator> sinkPositionAlloc = CreateObject<ListPositionAllocator>();
    sinkPositionAlloc->Add(Vector(14.0, 30.0, 0.0)); // Initial position (same as Node 0)
    MobilityHelper sinkMobility;
    sinkMobility.SetPositionAllocator(sinkPositionAlloc);
    sinkMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    sinkMobility.Install(mobileSink);

    // Install Internet stack
    InternetStackHelper internet;
    internet.Install(nodes);
    internet.Install(mobileSink);

    // Create point-to-point connections and NetDeviceContainers
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices;
    for (uint32_t i = 0; i < nodes.GetN(); ++i) 
    {
        devices.Add(p2p.Install(NodeContainer(nodes.Get(i), mobileSink)));
    }

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer nodeInterfaces = ipv4.Assign(devices);

    // Define the positions the mobile sink will travel to
    std::vector<Vector> positions = {
        Vector(14.0, 30.0, 0.0),  // Base station (Node 0)
        Vector(33.0, 20.0, 0.0), // Node 1
        Vector(37.0, 33.0, 0.0), // Node 3
        Vector(23.0, 39.0, 0.0), // Node 2
        Vector(14.0, 30.0, 0.0)   // Back to base station (Node 0)
    };

    // Create the NetAnim animation interface
    AnimationInterface anim("wsnfms.xml");

    // Set initial positions of nodes in NetAnim
    anim.SetConstantPosition(nodes.Get(0), 14.0, 30.0);
    anim.SetConstantPosition(nodes.Get(1), 33.0, 20.0);
    anim.SetConstantPosition(nodes.Get(2), 23.0, 39.0);
    anim.SetConstantPosition(nodes.Get(3), 37.0, 33.0);
    anim.SetConstantPosition(mobileSink, 14.0, 30.0);

    // Schedule the mobile sink movement
    uint32_t currentPos = 0;
    Time interval = Seconds(5.0); // Change position every 5 seconds
    Simulator::Schedule(interval, &AdvancePosition, mobileSink, std::cref(positions), std::ref(currentPos), interval, std::ref(nodeInterfaces), std::ref(anim));

    // Set up applications (e.g., PacketSink on mobile sink to receive data)
    uint16_t port = 9; // Port number for the PacketSink
    Address sinkAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", sinkAddress);
    ApplicationContainer sinkApps = packetSinkHelper.Install(mobileSink);
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(50.0));

    // Add applications, routing, etc. as needed
    // For simplicity, this example does not include sending applications on the nodes

    // Enable global routing
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Run the simulation
    Simulator::Stop(Seconds(50.0));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
