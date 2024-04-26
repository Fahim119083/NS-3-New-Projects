/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.                                                  
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-standards.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
// 

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int main(int argc, char* argv[])
{
    CommandLine cmd;
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Step-1 creating a group number of nodes
    NodeContainer allNodes;
    allNodes.Create(10);

    // Step-2 create link
    CsmaHelper homeLan;
    homeLan.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    homeLan.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    // Step-3 creating devices
    NetDeviceContainer csmaDevices;
    csmaDevices = homeLan.Install(allNodes);

    MobilityHelper m;
    Ptr<ListPositionAllocator> p = CreateObject<ListPositionAllocator>();
    p->Add(Vector(30,20,0));
    p->Add(Vector(25,20,0));
    p->Add(Vector(30,40,0));
    p->Add(Vector(25,20,0));
    p->Add(Vector(15,10,0));
    p->Add(Vector(35,25,0));
    p->Add(Vector(22,10,0));
    p->Add(Vector(32,22,0));
    p->Add(Vector(15,18,0));
    p->Add(Vector(25,10,0));
    m.SetPositionAllocator(p);
    
    // Random way point mobility = Paused Randmize Change
    m.SetMobilityModel("ns3::RandomWaypointMobilityModel",
                        "Speed", StringValue("ns3::ConstantRandomVariable[Constant=1.0]"),
                        "Pause", StringValue("ns3::ConstantRandomVariable[Constant=0.0]"),
                        "PositionAllocator", PointerValue(p));

    m.Install(allNodes.Get(1));
    
    // Random Walk Mobility = Constant Randomize Change
    m.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                        "Bounds", RectangleValue(Rectangle(-500,500,-500,500)));
    m.Install(allNodes.Get(2));
    m.Install(allNodes.Get(3));
    m.Install(allNodes.Get(4));
    m.Install(allNodes.Get(5));
    
    m.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    m.Install(allNodes.Get(0));
    m.Install(allNodes.Get(6));
    m.Install(allNodes.Get(7));
    m.Install(allNodes.Get(8));
    m.Install(allNodes.Get(9));
   
    // Step-4 Install IP stack
    InternetStackHelper stack;
    stack.Install(allNodes);

    // Step-5 Assignment of IP Address
    Ipv4AddressHelper address;
    address.SetBase("54.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer interfacesHomeLan = address.Assign(csmaDevices);
    
    // Step-6 Server configuration
    UdpEchoServerHelper echoServer(54);

    ApplicationContainer serverApps1 = echoServer.Install(allNodes.Get(1));
    serverApps1.Start(Seconds(1.0));
    serverApps1.Stop(Seconds(11.0));

    ApplicationContainer serverApps2 = echoServer.Install(allNodes.Get(8));
    serverApps2.Start(Seconds(2.0));
    serverApps2.Stop(Seconds(11.0));

    // Step-7 Client configuration
    UdpEchoClientHelper echoClient1(interfacesHomeLan.GetAddress(1), 54);
    echoClient1.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient1.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps1 = echoClient1.Install(allNodes.Get(9));
    clientApps1.Start(Seconds(2.0));
    clientApps1.Stop(Seconds(11.0));

    UdpEchoClientHelper echoClient2(interfacesHomeLan.GetAddress(1), 54);
    echoClient2.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient2.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient2.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps2 = echoClient2.Install(allNodes.Get(7));
    clientApps2.Start(Seconds(1.0));
    clientApps2.Stop(Seconds(11.0));
    
        UdpEchoClientHelper echoClient3(interfacesHomeLan.GetAddress(1), 54);
    echoClient2.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient2.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient2.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps3 = echoClient3.Install(allNodes.Get(8));
    clientApps2.Start(Seconds(1.0));
    clientApps2.Stop(Seconds(11.0));
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    AnimationInterface anim("filess.xml");

    Simulator::Stop(Seconds(15));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
