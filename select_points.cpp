#include "select_points.h"
#include "behaviortree_cpp/bt_factory.h"

using namespace BT;

static const char* xml_text = R"(
<root BTCPP_format="4">
  <BehaviorTree ID="MainTree">
    <Repeat num_cycles="3">
      <Sequence>
        <Fallback>
          <isPointSelected/>
          <SelectPoints />
        </Fallback>
        <LoopPoints />
        <SelectPoints />
      </Sequence>
    </Repeat>
  </BehaviorTree>
</root>
)";

NodeStatus SelectPoints::isPointSelected()
{
  if (_selected)
  {
    std::cout << "Points are selected." << std::endl;
    return NodeStatus::SUCCESS;
  }
  else
  {
    std::cout << "Points are not selected." << std::endl;
    return NodeStatus::FAILURE;
  }
}

NodeStatus SelectPoints::selectPoints()
{
    for (int i = 0; i < 10; i++)
    {
      _points->push_back(Point{double(i), double(i+1), double(i+2)});
    }
    _selected = true;
    return NodeStatus::SUCCESS;
}

NodeStatus SelectPoints::loopPoints()
{
    for (Point point : *_points)
    {
        std::cout << "Point: " << point.x << ", " << point.y << ", " << point.z << std::endl;
    }
    _selected = false;
    return NodeStatus::SUCCESS;
}

void SelectPoints::registerNodes(BT::BehaviorTreeFactory& factory)
{
    factory.registerSimpleCondition("isPointSelected", std::bind(&SelectPoints::isPointSelected, this));
    factory.registerSimpleAction("SelectPoints", std::bind(&SelectPoints::selectPoints, this));
    factory.registerSimpleAction("LoopPoints", std::bind(&SelectPoints::loopPoints, this));
}


int main()
{
  BT::BehaviorTreeFactory factory;

  SelectPoints select_points;
  select_points.registerNodes(factory);

  // In this example a single XML contains multiple <BehaviorTree>
  // To determine which one is the "main one", we should first register
  // the XML and then allocate a specific tree, using its ID

  factory.registerBehaviorTreeFromText(xml_text);
  auto tree = factory.createTree("MainTree");

  // helper function to print the tree
  // BT::printTreeRecursively(tree.rootNode());

  // Tick multiple times, until either FAILURE of SUCCESS is returned
  tree.tickWhileRunning();

  return 0;
}

