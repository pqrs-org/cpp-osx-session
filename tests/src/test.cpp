#include <boost/ut.hpp>
#include <pqrs/cf/number.hpp>
#include <pqrs/cf/string.hpp>
#include <pqrs/osx/session.hpp>

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "attributes"_test = [] {
    {
      pqrs::osx::session::attributes attributes(0);

      expect(!attributes.get_is_root());
      expect(!attributes.get_has_graphic_access());
      expect(!attributes.get_has_tty());
      expect(!attributes.get_is_remote());
    }

    {
      pqrs::osx::session::attributes attributes(sessionIsRoot |
                                                sessionHasGraphicAccess |
                                                sessionHasTTY |
                                                sessionIsRemote);

      expect(attributes.get_is_root());
      expect(attributes.get_has_graphic_access());
      expect(attributes.get_has_tty());
      expect(attributes.get_is_remote());
    }
  };

  "cg_attributes"_test = [] {
    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));

      pqrs::osx::session::cg_attributes attributes(*dictionary);

      expect(attributes.get_console_set() == std::nullopt);
      expect(attributes.get_login_done() == std::nullopt);
      expect(attributes.get_on_console() == std::nullopt);
      expect(attributes.get_user_id() == std::nullopt);
      expect(attributes.get_user_name() == std::nullopt);
    }

    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));
      auto console_set = pqrs::cf::make_cf_number(int64_t(12));
      auto user_id = pqrs::cf::make_cf_number(int64_t(501));
      auto user_name = pqrs::cf::make_cf_string("example");

      CFDictionarySetValue(*dictionary, kCGSessionConsoleSetKey, *console_set);
      CFDictionarySetValue(*dictionary, kCGSessionLoginDoneKey, kCFBooleanTrue);
      CFDictionarySetValue(*dictionary, kCGSessionOnConsoleKey, kCFBooleanFalse);
      CFDictionarySetValue(*dictionary, kCGSessionUserIDKey, *user_id);
      CFDictionarySetValue(*dictionary, kCGSessionUserNameKey, *user_name);

      pqrs::osx::session::cg_attributes attributes(*dictionary);

      expect(attributes.get_console_set() == std::optional<uint32_t>(12));
      expect(attributes.get_login_done() == std::optional<bool>(true));
      expect(attributes.get_on_console() == std::optional<bool>(false));
      expect(attributes.get_user_id() == std::optional<uid_t>(501));
      expect(attributes.get_user_name() == std::optional<std::string>("example"));
    }
  };

  "monitor"_test = [] {
    auto time_source = std::make_shared<pqrs::dispatcher::hardware_time_source>();
    auto dispatcher = std::make_shared<pqrs::dispatcher::dispatcher>(time_source);

    auto monitor = std::make_shared<pqrs::osx::session::monitor>(dispatcher);

    monitor = nullptr;

    dispatcher->terminate();
    dispatcher = nullptr;
  };

  return 0;
}
