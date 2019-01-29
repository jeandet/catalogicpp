#ifndef CATALOGUE_H
#define CATALOGUE_H
#include <Event.h>
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
    std::map<uuids::uuid, std::shared_ptr<Event_t>> events;
    uuids::uuid uuid = make_uuid();

    bool contains(const uuids::uuid& id)
    {
      return std::end(events) != events.find(id);
    }

    /// @NOTE this is not very efficient since startTime doesn't cache the value
    std::optional<time_t> startTime()
    {
      if(events.size())
      {
        return std::min_element(std::begin(events), std::end(events),
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
      if(events.size())
      {
        return std::max_element(std::begin(events), std::end(events),
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
  };

  template<typename time_t>
  bool operator==(const Catalogue<time_t>& lhs, const Catalogue<time_t>& rhs)
  {
    return (lhs.name == rhs.name) && (lhs.uuid == rhs.uuid) &&
           (std::equal(std::begin(lhs.events), std::end(lhs.events),
                       std::begin(rhs.events)));
  }

} // namespace CatalogiCpp

#endif // CATALOGUE_H
