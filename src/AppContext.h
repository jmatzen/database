#pragma once

#include <boost/unordered_map.hpp>

namespace jm
{
  class Service;
  
  class AppContext
  {
  public:
    template<typename T>
    void RegisterService(std::shared_ptr<T> service);
    
    std::shared_ptr<T> GetService() const {
      
    }
    
  private:
    boost::unordered_map<std::type_inf
  };
}
