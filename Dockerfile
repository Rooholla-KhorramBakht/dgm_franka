FROM robocaster/dockerobot-control:amd64
SHELL ["/bin/bash", "-c"]

RUN apt-get update && apt-get install -y ros-foxy-rosidl-generator-cpp libcereal-dev build-essential libboost-all-dev freeglut3 freeglut3-dev \
libedit-dev libtinyxml2-dev libyaml-cpp-dev ros-foxy-ament-cmake-nose python3-pip libglib2.0-dev libxmu-dev libpoco-dev libeigen3-dev python-all-dev\
 curl unzip 

# Compile and Install LCM
#RUN cd /root/ && git clone https://github.com/lcm-proj/lcm.git && cd lcm && mkdir build && cd build && cmake .. && make -j"$(nproc)" && make install 

# Compile and Install Dynamic Graph Manager
RUN python3 -m pip install treep lark catkin_pkg nose==1.3.7 empy

RUN  cd /root && source /opt/ros/foxy/setup.bash && mkdir /root/dgm-ws && cd ~/dgm-ws && git clone https://github.com/Rooholla-KhorramBakht/treep_projects.git && treep --clone DYNAMIC_GRAPH_MANAGER && colcon build && source install/setup.bash
RUN echo 'source /root/dgm-ws/install/setup.bash' >> ~/.bashrc

# Compile and install libfranka
WORKDIR /root
RUN git clone --recursive https://github.com/frankaemika/libfranka --branch 0.10.0 && cd libfranka && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF .. && cmake --build . && cpack -G DEB && dpkg -i libfranka*.deb 

# Compile and install franka DGM project
RUN mkdir /root/data && cd /root/data && source /opt/ros/foxy/setup.bash && source /root/dgm-ws/install/setup.bash && cd /root && git clone https://github.com/Rooholla-KhorramBakht/dgm_franka.git && cd dgm_franka && mkdir build && cd build && cmake .. && make -j"$(nproc)"

# # Start the jupyter notebook 
# CMD source /opt/ros/foxy/setup.bash && cd /root && source dgm-ws/install/setup.bash && export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python3/dist-packages/ && jupyter notebook -y --no-browser --allow-root --ip='*' --NotebookApp.token='' --NotebookApp.password=''

# Install and start a VSCode server
RUN cd /opt/ && wget https://github.com/coder/code-server/releases/download/v4.9.1/code-server_4.9.1_amd64.deb
RUN dpkg -i /opt/code-server_4.9.1_amd64.deb && rm /opt/code-server_4.9.1_amd64.deb

CMD code-server  --auth none /root