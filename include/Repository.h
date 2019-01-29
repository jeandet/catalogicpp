#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <Catalogue.h>
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
  template<typename time_t = double> struct Repository
  {
    using Product_t   = Product<time_t>;
    using Event_t     = Event<time_t>;
    using Catalogue_t = Catalogue<time_t>;

    std::string name;

    void add_event(std::shared_ptr<Event_t> event,
                   std::optional<uuids::uuid> catalogue_id = std::nullopt)
    {
      if(event)
      {
        if(catalogue_id)
        {
          auto catalogue_it = _catalogues.find(*catalogue_id);
          if(catalogue_it != std::end(_catalogues))
          { catalogue_it->second->events[event->uuid] = event; }
        }
        _event_pool[event->uuid] = event;
      }
    }

    void add_catalogue(std::unique_ptr<Catalogue_t> catalogue)
    {
      if(catalogue) { _catalogues[catalogue->uuid] = std::move(catalogue); }
    }

    uuids::uuid new_catalogue(const std::string& name)
    {
      std::unique_ptr<Catalogue_t> catalogue = std::make_unique<Catalogue_t>();
      catalogue->name                        = name;
      auto uuid                              = catalogue->uuid;
      _catalogues[uuid]                      = std::move(catalogue);
      return uuid;
    }

    std::shared_ptr<Event_t> event(uuids::uuid event_id)
    {
      auto event_it = _event_pool.find(event_id);
      if(event_it != std::end(_event_pool)) { return event_it->second; }
      return nullptr;
    }

    Catalogue_t* catalogue(uuids::uuid catalogue_id)
    {
      auto catalogue_it = _catalogues.find(catalogue_id);
      if(catalogue_it != std::end(_catalogues))
      { return catalogue_it->second.get(); }
      return nullptr;
    }

    const std::map<uuids::uuid, std::shared_ptr<Event_t>>& events() const
    {
      return _event_pool;
    }

    const std::map<uuids::uuid, std::unique_ptr<Catalogue_t>>&
    catalogues() const
    {
      return _catalogues;
    }

    friend bool operator==(const Repository<time_t>& lhs,
                           const Repository<time_t>& rhs)
    {
      bool isEqual = (lhs.name == rhs.name);
      isEqual &= std::equal(
          std::begin(lhs._event_pool), std::end(lhs._event_pool),
          std::begin(rhs._event_pool), [](const auto& a, const auto& b) {
            return (a.first == b.first) && (*a.second == *b.second);
          });
      isEqual &= std::equal(
          std::begin(lhs._catalogues), std::end(lhs._catalogues),
          std::begin(rhs._catalogues), [](const auto& a, const auto& b) {
            return (a.first == b.first) && (*a.second == *b.second);
          });
      return isEqual;
    }

  private:
    std::map<uuids::uuid, std::shared_ptr<Event_t>> _event_pool;
    std::map<uuids::uuid, std::unique_ptr<Catalogue_t>> _catalogues;
  };
} // namespace CatalogiCpp

#endif
