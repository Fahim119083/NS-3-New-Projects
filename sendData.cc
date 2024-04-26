#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

double GetDistance(Vector pos1, Vector pos2) {
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    double dz = pos1.z - pos2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

int main(int argc, char* argv[]) {
    CommandLine cmd;
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer allNodes;
    allNodes.Create(10);

    // Install mobility model to all nodes
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel");
    mobility.Install(allNodes);

    // Create CSMA network
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));
    NetDeviceContainer csmaDevices = csma.Install(allNodes);

    // Install Internet stack
    InternetStackHelper stack;
    stack.Install(allNodes);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(csmaDevices);

    // Start UDP echo server on node 1
    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(allNodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Start UDP echo client on node 9, which sends packets to node 1
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer clientApps = echoClient.Install(allNodes.Get(9));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Calculate distance between node 1 and node 9
    Ptr<MobilityModel> senderMobilityModel = allNodes.Get(9)->GetObject<MobilityModel>();
    Ptr<MobilityModel> receiverMobilityModel = allNodes.Get(1)->GetObject<MobilityModel>();

    if (!senderMobilityModel || !receiverMobilityModel) {
        NS_LOG_ERROR("Failed to retrieve mobility models for sender or receiver.");
        return 1;
    }

    Vector senderPosition = senderMobilityModel->GetPosition();
    Vector receiverPosition = receiverMobilityModel->GetPosition();
    double distance = GetDistance(senderPosition, receiverPosition);
    
    NS_LOG_INFO("Distance between sender (node 9) and receiver (node 1): " << distance << " meters");

    // Configure animation
    AnimationInterface anim("simulation.xml");
    anim.SetConstantPosition(allNodes.Get(9), 5.0, 5.0);
    anim.SetConstantPosition(allNodes.Get(1), 20.0, 20.0);
    anim.EnablePacketMetadata(true);

    // Run simulation
    Simulator::Stop(Seconds(15));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

