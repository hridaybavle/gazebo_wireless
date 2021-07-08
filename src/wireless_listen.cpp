
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/msgs/wireless_node.pb.h>
#include <gazebo/msgs/wireless_nodes.pb.h>
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/propagation_grid.pb.h>
#include <iostream>

//#include <gazebo/gazebo_client.hh>

void trans_subs(ConstPropagationGridPtr &msg)
{
  std::cout << msg->DebugString() << std::endl;
}

void receiver_subs(ConstWirelessNodesPtr &msg)
{
    std::cout << msg->DebugString() << std::endl;
}

int main(int _argc, char **_argv)
{

  gazebo::client::setup(_argc, _argv);
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  
  node->Init ();
  // start transportation
  //gazebo::transport::run();

  // Subscribe to a topic
  gazebo::transport::SubscriberPtr sub_1 = node->Subscribe("/gazebo/default/wirelessTransmitter/link/wirelessTransmitter/transceiver", trans_subs);
  gazebo::transport::SubscriberPtr sub_2 = node->Subscribe("/gazebo/default/wirelessTransmitter/link/wirelessTransmitter/transceiver", receiver_subs);

 
  // shutting down
  while (true)
    gazebo::common::Time::MSleep(10);

  // Make sure to shut everything down.
   gazebo::client::shutdown();
}