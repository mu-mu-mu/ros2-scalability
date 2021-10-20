import launch
import launch_ros.actions

def generate_launch_description():
    talker = launch_ros.actions.Node(
        package='tmpl',
        node_executable='talker',
        output='screen',
    )
    listener = launch_ros.actions.Node(
        package='tmpl',
        node_executable='listener',
        output='screen',
    )

    return launch.LaunchDescription([
        talker,
        listener
    ])
