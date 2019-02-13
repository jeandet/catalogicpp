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
    using Event_t = Event<time_t>;
    std::string name;
    uuids::uuid uuid = make_uuid();
    std::map<void*, std::function<void(std::shared_ptr<Event_t>)>>
        event_added_callbacks;

    Catalogue() {}
    Catalogue(const std::string& name, const uuids::uuid& uuid,
              const std::map<uuids::uuid, std::shared_ptr<Event_t>>& events)
        : name{name}, uuid{uuid}, _events{events}
    {}

    bool contains(const uuids::uuid& id)
    {
      return std::end(_events) != _events.find(id);
    }

    const std::map<uuids::uuid, std::shared_ptr<Event_t>> events() const
    {
      return _events;
    }

    std::shared_ptr<Event_t> event(const uuids::uuid& id)
    {
      return _events[id];
    }

    const Event_t& event(const uuids::uuid& id) const { return *_events[id]; }

    void add(std::shared_ptr<Event_t> e)
    {
      if(e)
      {
        _events[e->uuid] = e;
        for(auto& [_, callback] : event_added_callbacks)
        {
          callback(e);
        }
      }
    }

    void remove(const uuids::uuid& id) { _events.erase(id); }

    void remove(std::shared_ptr<Event_t>& e) { remove(e->uuid); }

    /// @NOTE this is not very efficient since startTime doesn't cache the value
    std::optional<time_t> startTime()
    {
      if(_events.size())
      {
        return std::min_element(std::begin(_events), std::end(_events),
                                [](const Event_t& x, const Event_t& y) {
                                  auto sx = x.startTime(), sy = y.startTime();
                                  if(sx && sy)
                                    return sx.value() < sy.value();
                                  else
                                    return false;
                                })
            ->startTime();
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<time_t> stopTime()
    {
      if(_events.size())
      {
        return std::max_element(std::begin(_events), std::end(_events),
                                [](const Event_t& x, const Event_t& y) {
                                  auto sx = x.stopTime(), sy = y.stopTime();
                                  if(sx && sy)
                                    return sx.value() < sy.value();
                                  else
                                    return false;
                                })
            ->stopTime();
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
    std::map<uuids::uuid, std::shared_ptr<Event_t>> _events;
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
