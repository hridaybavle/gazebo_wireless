
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/msgs/wireless_node.pb.h>
#include <gazebo/msgs/wireless_nodes.pb.h>
#include <gazebo/gazebo_client.hh>
#include <gazebo/msgs/propagation_grid.pb.h>
#include <iostream>
#include <sensor_msgs/ChannelFloat32.h>
#include <gazebo_wireless/Rss_level.h>
//#include <gazebo/gazebo_client.hh>

//gazebo::msgs::WirelessNodes NodesMsg;

float sig_level;
void receiver_subs(ConstWirelessNodesPtr &msg)
{
    std::string raw_msg = msg->DebugString();
    size_t index = raw_msg.find("signal_level");
    std::string sig_lev_str = raw_msg.substr(index+13, 15);
    std::cout << sig_lev_str << std::endl;
    sig_level = std::stof(sig_lev_str);
}

int main(int _argc, char **_argv)
{

  gazebo::client::setup(_argc, _argv);
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  
  node->Init ();
  // start transportation
  //gazebo::transport::run();

  // Subscribe to a topic
  gazebo::transport::SubscriberPtr sub_2 = node->Subscribe("/gazebo/default/iris_opt_flow/wirelessReceiver/link/wirelessReceiver/transceiver", receiver_subs);
  //publish on Gazebo
  //gazebo::transport::PublisherPtr sensor_pub = node->Advertise <gazebo::msgs::WirelessNodes>("/sensor/receiver", 1000, (1.0));
  //sensor_pub -> Publish(NodesMsg);
  //sensor_msgs::ChannelFloat32 Rec_msgs ;



  //publish on Ros

  ros::init(_argc, _argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<gazebo_wireless::Rss_level>("/receiver_rss", 1000);
  gazebo_wireless::Rss_level NodeMsg;
  NodeMsg.signal_level = sig_level;
  

  ros::Rate loop_rate(10);

  int count = 0;
    while (ros::ok())
   {
  chatter_pub.publish(NodeMsg);
  ros::spinOnce(); 
  loop_rate.sleep();
  ++count;
   }
 
  // shutting down
  while (true)
    gazebo::common::Time::MSleep(10);

  // Make sure to shut everything down.
   gazebo::client::shutdown();
}