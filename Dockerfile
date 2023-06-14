FROM ros:foxy

# install ros package
RUN apt-get update && apt-get install -y \
      ros-${ROS_DISTRO}-demo-nodes-cpp \
      ros-${ROS_DISTRO}-demo-nodes-py && \
    rm -rf /var/lib/apt/lists/*

# installing bt.cpp librery and dipendencis
RUN sudo apt update
RUN sudo apt-get install -y libzmq3-dev
RUN sudo apt-get install -y libboost-dev
RUN sudo apt-get install -y cmake
RUN cd root ; mkdir BT_cpp
RUN mkdir BT_cpp
WORKDIR /root/BT_cpp/
RUN git clone -b v3.8 https://github.com/BehaviorTree/BehaviorTree.CPP.git
RUN cd BehaviorTree.CPP ; mkdir build ; cd build
WORKDIR /root/BT_cpp/BehaviorTree.CPP/build
RUN cmake .. ; make
EXPOSE 1667
EXPOSE 1666

# setting simple_bt for example
WORKDIR /root/BT_cpp/
RUN mkdir simple_bt ; cd simple_bt
WORKDIR /root/BT_cpp/simple_bt
COPY ./simple_bt/bt_first_attempt.cpp .
COPY ./simple_bt/bt_tree.xml .
COPY ./simple_bt/CMakeLists.txt .
RUN mkdir build ; cd build
WORKDIR /root/BT_cpp/
# RUN cd simple_bt/build ; cmake .. ; make
