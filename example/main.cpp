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

  // ============================================================
  // attributes

  if (auto session_id = pqrs::osx::session::find_caller_security_session()) {
    std::cout << "session_id: " << *session_id << std::endl;
    if (auto attributes = pqrs::osx::session::find_attributes(*session_id)) {
      std::cout << "  attributes.is_root " << attributes->get_is_root() << std::endl;
      std::cout << "  attributes.has_graphic_access " << attributes->get_has_graphic_access() << std::endl;
      std::cout << "  attributes.has_tty " << attributes->get_has_tty() << std::endl;
      std::cout << "  attributes.is_remote " << attributes->get_is_remote() << std::endl;
    }
  }

  // ============================================================
  // monitor

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
