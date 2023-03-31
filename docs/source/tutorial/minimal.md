# Minimal Dynamic Graph Manager Tutorial

This section of the documentation provides a minimal example of the usage of Dynamic Graph Manager (DGM).

:::{figure-md} minimal_dgm

<img src="../images/minimal_dgm.png" alt="minimal_dgm structure" class="bg-primary" width="800px">

Sturcture of the Minimal DGM Tutorial 
:::

As we can see from the above figure, when using DGM you would have four components:
- the "fake robot" in this tutorial takes the place of the actual robot when using `dgm_franka`;
- the `dg_minimal_tutorial` communicates between the robot and DGM by reading sensor data from the robot and setting contol commands to the robot;
- the shared memory is a communication pipeline between the Python API and the C++ based `dg_minimal_tutorial`;
- the Python API is where the controller is implemented.

## Installation

The source code of `dg_minimal_tutorial` can be found at this [link](https://github.com/BolunDai0216/dg_minimal_tutorial). To install the package, first clone the package and build it

```console
git clone https://github.com/BolunDai0216/dg_minimal_tutorial.git
cd dg_minimal_tutorial
mkdir build && cd build
cmake ..
cmake --build .
cmake --install . --prefix "<dir-to-dg_minimal_tutorial>/dg_minimal_tutorial/install"
```

Fill `<dir-to-dg_minimal_tutorial>` with the absolute path of the directory `dg_minimal_tutorial` is in. Then, in the root directory of `dg_minimal_tutorial` run the generated executable

```console
./install/lib/dg_minimal_tutorial/dg_minimal_tutorial_main
```

## Run Minimal Example

Add the location of `dynamic_graph_manager_cpp_bindings` to `PYTHONPATH`, if following the installation instruction of `dgm_franka` you should be able to add it to `PYTHONPATH` using the command

```console
export PYTHONPATH=$PYTHONPATH:~/dgm-ws/install/dynamic_graph_manager/lib/python3.8/site-packages
```

Open another terminal, and run the Python test file

```console
python3 python/test.py
```

you should see an output similar to

```console
Loading parameters from /home/mocap/Documents/dg_minimal_tutorial/config/dg_minimal_tutorial.yaml
Seting up cond_var
Value in shared memory is: [1. 2.]
```

in the Python console. If this works, then it indicates everything is working properly.