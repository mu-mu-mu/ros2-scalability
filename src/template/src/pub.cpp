#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

#include "common.hpp"

using namespace std::chrono;
using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("pub"), count_(0)
    {
      publisher_ = this->create_publisher<Payload>("topic_start", 100);
      timer_ = this->create_wall_timer(
      200ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = Payload();

      auto now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
      message.time = now.count();

      //message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Published");
      publisher_->publish(message);
      if (count_ >= MAX_COUNT) {
        rclcpp::shutdown();
      }
      count_++;
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<Payload>::SharedPtr publisher_;
    int count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
