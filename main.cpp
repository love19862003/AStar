#include <set>
#include <random>
#include <array>
#include <iostream>
#include "AStar.h"
#include "MapTemplate.h"
using namespace  AStarSpace;

typedef unsigned int uint32;
typedef std::pair<uint32, uint32> keys;

struct Point{
  std::array<float, 3> pos;   //位置
  std::list<keys> link;       //可以到达的点
};


typedef  ShareSpace::Utility::ObjectMap<keys, Point> Map;


uint32 grandom(uint32 min, uint32 max){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32> dis(min, max - 1);
  return dis(rd);
}

void testAstar(){
    Map points;
    const uint32 MaxSizeX = 20;
    const uint32 MaxSizeY = 26;

    //增加路点邻居数据
    auto addlink = [MaxSizeX, MaxSizeY](Point& p, const keys& l){
      if (l.first >= 0 && l.first < MaxSizeX && l.second >= 0 && l.second < MaxSizeY){
        p.link.push_back(l);
      }
    };

    //生成一个大的路点图
    for(uint32 i = 0; i < MaxSizeX; ++i){
      for(uint32 j = 0; j < MaxSizeY; ++j){
        Point p;
        p.pos = {static_cast<float>(i), static_cast<float>(j), 1.f};
        addlink(p, std::make_pair(i, j-1));
        addlink(p, std::make_pair(i, j+1));
        addlink(p, std::make_pair(i+1,j));
        addlink(p, std::make_pair(i+1,j+1));
        addlink(p, std::make_pair(i+1,j-1));
        addlink(p, std::make_pair(i-1, j+1));
        addlink(p, std::make_pair(i-1, j));
        addlink(p, std::make_pair(i-1, j-1));
        points.addData(std::make_pair(i, j), p);
      }
    }

    typedef AStart<keys, uint32> AStarType;
    //估值函数
    auto h = [&points](const Map::key_type& s, const Map::key_type& e){
      auto pl = points.getData(s);
      auto pr = points.getData(e);
      float x = pl.pos[0] - pr.pos[0];
      float y = pl.pos[1] - pr.pos[1];
      float z = pl.pos[2] - pr.pos[2];
      return static_cast<uint32>(std::sqrtl(x*x + y*y + z*z) * 100);
    };

    //消耗函数 暂时和估值函数一样，可以自定义
    auto g = h;

    //判断是不是同一个路点
    auto e = [](const Map::key_type& l, const Map::key_type& r){
      return l.first == r.first && l.second == r.second;
    };

    //获取路点相通的路点列表
    auto links = [&points](const Map::key_type& l){
      auto p = points.getData(l);
      return p.link;
    };


    AStarType object(h, g, e, links);

    //输出寻路结果
    auto getPath = [&object, MaxSizeX, MaxSizeY](const keys& s, const keys& e){
      auto path = object.findPath(s, e);
      std::set<keys> PathSet;
      for(auto& d : path){
        PathSet.insert(d);
      }
      for(uint32 i = 0; i < MaxSizeX; ++i){
        std::string str = "";
        for(uint32 j = 0; j < MaxSizeY; ++j){ str += (PathSet.count(std::make_pair(i, j)) > 0 ? "#" :" ");}
        std::cout << str.c_str()  << std::endl;
      }
      std::cout << "------------------------------------------------------------"<< std::endl;
    };


    //测试寻路
    for (uint32 i = 0;i < 5; ++i){
      keys s = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;
      keys e = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;
      std::cout << "[" << s.first << " " << s.second << "] " << "[" << e.first <<  " "  << e.second << "]" << std::endl;
      getPath(s, e) ;
    }


  }
int main(int argc, char* argv[]){
  testAstar();  
  return 0;
}