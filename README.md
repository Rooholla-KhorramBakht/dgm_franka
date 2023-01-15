# Dynamic Graph Manager for Franka Emika

![DGM_Franka](doc/DGMFranka.drawio.png)
This repository holds the implementation of the dynamic graph manager interface for the Franka Emika 7-DoF robot arm.

## What Is This?

Dynamic graph manager is a package that provides a simple interface between the drivers of a robot and a controller that is defined based on the LAAS-CNRS dynamic graph framework. As the name implies, a dynamic graph is a directed graph with nodes representing mathematical computations and input/output of the system. 

As shown below, the dynamic graph provides a powerful and conceptually easy-to-understand language to present robot controllers where the inputs are generated by the robot sensors and user commands, and the outputs are sent to the actuators. 

![DG_Conecept](doc/Dynamic-Graph.drawio.png)

The dynamic graph is built around a shared memory and several computation and data management threads that interchange information between each other in a harmony defined by a network of interprocess synchronization mechanisms (showed by dashed paths in [Fig1](#DGM_Franka)) Dynamic Graph manager may be used in two different ways:

- Defining computation nodes in C++ and hooking them up together (either through python or C++ API) to define a graph that represents the control system of the robot. 

- Directly reading the sensor values from and writing the control commands to the shared memory through a third-party process in python or C++. 



This repository is meant to be used based on the second approach. Specifically, the real-time mechanisms built into DGM handle the timely transfer of data between the shared memory and the robot while the controller algorithm in python reads and writes to this memory (as an intermediate space) without worrying about meeting the tight hardware timing constraints imposed by the robot interface. As such, the DGM in this scenario acts as a rate adaptor and safety controller for high-level controllers defined in Python where new ideas can be tried out fast and easily. 

As opposed to the original dynamic graph manager project, this repository uses a modified version that produces trigger signals to the python algorithm when new sensor measurements are available. This communication is implemented through Light-Weight Communication and Marshalling (LCM) library as a low-latency interprocess communication mechanism. Furthermore, LCM is also used to trigger the update of the shared memory for robot interfaces that work by calling a user-defined controller callback. For the Franka Robot, 


## Installation

### Prerequisites

First, install the ROS foxy as explained [here](https://docs.ros.org/en/foxy/Installation.html). Then, install the following prerequisite packages:

```bash
sudo apt-get install ros-foxy-rosidl-generator-cpp libcereal-dev build-essential libboost-all-dev\
freeglut3 freeglut3-dev libedit-dev libtinyxml2-dev libyaml-cpp-dev ros-foxy-ament-cmake-nose
# Source the ros installation
source /opt/ros/foxy/setup.bash 
```

We assume that a virtual environment with python3 as the interpreter has been created and activated. A simple and quick way is to use the virtualenvwrapper. First, install it as follows:

```bash
pip3 install virtualenvwrapper
export WORKON_HOME=~/Envs
mkdir -p $WORKON_HOME
source ~/.local/bin/virtualenvwrapper.sh
```
Then, create and activate an environment with python3 as the default interpreter:

```bash
mkvirtualenv -p python3 dgm_env
workon dgm_env 
```

Several dependencies need to be installed within the newly generated environment. Install them as follows:

```bash
pip install lark catkin_pkg nose==1.3.7 numpy
# A fix for ROS
pip uninstall em
pip install empy 
```

Finally, install the libfranka library and preempt-rt Linux kernel as instructed [here](https://frankaemika.github.io/docs/installation_linux.html). 

### Compile and Install

We use [treep](https://pypi.org/project/treep/) to manage the repositories used for our projects. First, clone our treep configurations:

```bash
mkdir ~/dgm-ws && cd ~/dgm-ws
git clone git@github.com:Rooholla-KhorramBakht/treep_projects.git
treep --clone DYNAMIC_GRAPH_MANAGER
```

The last command above creates a `workspace/src` directory and clones the related repositories. Finally, run the compile command and source the installation script as follows:

```
colcon build
source install/setup.bash
```
## How to Use It?

This section addresses the use case where a python node directly reads and writes the shared memory. This is done using classes defined in `notebooks/<dynamic/kinematic>_interface.ipynb` notebooks. 

The interface classes defined in these notebooks, take the DGM YAML configuration file and tap into the created shared memory. It then registers an LCM callback that gets called on the reception of the trigger signals from DGM. Each time a new measurement is available, DGM sends this signal and triggers the computation of one control iteration. 

In addition to running the control algorithm, this class can also record the input/output data into a CSV file. Furthermore, an online plot of the data has been made possible by sending ZMQ messages to [Plotjugger](https://www.plotjuggler.io/) which is an open-source data plotting and recording tool. 

Before running the notebooks, start the dynamic graph manager for the dynamic or kinematic interface as follows:

```bash
# To be added
```

Then, follow the instructions provided in the notebooks and try out the demo controllers.
