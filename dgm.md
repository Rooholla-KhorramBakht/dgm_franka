# Jetson Installation Procedure

Frist install the following packages:

- ROS foxy 

Furthermore, install the prerequisit packages as follows:
```bash
sudo apt-get install ros-foxy-rosidl-generator-cpp libcereal-dev build-essential freeglut3 freeglut3-dev libedit-dev libtinyxml2-dev
# Source the ros installation
source /opt/ros/foxy/setup.bash 
```

We assume that a virtual envrionement with python3 as the interperter has been created and activated during the build process. A simple and lieght-weight solution for this is to use the virtualenvwrapper as follows:


```bash
pip3 install virtualenvwrapper
export WORKON_HOME=~/Envs
mkdir -p $WORKON_HOME
source ~/.local/bin/virtualenvwrapper.sh
```

Then, create a viruatl enviroment with python3 as the interpreter:

```bash
mkvirtualenv -p python3 dgm_env
workon dgm_env 
```

Install the following packages within the virtual envrionment:

```bash
pip install lark catkin_pkg nose==1.3.7 numpy
# A fix for ROS
pip uninstall em
pip install empy 
```
Now, use treep to clone the related repositories to the dynamic-graph-manager project:

```bash
mkdir ~/dgm-ws && cd ~/dgm-ws
git clone git@github.com:Rooholla-KhorramBakht/treep_projects.git
treep --clone DYNAMIC_GRAPH_MANAGER
```

The last command above creates a `workspace/src` directory and clones the related repositories to the `DYNAMIC_GRAPH_MANAGER` inside of it. 

Finally, run the compile command and source the installation script as follows:

```
colcon build
source install/setup.bash
```

