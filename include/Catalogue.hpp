#ifndef CATALOGUE_H
#define CATALOGUE_H
#include <Event.hpp>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <uuid.h>

namespace CatalogiCpp
{
  template<typename time_t = double> struct Catalogue
  {
    using uuid_t    = uuids::uuid;
    using Event_t   = Event<time_t>;
    using Event_ptr = std::shared_ptr<Event_t>;
    template<typename... _Args>
    static inline Event_ptr make_event_ptr(_Args&&... __args)
    {
      return std::make_shared<Event_t>(std::forward<_Args>(__args)...);
    }
    std::string name;
    uuid_t uuid = make_uuid();
    std::map<void*, std::function<void(Event_ptr)>> event_added_callbacks;

    Catalogue() {}
    Catalogue(const std::string& name, const uuid_t& uuid,
              const std::map<uuid_t, Event_ptr>& events)
        : name{name}, uuid{uuid}, _events{events}
    {}

    bool contains(const uuid_t& id)
    {
      return std::end(_events) != _events.find(id);
    }

    const std::map<uuid_t, Event_ptr> events() const { return _events; }

    Event_ptr event(const uuid_t& id) { return _events[id]; }

    const Event_ptr& event(const uuid_t& id) const { return _events.at(id); }

    void add(Event_ptr event)
    {
      if(event)
      {
        _events[event->uuid] = event;
        for(auto& [_, callback] : event_added_callbacks)
        {
          callback(event);
        }
      }
    }

    void remove(const uuid_t& id) { _events.erase(id); }

    void remove(Event_ptr& e) { remove(e->uuid); }

    /// @NOTE this is not very efficient since startTime doesn't cache the value
    std::optional<time_t> startTime() const
    {
      if(_events.size())
      {
        return std::min_element(std::cbegin(_events), std::cend(_events),
                                [](const auto& x, const auto& y) {
                                  auto sx = x.second->startTime(),
                                       sy = y.second->startTime();
                                  if(sx && sy)
                                    return sx.value() < sy.value();
                                  else
                                    return false;
                                })
            ->second->startTime();
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<time_t> stopTime() const
    {
      if(_events.size())
      {
        return std::max_element(std::cbegin(_events), std::cend(_events),
                                [](const auto& x, const auto& y) {
                                  auto sx = x.second->stopTime(),
                                       sy = y.second->stopTime();
                                  if(sx && sy)
                                    return sx.value() < sy.value();
                                  else
                                    return false;
                                })
            ->second->stopTime();
      }
      else
      {
        return std::nullopt;
      }
    }

    auto begin() { return std::begin(_events); }
    auto end() { return std::end(_events); }

    auto cbegin() const noexcept { return std::cbegin(_events); }
    auto cend() const noexcept { return std::cend(_events); }

  private:
    std::map<uuid_t, Event_ptr> _events;
  };

} // namespace CatalogiCpp

namespace std
{
  template<typename time_t> auto begin(const CatalogiCpp::Catalogue<time_t>& c)
  {
    return c.cbegin();
  }
  template<typename time_t> auto end(const CatalogiCpp::Catalogue<time_t>& c)
  {
    return c.cend();
  }
} // namespace std

namespace CatalogiCpp
{
  template<typename time_t>
  bool operator==(const Catalogue<time_t>& lhs, const Catalogue<time_t>& rhs)
  {
    return (lhs.name == rhs.name) && (lhs.uuid == rhs.uuid) &&
           (std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs)));
  }

  template<typename time_t>
  bool operator!=(const Catalogue<time_t>& lhs, const Catalogue<time_t>& rhs)
  {
    return !(lhs == rhs);
  }
} // namespace CatalogiCpp

#endif // CATALOGUE_H
