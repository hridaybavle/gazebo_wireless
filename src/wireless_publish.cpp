
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

class rss_data_handler 
{

public:
  rss_data_handler() {}
  ~rss_data_handler() {}

private:
  ros::Publisher rss_pub;
  gazebo::transport::SubscriberPtr gazebo_sub;

public:
  void open(gazebo::transport::NodePtr node, ros::NodeHandle n);
  void receiver_subs(ConstWirelessNodesPtr &msg);
  void publish_rss_data(float a1, float a2, float a3);

};


int nthSubstr(int n, const std::string& s,
              const std::string& p) {
              size_t i = s.find(p);     // Find the first occurrence

   int j;
   for (j = 1; j < n && i != std::string::npos; ++j)
      i = s.find(p, i+1); // Find the next occurrence

   if (j == n)
     return(i);
   else
     return(-1);
}


void rss_data_handler::publish_rss_data(float a1, float a2, float a3){

  gazebo_wireless::Rss_level NodeMsg;
  NodeMsg.signal_level1 = a1;
  NodeMsg.signal_level2 = a2;
  NodeMsg.signal_level3 = a3;
  rss_pub.publish(NodeMsg);
}

void rss_data_handler::receiver_subs(ConstWirelessNodesPtr &msg)
{  
    std::string raw_msg = msg->DebugString();
    size_t occur1 = raw_msg.find("signal_level:");
    size_t occur2 = raw_msg.find("signal_level:", occur1+1);
    size_t occur3 = raw_msg.find("signal_level:", occur2+1);
    //size_t occur1 = 43; size_t occur2 = 121; size_t occur3 = 201;  
    std::string sig_lev_str1 = raw_msg.substr(occur1+13, 15);
    std::string sig_lev_str2 = raw_msg.substr(occur2+13, 15);
    std::string sig_lev_str3 = raw_msg.substr(occur3+13, 15);
    

    float sig_level1;
    float sig_level2;
    float sig_level3;

    sig_level1 = std::stof(sig_lev_str1);
    sig_level2 = std::stof(sig_lev_str2);
    if (occur3 <1000){
      sig_level3 = std::stof(sig_lev_str3);
    } else{
       sig_level3 = 0.0; 
    }
    
    std::cout << sig_lev_str1 <<"\n"<<sig_lev_str2 <<"\n"<<sig_lev_str3 <<"\n-----------------\n" <<std::endl;
    this->publish_rss_data(sig_level1, sig_level2, sig_level3);

}

void rss_data_handler::open(gazebo::transport::NodePtr node, ros::NodeHandle n) {

  // Subscribe to a topic
  gazebo_sub = node->Subscribe("/gazebo/default/iris_opt_flow/wirelessReceiver/link/wirelessReceiver/transceiver", 
                                                      &rss_data_handler::receiver_subs, this);
  rss_pub = n.advertise<gazebo_wireless::Rss_level>("/receiver_rss", 300);
}


int main(int _argc, char **_argv)
{
  // gazebo setup
  gazebo::client::setup(_argc, _argv);
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  //ros setup
  ros::init(_argc, _argv, "rss_handler");
  ros::NodeHandle n;

  rss_data_handler rss_data_handle_obj;
  rss_data_handle_obj.open(node, n);

  // shutting down
  while (ros::ok()){
    gazebo::common::Time::MSleep(10);
  }
  // Make sure to shut everything down.
  ros::shutdown();
  gazebo::client::shutdown();

}


// d0 = -25 db alpha = 6