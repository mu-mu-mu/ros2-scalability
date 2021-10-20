
#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "common.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;
using namespace std;
class PubSub : public rclcpp::Node
{
  public:
    PubSub(string node_name, string sub_topic, string pub_topic)
    : Node(node_name), count_(0)
    {
      rclcpp::QoS qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();

      this->declare_parameter("qos");
      std::string rel = this->get_parameter("qos").as_string();
      if(rel == "best_effort")
        qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

      publisher_ = this->create_publisher<Payload>(pub_topic, qos);
      subscription_ = this->create_subscription<Payload>(
      sub_topic, qos, std::bind(&PubSub::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg)
    {
      auto message = Payload();
      message.time = msg->time;
      publisher_->publish(message);
      if (count_ >= MAX_COUNT) {
        rclcpp::shutdown();
      }
      count_++;
    }

    rclcpp::Subscription<Payload>::SharedPtr subscription_;
    rclcpp::Publisher<Payload>::SharedPtr publisher_;
    int count_;
};
