#pragma once

#include <boost/unordered_map.hpp>
#include <utility>
#include <memory>
#include <typeinfo>
#include <stdexcept>

namespace jm
{
  class Service;
  
  class AppContext
  {
  public:
    template<typename T, typename... Args>
    void RegisterService(Args&&... args) {
      serviceMap_.emplace(
        typeid(T), 
        std::make_shared<T>(std::forward<Args>(args)...));
    }
    
    template<typename T>
    T& GetService() const {
      auto it = serviceMap_.find(typeid(T));
      if (it == serviceMap_.end())
        throw std::runtime_error("unable to find requested service");
      return static_cast<T&>(*it->second);
    }
    
    static AppContext& GetAppContext();

  private:
    using TypeInfoRef = std::reference_wrapper<const std::type_info>;

    struct TypeInfoEqualTo {
      bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
        return lhs.get() == rhs.get();
      }
    };
    struct TypeInfoHash {
      size_t operator()(TypeInfoRef val) const {
        return val.get().hash_code();
      }
    };
    boost::unordered_map<
      std::reference_wrapper<const std::type_info>, 
      std::shared_ptr<Service>,
      TypeInfoHash, TypeInfoEqualTo
    > serviceMap_;
  };
}
