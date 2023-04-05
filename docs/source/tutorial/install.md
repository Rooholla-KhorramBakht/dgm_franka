# `dgm_franka` & `dgh_franka` installation

After following the installation of the prerequisites in the readme, we can now install `dgm_franka` and `dgh_franka`. 

## Install `dgm_franka`

First, clone the repo

```console
git clone https://github.com/BolunDai0216/dgm_franka
cd dgm_franka
```

Then, build the package, but before that we also need to source the `dgm-ws` we created before

```console
source ~/dgm-ws/install/setup.bash
mkdir build && cd build
cmake ..
cmake --build .
```

Now that the package is built, we can install it anywhere we want

```console
cmake --install . --prefix "<install-dir>/install"
```

## Install `dgh_franka`

Now that `dgm_franka` is installed, we also need to install the dynamic graph head (DGH) to use it from Python. First, we also need to clone the repo

```console
git clone https://github.com/BolunDai0216/dgh_franka.git
cd dgh_franka
```

Since this is a python package, we just need to install it

```console
python3 -m pip install -e .
```

The next step is to run the executable in the `install` folder of `dgm_franka` and then control the robot from the Python API.

