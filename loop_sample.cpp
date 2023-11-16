#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/decorators/loop_node.h"
#include <list>

using namespace BT;

static const char* xml_tree = R"(
<root BTCPP_format="4">
  <BehaviorTree ID="Untitled">
    <Repeat num_cycles="3">
      <Sequence>
        <Fallback>
          <isPointSelected/>
          <SelectPoints points="{points}" selected="{selected}"/>
        </Fallback>
        <LoopPoints if_empty="SUCCESS"
                    queue="{points}"
                    value="{point}">
          <TellMeWhichPoint point="{point}"/>
        </LoopPoints>
        <SaySomething message="I am done."/>
      </Sequence>
    </Repeat>
  </BehaviorTree>
</root>
)";

struct Point
{
    double x, y, z;
};

namespace BT
{
    template<>
    Point convertFromString(StringView key)
    {
        auto parts = splitString(key, ';');
        if (parts.size() != 3)
        {
            throw RuntimeError("invalid input");
        }
        else
        {
            Point output;
            output.x = convertFromString<double>(parts[0]);
            output.y = convertFromString<double>(parts[0]);
            output.z = convertFromString<double>(parts[0]);
            return output;
        }
    }
}

class SelectPoints : public SyncActionNode
{
    public:
    SelectPoints(const std::string& name, const NodeConfiguration& config)
        : SyncActionNode(name, config)
    {}

    NodeStatus tick() override
    {
        auto shared_queue = std::make_shared<std::deque<Point>>();
        for (int i = 0; i < 10; i++)
        {
            shared_queue->push_back(Point{double(i), double(i+1), double(i+2)});
            // Point p = (*shared_queue)[i];
            // std::cout << "Point: " << p.x << ", " << p.y << ", " << p.z << std::endl;
        }
        setOutput("points", shared_queue);
        setOutput("selected", true);
        std::cout << "Selected points." << std::endl;
        return NodeStatus::SUCCESS;
    }

    static PortsList providedPorts()
    {
        return {OutputPort<SharedQueue<Point>>("points"),
                OutputPort<bool>("selected")};
    }
};

class SaySomething : public SyncActionNode
{
    public:
    SaySomething(const std::string& name, const NodeConfiguration& config)
        : SyncActionNode(name, config)
    {}

    NodeStatus tick() override
    {
        std::string msg;
        getInput("message", msg);
        std::cout << msg << std::endl;
        return NodeStatus::SUCCESS;
    }
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<std::string>("message")};
    }
};

class TellMeWhichPoint : public SyncActionNode
{
    public:
    TellMeWhichPoint(const std::string& name, const NodeConfiguration& config)
        : SyncActionNode(name, config)
    {}

    NodeStatus tick() override
    {
        Point p;
        getInput("point", p);
        std::cout << "Point: " << p.x << ", " << p.y << ", " << p.z << std::endl;
        return NodeStatus::SUCCESS;
    }
    static BT::PortsList providedPorts()
    {
        return {BT::InputPort<Point>("point")};
    }
};


// NodeStatus isPointSelected(TreeNode& self)
// {
//     auto p = std::make_shared<std::deque<Point>>();
//     auto res = self.getInput<bool>("selected");
//     if (!res)
//     {
//         std::cout << "Points are not selected." << std::endl;
//         return NodeStatus::FAILURE;
//     }
//     std::cout << "Points are selected." << std::endl;
//     return NodeStatus::SUCCESS;
// }

int main()
{
    BehaviorTreeFactory factory;
    // factory.registerNodeType<LoopNode<Point>>("LoopPoints");
    // factory.registerSimpleCondition("isPointSelected", std::bind(isPointSelected, std::placeholders::_1));
    factory.registerSimpleCondition("isPointSelected",  [](BT::TreeNode& self) {
        // auto p = std::make_shared<std::deque<Point>>();
        auto res = self.getInput<bool>("selected");

        if (!res)
        {
            std::cout << "Points are not selected." << std::endl;
            return NodeStatus::FAILURE;
        }
        std::cout << "Points are selected." << std::endl;
        return NodeStatus::SUCCESS;
    });
    factory.registerNodeType<LoopNode<Point>>("LoopPoints");
    factory.registerNodeType<SelectPoints>("SelectPoints");
    factory.registerNodeType<TellMeWhichPoint>("TellMeWhichPoint");
    factory.registerNodeType<SaySomething>("SaySomething");

    auto tree = factory.createTreeFromText(xml_tree);
    tree.tickWhileRunning();

    return 0;
}
