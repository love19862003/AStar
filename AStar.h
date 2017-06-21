/********************************************************************
    Filename: AStar.h
    Description:
    Version:  1.0
    Created:  20:6:2017   17:28

    Compiler: gcc vc
    Author:   wufan, love19862003@163.com
    Organization: lezhuogame
*********************************************************************/
#ifndef __AStar_H__
#define __AStar_H__
#include <memory>
#include <algorithm>
#include <list>
#include <functional>

namespace AStarSpace{
  
  template<typename T, typename W = unsigned int>
  class AStart{

  private:
    typedef T DataType;
    typedef W WeightType;

    struct Data{
      explicit Data(const DataType& d): _data(d), _g(0), _h(0), _parent(nullptr){}
      DataType _data;
      WeightType _g;          
      WeightType _h;          
      std::shared_ptr<Data> _parent;
    };

    typedef std::shared_ptr<Data> Pointer;
    typedef std::list<Pointer> NodeList;
    typedef std::list<DataType> PathList;
    typedef std::function<WeightType(const DataType&, const DataType&)> HFunctor; //估值函数
    typedef std::function<WeightType(const DataType&, const DataType&)> GFunctor; //消耗函数 
    typedef std::function<bool(const DataType&, const DataType&)>   EFunctor;     //判断节点是否相同
    typedef std::function<std::list<DataType>(const DataType&)>     LFunctor;     //返回节点相连节点

  public:
    explicit AStart(const HFunctor& h, const GFunctor& g, const EFunctor& e, const LFunctor& l):_h_fun(h), _g_fun(g), _e_fun(e), _l_fun(l){

    }
    ~AStart(){
      _open.clear();
      _close.clear();
      _path.clear();
    }

    PathList findPath(const DataType& s, const DataType& e){
      Pointer start = std::make_shared<Data>(s);
      Pointer end = std::make_shared<Data>(e);
      doFindPath(start, end);
      _open.clear();
      _close.clear();
      return _path;
    }
  private:
    bool doFindPath(Pointer start, Pointer end){
      _path.clear();
      if(!start || !end){ return false; }
      if(start == end){ _path.push_back(start->_data); return true; }
  
      start->_h = _h_fun(start->_data, end->_data);
      _open.push_back(start);
      while(!_open.empty()){
        Pointer front = _open.front();
       
        if(_e_fun(front->_data, end->_data)){
          while(front){
            _path.push_front(front->_data);
            front = front->_parent;
          }
          break;
        }
        _open.pop_front();
        auto links =  _l_fun(front->_data);
        for(auto& n : links){
          if(isClose(n)){ continue; }
          WeightType g = front->_g + _g_fun(front->_data, n);
          auto node  = openNode(n);
          if(!node){
            node = std::make_shared<Data>(n);
            _open.push_back(node);
            node->_h = _h_fun(n, end->_data);
            node->_g = g;
            node->_parent = front;
          } else if(node->_g > g){
            node->_g = g;
            node->_parent = front;
          }
        }

        _close.push_back(front);
        _open.sort([](const Pointer& l, const Pointer& r){return  l->_h +  l->_g < r->_h + r->_g;});
      }
      return !_path.empty();
    }

    Pointer openNode(const DataType& p){
      auto it = std::find_if(_open.begin(),
                             _open.end(), 
                             [&p, this](const Pointer& t){return _e_fun(p, t->_data);});
      if (it != _open.end()){
        return *it;
      }
      return nullptr;
    }

    bool isClose(const DataType& p){
      return std::any_of(_close.begin(), _close.end(), [&p, this](const Pointer& t){return _e_fun(p, t->_data);});
    }
  private:
    NodeList _open;   
    NodeList _close;
    HFunctor _h_fun;
    GFunctor _g_fun;
    EFunctor _e_fun;
    LFunctor _l_fun;
    PathList _path;
  };
  
}
#endif // __AStar_H__