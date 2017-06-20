#include <set>
#include <random>
#include <array>
#include <iostream>
#include <string>
#include <cmath>
#include "AStar.h"
#include "MapTemplate.h"
using namespace  AStarSpace;

typedef unsigned int uint32;
typedef std::pair<uint32, uint32> keys;

enum Tag{
  normal = 0,
  obstacle = 1,
};
struct Point{
  std::array<float, 3> pos;   //位置
  std::list<keys> link;       //可以到达的点
  Tag tag;
};


typedef  ShareSpace::Utility::ObjectMap<keys, Point> Map;


uint32 grandom(uint32 min, uint32 max){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32> dis(min, max - 1);
  return dis(rd);
}

void testAstar(uint32 MaxSizeX,  uint32 MaxSizeY, uint32 obstacleCount,  uint32 testCount){
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
        p.tag = normal;
        addlink(p, std::make_pair(i, j - 1));
        addlink(p, std::make_pair(i, j + 1));
        addlink(p, std::make_pair(i + 1, j));
        addlink(p, std::make_pair(i + 1, j + 1));
        addlink(p, std::make_pair(i + 1, j - 1));
        addlink(p, std::make_pair(i - 1, j + 1));
        addlink(p, std::make_pair(i - 1, j));
        addlink(p, std::make_pair(i - 1, j - 1));
        points.addData(std::make_pair(i, j), p);
      }
    }
    //生成障碍点
    for (uint32 i = 0 ; i < obstacleCount;++i){
      keys s = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) );
      auto& p = points.getData(s);
      p.tag = obstacle;

      //删除障碍点的邻居
      for (auto& l : p.link){
        auto n = points.getData(l);
        n.link.remove(s);
      }

      p.link.clear();
    }

    typedef AStart<keys, uint32> AStarType;
    //估值函数
    auto h = [&points](const Map::key_type& s, const Map::key_type& e){
      auto ps = points.getData(s);
      auto pe = points.getData(e);
      float x = ps.pos[0] - pe.pos[0];
      float y = ps.pos[1] - pe.pos[1];
      float z = ps.pos[2] - pe.pos[2];
      return static_cast<uint32>(sqrtl(x*x + y*y + z*z) * 100);
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
      auto r = p.link;

      //这里其实可以不要.上面生成的图中已经去掉了. 但实际项目中可能会要是用 比如排除一些点等等,
      r.remove_if([&points](const keys& k){return points.getData(k).tag == obstacle;});
      return std::move(r);
    };


    AStarType object(h, g, e, links);

    //输出寻路结果
    auto getPath = [&object, MaxSizeX, MaxSizeY,&points](const keys& s, const keys& e){
      auto path = object.findPath(s, e);
      std::set<keys> PathSet;
      for(auto& d : path){
        PathSet.insert(d);
      }
      for(uint32 i = 0; i < MaxSizeX; ++i){
        std::string str = "";
        for(uint32 j = 0; j < MaxSizeY; ++j){ 
          auto k = std::make_pair(i, j);
          auto p = points.getData(k);
          if (PathSet.count(k)){
            str.append("#");
          }else if (p.tag ==  obstacle){
            str.append("o");
          } else{
            str.append(" ");
          }
        }
        std::cout << str.c_str()  << std::endl;
      }
      std::cout << "------------------------------------------------------------"<< std::endl;
    };


    //测试寻路
    for (uint32 i = 0;i < testCount;){
      keys s = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;
      keys e = std::make_pair(grandom(0, MaxSizeX - 1) , grandom(0, MaxSizeY - 1) ) ;

      auto& ps = points.getData(s);
      auto& pe = points.getData(e);
      if (ps.tag == obstacle ||  pe.tag == obstacle){
        continue;
      }
      ++i;
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
  uint32 XMax = 20, YMax=26, testCount = 30, obstacleCount = 20;
  if (argc > 1){ str2U(argv[1], XMax);}
  if (argc > 2){ str2U(argv[2], YMax);}
  if (argc > 3){ str2U(argv[3], testCount);}
  if (argc > 4){ str2U(argv[4], obstacleCount);}

  testAstar(XMax, YMax, obstacleCount, testCount);  
  return 0;
}