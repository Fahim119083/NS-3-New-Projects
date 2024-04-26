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

double GetDistance(Vector pos1, Vector pos2) {
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    double dz = pos1.z - pos2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

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
    echoClient3.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient3.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient3.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps3 = echoClient3.Install(allNodes.Get(8));
    clientApps3.Start(Seconds(1.0));
    clientApps3.Stop(Seconds(11.0));
    
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    AnimationInterface anim("mnopfiless.xml");

    Simulator::Stop(Seconds(15));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}

