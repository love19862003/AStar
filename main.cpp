#include <set>
#include <random>
#include <array>
#include <iostream>
#include <string>
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

void testAstar(uint32 MaxSizeX,  uint32 MaxSizeY, uint32 testCount){
    Map points;
    //uint32 MaxSizeX = 20;
    //uint32 MaxSizeY = 26;

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
      auto ps = points.getData(s);
      auto pe = points.getData(e);
      float x = ps.pos[0] - pe.pos[0];
      float y = ps.pos[1] - pe.pos[1];
      float z = ps.pos[2] - pe.pos[2];
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
    for (uint32 i = 0;i < testCount; ++i){
      keys s = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;
      keys e = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;
      std::cout << "[" << s.first << " " << s.second << "] " << "[" << e.first <<  " "  << e.second << "]" << std::endl;
      getPath(s, e) ;
    }


  }

bool str2U(const char* txt, uint32& v){
  try{
    v = static_cast<uint32>(std::stoul(txt));
    return true;
  } catch(...){
    return false;
  }
}
int main(int argc, char* argv[]){
  uint32 X = 20, Y=26, count = 30;
  if (argc > 1){ str2U(argv[1], X);}
  if (argc > 2){ str2U(argv[2], Y);}
  if (argc > 3){ str2U(argv[3], count);}
  testAstar(X, Y, count);  
  return 0;
}