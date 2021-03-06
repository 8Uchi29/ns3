#include <iostream>
#include <fstream>
#include <string>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"

#define NET_MASK "255.255.255.0"
#define NET_ADDR "192.168.1.0"

#define FIRST_NO "0.0.0.1"

#define SIM_START   0.10
#define PORT        50000
#define CW_INTERVAL 0.01

#define PROG_DIR    "local/exp08/data/"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("exp08-LongFatPipe");

uint32_t bottleNeckLinkDL=2;

void CwndTracer(Ptr<OutputStreamWrapper> stream,
				uint32_t oldcwnd, uint32_t newcwnd)
{
		*stream->GetStream() << bottleNeckLinkDL << " \t" << Simulator::Now().GetSeconds()
							<< " \t" << newcwnd << std::endl;
}

void MyEventHandller(Ptr<Application> app, Ptr<OutputStreamWrapper> st)
{
		Ptr<Socket> src_socket = app->GetObject<OnOffApplication>()->GetSocket();
		src_socket->TraceConnectWithoutContext("CongestionWindow", MakeBoundCallback(&CwndTracer, st));
}

void MyEventWrapper(Ptr<Application> app, Ptr<OutputStreamWrapper> st)
{
		Simulator::Schedule(Seconds(CW_INTERVAL), &MyEventHandler, app, st);
}

int main(int argc, char *argv[])
{
		uint32_t pktSize = 512;
		std::string appDataRate = "100Mbps";
		std::string bottleNeckLinkBW="100Mbps";
		uint32_t duration  = 100;
		char linkDelay[10];

		CommandLine cmd;
		cmd.AddValue("bw","bandwidth of bottleneck link in Mbps",bottleNeckLinkBW);
		cmd.AddValue("dl", "delay of bottleneck link in ms", bottleNeckLinkDL);
		cmd.AddValue("duration", "simulation duration in seconds", duration);
		cmd.Parse(argc, argv);

		sprintf(linkDelay, "%dms", (int)bottleNeckLinkDL);

		Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(pktSize));
		Config::SetDefault("ns3::OnOffApplication::DataRate",   StringValue(appDataRate));
		Config::SetDefault("ns3::TcpL4Protocol::SocketType",    StringValue("ns3::TcpNewReno"));

		NS_LOG_UNCOND("--------------------------------------");
		NS_LOG_UNCOND("Simulation Enviroment:");
		NS_LOG_UNCOND(" - bottleneck link ("
						<< bottleNeckLinkBW<<","
						<<linkDelay<<")");
		NS_LOG_UNCOND(" - OnOffApplication ("
						<< pktSize << "byte,"
						<< appDataRate<<")");
		NS_LOG_UNCOND(" - Socket Option (TcpNewReno,"
						<< pktSize*2-42 << "bytes" << ")");
		NS_LOG_UNCOND(" - simulation time "
						<< SIM_START+duration << "s");
		NS_LOG_UNCOND("--------------------------------------");

		NodeContainer net;
		net.Create(2);

		PointToPointHelper p2p;
		p2p.SetDeviceAttribute("DataRate", StringValue(bottleNeckLinkBW));
		p2p.SetChannelAttribute("Delay", StringValue(linkDelay));
		p2p.SetQueue("ns3::DropTailQueue","MaxPackets", UintegerValue(bottleNeckLinkQS));

		NetDeviceContainer dev;
		dev = p2p.Install(net);

		InternetStackHelper stack;
		stack.InstallAll();

		Ipv4AddressHelper address;
		address.SetBase(NET_ADDR, NET_MASK, FIRST_NO);
		Ipv4InterfaceContainer ifs = address.Assign(dev);
		Ipv4GlobalRoutingHelper::PopulateRoutingTables();

		AddressValue remoteAddress(InetSocketAddress(ifs.GetAddress(1, 0), PORT));
		OnOffHelper ftp("ns3::TcpSocketFactory", Address());
		ftp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomValue[Constant=1]"));
		ftp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomValue[Constant=0]"));
		ftp.SetAttribute("Remote", StringValue("remoteAddress"));

		ApplicatinContainer src = ftp.Install(net.Get(0));
		src.Start(Seconds(SIM_START));
		src.Stop(Seconds(SIM_START+duration));

		Address sinkAddress(InetSocketAddress (IPv4Address::GetAny(), PORT));
		PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", sinkAddress);
		ApplicationContainer sink = sinkHelper.Install(net.Get(1));
		sink.Start(Seconds(SIM_START));
		sink.Stop(Seconds(SIM_START+duration));

		std::string fd = std::string(PROG_DIR) + "cwnd-" + bottleNeckLinkBW + "-" + linkDelay;
		AsciiTraceHelper ascii;
		Ptr<OutputStreamWrapper> st = ascii.CreateFileStream(fd);
		Simulator::Schedule(Seconds(0.1), &MyEventWrapper, net.Get(0)->GetApplication(0), st);

		Simulator::Stop(Seconds(SIM_START + duration));
		Simulator::Run();
		Simulator::Destroy();
		return 0;
}
