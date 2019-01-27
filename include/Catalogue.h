#ifndef CATALOGUE_H
#define CATALOGUE_H
#include <Event.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <uuid.h>

namespace CatalogiCpp
{
  template<typename time_t = double> struct Catalogue
  {
    using Event_t = Event<time_t>;
    std::string name;
    std::map<uuid_t, std::shared_ptr<Event_t>> events;
    uuids::uuid uuid = make_uuid();

    /// @NOTE this is not very efficient since startTime doesn't cache the value
    time_t startTime()
    {
      if(events.size())
      {
        return std::min_element(std::begin(events), std::end(events),
                                [](const Event_t& x, const Event_t& y) {
                                  return x.startTime() < y.startTime();
                                })
            ->startTime();
      }
      else
      {
        return time_t{};
      }
    }

    time_t stopTime()
    {
      if(events.size())
      {
        return std::max_element(std::begin(events), std::end(events),
                                [](const Event_t& x, const Event_t& y) {
                                  return x.stopTime() < y.stopTime();
                                })
            ->stopTime();
      }
      else
      {
        return time_t{};
      }
    }
  };

} // namespace CatalogiCpp

#endif // CATALOGUE_H
