#pragma once

#include "behaviortree_cpp/bt_factory.h"


struct Point
{
  double x, y, z;
};

class SelectPoints
{
  public:
    void registerNodes(BT::BehaviorTreeFactory& factory);

    void reset();

    // SUCCESS if _points is not empty
    BT::NodeStatus isPointSelected();

    BT::NodeStatus selectPoints();
    BT::NodeStatus loopPoints();
    // BT::NodeStatus tellMeWhichPoint();

  private:
    std::shared_ptr<std::deque<Point>> _points = std::make_shared<std::deque<Point>>();
    bool _selected = false;
};