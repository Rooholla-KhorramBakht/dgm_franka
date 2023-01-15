FROM ros:foxy

RUN apt-get install -y ros-foxy-rosidl-generator-cpp libcereal-dev build-essential libboost-all-dev freeglut3 freeglut3-dev \
libedit-dev libtinyxml2-dev libyaml-cpp-dev ros-foxy-ament-cmake-nose python3-pip libglib2.0-dev libxmu-dev libpoco-dev libeigen3-dev python-all-dev


RUN pip3 install treep

RUN mkdir /root/dgm-ws && cd ~/dgm-ws && git clone https://github.com/Rooholla-KhorramBakht/treep_projects.git && treep --clone DYNAMIC_GRAPH_MANAGER && colcon build && source install/setup.bash
WORKDIR /root
RUN git clone --recursive https://github.com/frankaemika/libfranka --branch 0.10.0 && cd libfranka && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF .. && cmake --build . && cpack -G DEB && dpkg -i libfranka*.deb 

RUN cd /root && git clone https://github.com/Rooholla-KhorramBakht/dgm_franka.git && cd dgm_franka && mkdir build && cd build && cmake .. && make 

RUN pip3 install jupyterlab && mkdir /root/data

CMD cd /root/dgm-ws/install && source install.bash && jupyter notebook --no-browser --allow-root --ip='*' --NotebookApp.token='' --NotebookApp.password=''