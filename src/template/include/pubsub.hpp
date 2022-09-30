
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <sched.h>
#include <fstream>

#include "common.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;
using namespace std::chrono;
using namespace std::chrono_literals;
using namespace std;
class PubSub : public rclcpp::Node
{
  public:
    PubSub(string node_name, string sub_topic, string pub_topic)
    : Node(node_name), count_(0)
    {
      publisher_ = this->create_publisher<Payload>(pub_topic, 100);
      subscription_ = this->create_subscription<Payload>(
      sub_topic, 10, std::bind(&PubSub::topic_callback, this, _1));
    }

  private:
    void topic_callback(const Payload::SharedPtr msg)
    {

      auto message = Payload();

      message.time = msg->time;
      message.tltime = msg->tltime;
      message.pubcore = msg->pubcore;
      message.tlcore = msg->tlcore;
      message.subcore = msg->subcore;
      message.tlherz = msg->tlherz;

      for(int i =0; i < MAX_LEN; i++) {
	      if(message.tlcore[i] == -1) {
		      message.tlcore[i] = sched_getcpu();
		      auto now = duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch());
		      message.tltime[i] = now.count();
		      std::string fn = "/sys/devices/system/cpu/cpu";
		      fn += std::to_string(message.tlcore[i]);
		      fn += "/cpufreq/scaling_cur_freq";
		      std::ifstream fqf(fn);
		      std::string hz;
		      std::getline(fqf,hz);
		      message.tlherz[i] = std::stoi(hz);
		      break;
	      }
      }


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
