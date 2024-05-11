#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleExample");

int main(int argc, char* argv[]) {
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // Create nodes
    NodeContainer nodes;
    nodes.Create(21); // Updated to create 21 nodes

    // Set positions for each node
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add(Vector(16, 23, 0.0));   // Node 0
    positionAlloc->Add(Vector(9.0, 22.0, 0.0));   // Node 1
    positionAlloc->Add(Vector(11.1383, 27.28874, 0.0));   // Node 2
    positionAlloc->Add(Vector(13,30, 0.0));   // Node 3
    positionAlloc->Add(Vector(3,26, 0.0));   // Node 4
    positionAlloc->Add(Vector(15.0, 15.0, 0.0));     // Node 5
    positionAlloc->Add(Vector(7.0, 6.0, 0.0));     // Node 6
    positionAlloc->Add(Vector(12.0, 10.0, 0.0));   // Node 7
    positionAlloc->Add(Vector(12.0, 7.0, 0.0));     // Node 8
    positionAlloc->Add(Vector(13.0, 16.0, 0.0));     // Node 9
    positionAlloc->Add(Vector(6.0, 12.0, 0.0));    // Node 10
    positionAlloc->Add(Vector(15.0, 2.0, 0.0));    // Node 11
    positionAlloc->Add(Vector(18.0, 3.0, 0.0));    // Node 12
    positionAlloc->Add(Vector(22.0, 4.0, 0.0));    // Node 13
    positionAlloc->Add(Vector(16.0, 4.0, 0.0));    // Node 14
    positionAlloc->Add(Vector(22, 16, 0.0));    // Node 15
    positionAlloc->Add(Vector(27, 15, 0.0));    // Node 16
    positionAlloc->Add(Vector(24, 11, 0.0));    // Node 17
    positionAlloc->Add(Vector(20, 11, 0.0));    // Node 18
    positionAlloc->Add(Vector(26, 14, 0.0));    // Node 19
    positionAlloc->Add(Vector(11, 20, 0.0));    // New DataCollectorNode position
    mobility.SetPositionAllocator(positionAlloc);
    mobility.Install(nodes);

    // Install internet stack
    InternetStackHelper stack;
    stack.Install(nodes);

    // Create and install network devices
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices01 = pointToPoint.Install(nodes.Get(0), nodes.Get(2));   // Node 0 to Node 2
    NetDeviceContainer devices11 = pointToPoint.Install(nodes.Get(1), nodes.Get(2));   // Node 1 to Node 2
    NetDeviceContainer devices31 = pointToPoint.Install(nodes.Get(3), nodes.Get(2));   // Node 3 to Node 2
    NetDeviceContainer devices41 = pointToPoint.Install(nodes.Get(4), nodes.Get(2));   // Node 4 to Node 2
    NetDeviceContainer devices52 = pointToPoint.Install(nodes.Get(5), nodes.Get(7));   // Node 5 to Node 7
    NetDeviceContainer devices62 = pointToPoint.Install(nodes.Get(6), nodes.Get(7));   // Node 6 to Node 7
    NetDeviceContainer devices82 = pointToPoint.Install(nodes.Get(8), nodes.Get(7));   // Node 8 to Node 7
    NetDeviceContainer devices92 = pointToPoint.Install(nodes.Get(9), nodes.Get(7));   // Node 9 to Node 7
    NetDeviceContainer devices102 = pointToPoint.Install(nodes.Get(10), nodes.Get(7)); // Node 10 to Node 7
    NetDeviceContainer devices1112 = pointToPoint.Install(nodes.Get(11), nodes.Get(12)); // Node 11 to Node 12
    NetDeviceContainer devices1312 = pointToPoint.Install(nodes.Get(13), nodes.Get(12)); // Node 13 to Node 12
    NetDeviceContainer devices1412 = pointToPoint.Install(nodes.Get(14), nodes.Get(12)); // Node 14 to Node 12
    NetDeviceContainer devices1516 = pointToPoint.Install(nodes.Get(15), nodes.Get(7)); // Node 15 to Node 7
    NetDeviceContainer devices1617 = pointToPoint.Install(nodes.Get(16), nodes.Get(15)); // Node 16 to Node 15
    NetDeviceContainer devices1718 = pointToPoint.Install(nodes.Get(17), nodes.Get(15)); // Node 17 to Node 15
    NetDeviceContainer devices1819 = pointToPoint.Install(nodes.Get(18), nodes.Get(15)); // Node 18 to Node 15
    NetDeviceContainer devices1920 = pointToPoint.Install(nodes.Get(19), nodes.Get(15)); // Node 19 to Node 15
    // Add similar point-to-point links for other nodes as before
    // Add links for the new node 20 (DataCollectorNode)
    NetDeviceContainer devices202 = pointToPoint.Install(nodes.Get(2), nodes.Get(7));   // Node 2 to Node 7
    NetDeviceContainer devices712 = pointToPoint.Install(nodes.Get(7), nodes.Get(12));  // Node 7 to Node 12
    NetDeviceContainer devices1215 = pointToPoint.Install(nodes.Get(12), nodes.Get(15)); // Node 12 to Node 15

    // Assign IPv4 addresses
    Ipv4AddressHelper address;
    address.SetBase("10.0.0.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces01 = address.Assign(devices01);
    Ipv4InterfaceContainer interfaces11 = address.Assign(devices11);
    Ipv4InterfaceContainer interfaces31 = address.Assign(devices31);
    Ipv4InterfaceContainer interfaces41 = address.Assign(devices41);
    Ipv4InterfaceContainer interfaces52 = address.Assign(devices52);
    Ipv4InterfaceContainer interfaces62 = address.Assign(devices62);
    Ipv4InterfaceContainer interfaces82 = address.Assign(devices82);
    Ipv4InterfaceContainer interfaces92 = address.Assign(devices92);
    Ipv4InterfaceContainer interfaces102 = address.Assign(devices102);
    Ipv4InterfaceContainer interfaces1112 = address.Assign(devices1112);
    Ipv4InterfaceContainer interfaces1312 = address.Assign(devices1312);
    Ipv4InterfaceContainer interfaces1412 = address.Assign(devices1412);
    Ipv4InterfaceContainer interfaces1516 = address.Assign(devices1516);
    Ipv4InterfaceContainer interfaces1617 = address.Assign(devices1617);
    Ipv4InterfaceContainer interfaces1718 = address.Assign(devices1718);
    Ipv4InterfaceContainer interfaces1819 = address.Assign(devices1819);
    Ipv4InterfaceContainer interfaces1920 = address.Assign(devices1920);
    // Assign addresses for the new links
    Ipv4InterfaceContainer interfaces202 = address.Assign(devices202);
    Ipv4InterfaceContainer interfaces712 = address.Assign(devices712);
    Ipv4InterfaceContainer interfaces1215 = address.Assign(devices1215);

    // Create server application on Node 7
    UdpEchoServerHelper echoServer7(7);
    ApplicationContainer serverApp7 = echoServer7.Install(nodes.Get(7));
    serverApp7.Start(Seconds(1.0));
    serverApp7.Stop(Seconds(10.0));

    // Create server application on Node 12
    UdpEchoServerHelper echoServer12(12);
    ApplicationContainer serverApp12 = echoServer12.Install(nodes.Get(12));
    serverApp12.Start(Seconds(1.0));
    serverApp12.Stop(Seconds(10.0));

    // Install client applications on Nodes 5, 6, 8, 9, and 10 to send data to Node 7
    UdpEchoClientHelper echoClient57(interfaces52.GetAddress(1), 7);   // Client sends data to Node 7 from Node 5
    UdpEchoClientHelper echoClient67(interfaces62.GetAddress(1), 7);   // Client sends data to Node 7 from Node 6
    UdpEchoClientHelper echoClient87(interfaces82.GetAddress(1), 7);   // Client sends data to Node 7 from Node 8
    UdpEchoClientHelper echoClient97(interfaces92.GetAddress(1), 7);   // Client sends data to Node 7 from Node 9
    UdpEchoClientHelper echoClient107(interfaces102.GetAddress(1), 7); // Client sends data to Node 7 from Node 10

    // Install client applications on Nodes 0, 1, 3, and 4 to send data to Node 2
    UdpEchoClientHelper echoClient02(interfaces01.GetAddress(1), 2);   // Client sends data to Node 2 from Node 0
    UdpEchoClientHelper echoClient12(interfaces11.GetAddress(1), 2);   // Client sends data to Node 2 from Node 1
    UdpEchoClientHelper echoClient32(interfaces31.GetAddress(1), 2);   // Client sends data to Node 2 from Node 3
    UdpEchoClientHelper echoClient42(interfaces41.GetAddress(1), 2);   // Client sends data to Node 2 from Node 4

    // Install client applications on Nodes 11, 13, and 14 to send data to Node 12
    UdpEchoClientHelper echoClient1112(interfaces1112.GetAddress(1), 12); // Client sends data to Node 12 from Node 11
    UdpEchoClientHelper echoClient1312(interfaces1312.GetAddress(1), 12); // Client sends data to Node 12 from Node 13
    UdpEchoClientHelper echoClient1412(interfaces1412.GetAddress(1), 12); // Client sends data to Node 12 from Node 14

    // Install client applications on Nodes 16, 17, 18, and 19 to send data to Node 15
    UdpEchoClientHelper echoClient1615(interfaces1617.GetAddress(1), 15); // Client sends data to Node 15 from Node 16
    UdpEchoClientHelper echoClient1715(interfaces1718.GetAddress(1), 15); // Client sends data to Node 15 from Node 17
    UdpEchoClientHelper echoClient1815(interfaces1819.GetAddress(1), 15); // Client sends data to Node 15 from Node 18
    UdpEchoClientHelper echoClient1915(interfaces1920.GetAddress(1), 15); // Client sends data to Node 15 from Node 19

    // Install client applications on DataCollectorNode to collect data
    UdpEchoClientHelper echoClient202(interfaces202.GetAddress(1), 20);  // Collect data from Node 2
    UdpEchoClientHelper echoClient712(interfaces712.GetAddress(1), 20);  // Collect data from Node 7
    UdpEchoClientHelper echoClient1212(interfaces1215.GetAddress(1), 20); // Collect data from Node 12
    UdpEchoClientHelper echoClient1515(interfaces1215.GetAddress(1), 20); // Collect data from Node 15

    // Set attributes for client applications
    echoClient57.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient57.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient57.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient67.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient67.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient67.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient87.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient87.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient87.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient97.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient97.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient97.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient107.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient107.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient107.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient02.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient02.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient02.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient12.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient12.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient12.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient32.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient32.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient32.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient42.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient42.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient42.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1112.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1112.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1112.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1312.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1312.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1312.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1412.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1412.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1412.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1615.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1615.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1615.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1715.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1715.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1715.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1815.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1815.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1815.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1915.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1915.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1915.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient202.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient202.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient202.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient712.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient712.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient712.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1212.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1212.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1212.SetAttribute("PacketSize", UintegerValue(1024));

    echoClient1515.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient1515.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1515.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps;
    clientApps.Add(echoClient57.Install(nodes.Get(5)));     // Install client on Node 5
    clientApps.Add(echoClient67.Install(nodes.Get(6)));     // Install client on Node 6
    clientApps.Add(echoClient87.Install(nodes.Get(8)));     // Install client on Node 8
    clientApps.Add(echoClient97.Install(nodes.Get(9)));     // Install client on Node 9
    clientApps.Add(echoClient107.Install(nodes.Get(10)));   // Install client on Node 10
    clientApps.Add(echoClient02.Install(nodes.Get(0)));     // Install client on Node 0
    clientApps.Add(echoClient12.Install(nodes.Get(1)));     // Install client on Node 1
    clientApps.Add(echoClient32.Install(nodes.Get(3)));     // Install client on Node 3
    clientApps.Add(echoClient42.Install(nodes.Get(4)));     // Install client on Node 4
    clientApps.Add(echoClient1112.Install(nodes.Get(11))); // Install client on Node 11
    clientApps.Add(echoClient1312.Install(nodes.Get(13))); // Install client on Node 13
    clientApps.Add(echoClient1412.Install(nodes.Get(14))); // Install client on Node 14
    clientApps.Add(echoClient1615.Install(nodes.Get(16))); // Install client on Node 16
    clientApps.Add(echoClient1715.Install(nodes.Get(17))); // Install client on Node 17
    clientApps.Add(echoClient1815.Install(nodes.Get(18))); // Install client on Node 18
    clientApps.Add(echoClient1915.Install(nodes.Get(19))); // Install client on Node 19
    // Install client on DataCollectorNode
    clientApps.Add(echoClient202.Install(nodes.Get(20)));  // Install client on DataCollectorNode
    clientApps.Add(echoClient712.Install(nodes.Get(20)));  // Install client on DataCollectorNode
    clientApps.Add(echoClient1212.Install(nodes.Get(20))); // Install client on DataCollectorNode
    clientApps.Add(echoClient1515.Install(nodes.Get(20))); // Install client on DataCollectorNode

     AnimationInterface anim("fifth.xml");

    // Start simulation
    Simulator::Stop(Seconds(11.0)); // Update simulation end time
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
