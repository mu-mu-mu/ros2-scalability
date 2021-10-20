import sys
import os
import shutil

template_dir = "src/template"

if len(sys.argv) != 4:
    print("serial.py project_name #nodes <msg size(bytes,1K,1M)>")
    sys.exit(1)

project = sys.argv[1]
project_dir = "src/" + project
nodes_num = int(sys.argv[2])
if sys.argv[3] in ["1M", "1m", "1mb", "1MB"]:
    msg_size = 1024 * 1024
elif sys.argv[3] in ["1K", "1k", "1kb", "1KB"]:
    msg_size = 1024
else:
    msg_size = int(sys.argv[3])


# Make directories
try:
    os.mkdir(project_dir)
except:
    print("Error: Couldn't make the project directory")
    sys.exit(1)

os.mkdir(project_dir+"/src")
os.mkdir(project_dir+"/msg")
os.mkdir(project_dir+"/launch")
os.mkdir(project_dir+"/include")

# Copy unmodified files
def copy_file(fname):
    shutil.copyfile(template_dir + "/" + fname, project_dir + "/" + fname)

copy_file("include/pubsub.hpp")
copy_file("src/pub.cpp")
copy_file("src/sub.cpp")

# Payload.msg
with open(template_dir + "/msg/Payload.msg","r") as t:
    with open(project_dir + "/msg/Payload.msg","w") as p:
        for line in t.readlines():
            p.write(line)
        p.write("char[{}] chunk".format(msg_size))

# CMakeLists.txt
cmake_exec = """add_executable(tl{0} src/pubsub{0}.cpp)
ament_target_dependencies(tl{0} rclcpp std_msgs)
rosidl_target_interfaces(tl{0}
  ${{PROJECT_NAME}} "rosidl_typesupport_cpp")

"""
cmake_dep = """  tl{0}
"""
with open(template_dir + "/CMakeLists.txt","r") as t:
    with open(project_dir + "/CMakeLists.txt","w") as p:
        for line in t.readlines():
            if "EXEC HERE" in line:
                for i in range(nodes_num):
                    p.write(cmake_exec.format(str(i)))
            elif "DEP HERE" in line:
                for i in range(nodes_num):
                    p.write(cmake_dep.format(str(i)))
            elif "tmpl" in line:
                p.write("project({0})".format(project))
            else:
                p.write(line)

# launch
launch_package = """
        package='{0}',
"""
launch_tl = """
    tl.append(launch_ros.actions.Node(
        package='{0}',
        node_executable='tl{1}',
        output='screen',
    ))
"""
with open(template_dir + "/launch/launch.py","r") as t:
    with open(project_dir + "/launch/launch.py","w") as p:
        for line in t.readlines():
            if "HERE" in line:
                for i in range(nodes_num):
                    p.write(launch_tl.format(project,str(i)))
            elif "tmpl" in line:
                p.write(launch_package.format(project))
            else:
                p.write(line)

# package
package_name = """
  <name>{0}</name>
"""
with open(template_dir + "/package.xml","r") as t:
    with open(project_dir + "/package.xml","w") as p:
        for line in t.readlines():
            if "tmpl" in line:
                p.write(package_name.format(project))
            else:
                p.write(line)

# common.h
common_include = """
#include "{0}/msg/payload.hpp"
"""

common_namespace = """
using namespace {0}::msg;
"""
with open(template_dir + "/include/common.hpp","r") as t:
    with open(project_dir + "/include/common.hpp","w") as p:
        for line in t.readlines():
            if r"tmpl/msg" in line:
                p.write(common_include.format(project))
            elif r"tmpl::msg" in line:
                p.write(common_namespace.format(project))
            else:
                p.write(line)

# pubsub.cpp
pubsub_spin = 'rclcpp::spin(std::make_shared<PubSub>("pubsub{0}", "topic{1}", "topic{2}"));\n'

with open(template_dir + "/src/pubsub.cpp","r") as t:
    lines = t.readlines()
    for i in range(nodes_num):
        with open(project_dir + "/src/pubsub{}.cpp".format(str(i)),"w") as p:
            for line in lines:
                if "HERE" in line:
                    node_name = str(i)
                    subtopic = str(i) if i != 0 else "_start"
                    pubtopic = str(i+1) if i != nodes_num - 1 else "_end"
                    p.write(pubsub_spin.format(node_name,subtopic,pubtopic))
                else:
                    p.write(line)

