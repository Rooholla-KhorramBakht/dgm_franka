# Dynamic Graph Manager Interface for Franka Emika Robot
This repository holds the implementation of dynamic graph manager interface for the Franka Emika 7-DoF robot arm.

## System Prepration and Prerequisits

The instruction here assumes a clean Ubuntu (20.04 is tested) installtion and installs required packages. First, install the common robotics packages using the useful scripts from the [machines in motion](http://wp.nyu.edu/machinesinmotion/) lab at NYU:

``` bash
git clone https://github.com/machines-in-motionubuntu_installation_scripts.git

sudo ./ubuntu_installation_scripts/official/setup_ubuntu install all
```

Above will install usefull robotics packages like ROS, Pinocchio and many more. 

In addition to the packages installed by the script from the previous step, we need to install the following prerequisits:

``` bash
sudo apt-get install libedit-dev
```

To interface with Franka arm, the libfranka is also assumed to be installed (follow through the instructions [here](https://frankaemika.github.io/docs/installation_linux.html)). 

## Compile and Install

First create the workspace directory and navigate into it:
```bash
mkdir ~/devel
cd ~/devel
```
We use [treep](https://pypi.org/project/treep/) to manage the repositories used for our projects. First, clone our treep configurations:

```bash
git clone git@github.com:Rooholla-KhorramBakht/treep_projects.git
```

Having the above repository in the working directory, we can use the treep command to clone the required repositories as follows:

```bash
treep --clone DGM_FRANKA
```
the command above creates a `workspace/src` directory tree. Having this, we can start the compilation process using colcon. But before that, source the ROS and openrobotics installation scrips:

```bash
source /opt/ros/foxy/setup.bash
source /opt/openrobotics/setup.bash
#Compile
colcon build
```

Finally, source the installtaion scrips:
```bash
source install/setup.bash
```
