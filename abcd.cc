#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleNetworkExample");

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Create nodes
  NodeContainer nodes;
  nodes.Create (10);

  // Create point-to-point links
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  for (uint32_t i = 0; i < nodes.GetN() - 1; ++i) {
    devices.Add(p2p.Install(nodes.Get(i), nodes.Get(i+1)));
  }

  // Install internet stack
  InternetStackHelper stack;
  stack.Install (nodes);

  // Assign IP addresses
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces;
  interfaces = address.Assign (devices);

  // Set up applications for data transmission

  // Node 2 sending data to Node 0
  uint16_t port = 9; // Arbitrary port number
  OnOffHelper onoff1 ("ns3::UdpSocketFactory", InetSocketAddress (interfaces.GetAddress (0), port));
  onoff1.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  ApplicationContainer apps1 = onoff1.Install (nodes.Get (2));
  apps1.Start (Seconds (1.0));
  apps1.Stop (Seconds (10.0));

  // Node 4 sending data to Node 3
  OnOffHelper onoff2 ("ns3::UdpSocketFactory", InetSocketAddress (interfaces.GetAddress (3), port));
  onoff2.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff2.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  ApplicationContainer apps2 = onoff2.Install (nodes.Get (4));
  apps2.Start (Seconds (2.0));
  apps2.Stop (Seconds (10.0));

  // Node 9 sending data to Node 6
  OnOffHelper onoff3 ("ns3::UdpSocketFactory", InetSocketAddress (interfaces.GetAddress (6), port));
  onoff3.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff3.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff3.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  ApplicationContainer apps3 = onoff3.Install (nodes.Get (9));
  apps3.Start (Seconds (3.0));
  apps3.Stop (Seconds (10.0));

  AnimationInterface anim("abcdfiles.xml");

  // Run simulation
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

