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
  // cg

  if (auto attributes = pqrs::osx::session::find_cg_session_current_attributes()) {
    std::cout << "current_attributes: " << std::endl;
    if (auto& v = attributes->get_console_set()) {
      std::cout << "  console_set: " << *v << std::endl;
    }
    if (auto& v = attributes->get_login_done()) {
      std::cout << "  login_done: " << *v << std::endl;
    }
    if (auto& v = attributes->get_on_console()) {
      std::cout << "  on_console: " << *v << std::endl;
    }
    if (auto& v = attributes->get_user_id()) {
      std::cout << "  user_id: " << *v << std::endl;
    }
    if (auto& v = attributes->get_user_name()) {
      std::cout << "  user_name: " << *v << std::endl;
    }
  } else {
    std::cout << "cg_session_current_attributes: std::nullopt" << std::endl;
  }

  // ============================================================
  // monitor

  auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
  auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

  auto monitor = std::make_shared<pqrs::osx::session::monitor>(dispatcher);

  monitor->on_console_changed.connect([](auto&& on_console) {
    std::cout << "on_console_changed: " << on_console << std::endl;
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
