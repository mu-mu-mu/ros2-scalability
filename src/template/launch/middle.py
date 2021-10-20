import launch
import launch_ros.actions
from launch.actions import DeclareLaunchArgument
from launch.substitutions import TextSubstitution
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    qos_arg = DeclareLaunchArgument(
        "qos", default_value=TextSubstitution(text="reliable")
    )

    tl = []
    # HERE

    # tl.append(launch_ros.actions.Node(
    #     package='cpp_pubsub',
    #     executable='tl',
    #     output='screen',
    #     parameters=[{ "qos": LaunchConfiguration("qos") }],
    # ))

    return launch.LaunchDescription([
        qos_arg,
        *tl,
    ])
