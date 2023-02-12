#pragma once

// #include <rust/cxx.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>

namespace org::jcan
{

// This is a forward declaration of the Frame struct, which is defined in the Rust library
struct Frame;

// This is a forward declaration of the JBus class, which is defined in the Rust library.
// While we cannot inherit from this class, we can intantiate it within a Bus 'wrapper class'.
// This will allow us to add the c++-specific add_callback functionality around the Rust implementation,
// in a clean way.
class JBus;


// Callback functions
typedef std::function<void(Frame)> CallbackFunction;
// template <class T>
// using MethodCallback = void (T::*)(Frame);


class Bus{
private:
    JBus *jBus; 
    std::map<int, CallbackFunction> callbacks_;

public:

  Bus();
  void open(const char *name);
  
  void set_id_filter(std::vector<uint32_t> allowed_ids);

  void send(Frame frame);
  void receive();

  std::vector<Frame> receive_many();

  void add_callback(int id, void (*callback)(Frame));

  template <typename T>
  void add_callback_to(int id, T *instance, void (T::*method)(Frame))
  {
    // NOTE: This MUST be defined in the header file, since it is a template method!!!
    this->callbacks_[id] = std::bind(method, instance, std::placeholders::_1);
  }

  void spin();
};

std::unique_ptr<Bus> new_bus();
}