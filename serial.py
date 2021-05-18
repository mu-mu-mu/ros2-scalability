import sys
import os
import shutil

template_dir = "src/template"

if len(sys.argv) != 3:
    print("serial.py project_name #nodes ")
    sys.exit(1)

project = sys.argv[1]
project_dir = "src/" + project
nodes_num = int(sys.argv[2])


# Make directories
try:
    os.mkdir(project_dir)
except:
    print("Error: Couldn't make the project directory")
    sys.exit(1)

os.mkdir(project_dir+"/src")
os.mkdir(project_dir+"/launch")
os.mkdir(project_dir+"/include")

# Copy unmodified files
def copy_file(fname):
    shutil.copyfile(template_dir + "/" + fname, project_dir + "/" + fname)

copy_file("include/pubsub.h")
copy_file("src/pub.cpp")
copy_file("src/sub.cpp")


# CMakeLists.txt
cmake_exec = """add_executable(tl{0} src/pubsub{0}.cpp)
ament_target_dependencies(tl{0} rclcpp std_msgs)
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
            elif "template" in line:
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
        executable='tl{1}',
        output='screen',
    ))
"""
with open(template_dir + "/launch/launch.py","r") as t:
    with open(project_dir + "/launch/launch.py","w") as p:
        for line in t.readlines():
            if "HERE" in line:
                for i in range(nodes_num):
                    p.write(launch_tl.format(project,str(i)))
            elif "template" in line:
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
            if "template" in line:
                p.write(package_name.format(project))
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

