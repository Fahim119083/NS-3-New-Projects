#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/point-to-point-helper.h"


using namespace ns3;

double GetDistance(Vector pos1, Vector pos2)
{
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main(int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper p2p;
    NetDeviceContainer devices = p2p.Install(nodes);

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(nodes);

    AnimationInterface anim("animationsss.xml");
    anim.SetConstantPosition(nodes.Get(0), 10.0, 10.0);
    anim.SetConstantPosition(nodes.Get(1), 20.0, 20.0);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

