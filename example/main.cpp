#include <csignal>
#include <iostream>
#include <pqrs/osx/session.hpp>

namespace {
auto global_wait = pqrs::make_thread_wait();
}

int main(void) {
  std::signal(SIGINT, [](int) {
    global_wait->notify();
  });

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  auto monitor = std::make_shared<pqrs::osx::session::monitor>(dispatcher);

  monitor->console_user_id_changed.connect([](auto&& console_user_id) {
    std::cout << "console_user_id_changed (console_user_id:";
    if (console_user_id) {
      std::cout << *console_user_id;
    } else {
      std::cout << "empty";
    }
    std::cout << ")" << std::endl;
  });

  monitor->async_start(std::chrono::milliseconds(1000));

  // ============================================================

  global_wait->wait_notice();

  // ============================================================

  monitor = nullptr;

  dispatcher->terminate();
  dispatcher = nullptr;

  std::cout << "finished" << std::endl;

  return 0;
}
