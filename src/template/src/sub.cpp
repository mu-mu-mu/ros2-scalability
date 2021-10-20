#include <memory>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "common.hpp"
using std::placeholders::_1;

using namespace std::chrono;
using namespace std::chrono_literals;

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("sub"), _count(0)
    {
      rclcpp::QoS qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();

      this->declare_parameter("qos");
      std::string rel = this->get_parameter("qos").as_string();
      if(rel == "best_effort")
        qos = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

      subscription_ = this->create_subscription<Payload>(
      "topic_end", qos, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg) const
    {
      auto now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
      auto delta = now.count() - msg->time;

      RCLCPP_INFO(this->get_logger(), "Subscribed %d", delta);

      if (_count >= MAX_COUNT) {
        rclcpp::shutdown();
      }
    }
    rclcpp::Subscription<Payload>::SharedPtr subscription_;
    int _count;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
