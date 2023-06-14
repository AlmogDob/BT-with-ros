#include <iostream> 
#include <chrono>
#include "behaviortree_cpp/action_node.h"
#include "behaviortree_cpp/bt_factory.h"

#ifdef ZMQ_FOUND
#include "behaviortree_cpp_v3/loggers/bt_zmq_publisher.h"
#endif

using namespace std::chrono_literals;

//Node class
class ApproachObject : public BT::SyncActionNode
{
public:
    explicit ApproachObject(const std::string &name) : BT::SyncActionNode(name, {})
    {
    }

    BT::NodeStatus tick() override
    {
        std::cout << "Approach Object: " << this->name() << std::endl;
        std::this_thread::sleep_for(5s);
        return BT::NodeStatus::SUCCESS;
    }
};
//Function
BT::NodeStatus CheckBattery()
{
    std::cout << "Battery OK" << std::endl;
    return BT::NodeStatus::SUCCESS;
}

//Custom class method
class GripperInterface
{
public:
    GripperInterface() : _open(true)
    {

    }

    BT::NodeStatus open()
    {
        _open = true;
        std::cout << "Gripper open" << std::endl;
        return BT::NodeStatus::SUCCESS;  
    }

    BT::NodeStatus close()
    {
        _open = false;
        std::cout << "Gripper close" << std::endl;
        return BT::NodeStatus::SUCCESS;          
    }

private:
    bool _open;
};

int main(int argc, char** argv)
{
    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<ApproachObject>("ApproachObject");

    factory.registerSimpleCondition("CheckBattery", std::bind(CheckBattery)); //a method to redister a simple condition node

    GripperInterface gripper;
    factory.registerSimpleAction(
        "OpenGripper",
        std::bind(&GripperInterface::open, &gripper));

    factory.registerSimpleAction(
        "CloseGripper",
        std::bind(&GripperInterface::close, &gripper));

    // Creat Tree
    auto tree = factory.createTreeFromFile("./../bt_tree.xml");
    
    #ifdef ZMQ_FOUND
    // This logger publish status changes using ZeroMQ. Used by Groot
        PublisherZMQ publisher_zmq(tree);
    #endif
    // execute the tree
    printTreeRecursively(tree.rootNode());
    do 
    {
        tree.tickWhileRunning();
    }
    while (true);

    return 0;
}