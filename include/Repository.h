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
          auto catalogue_it = catalogues.find(*catalogue_id);
          if(catalogue_it != std::end(catalogues))
          { catalogue_it->second->events[event->uuid] = event; }
        }
        event_pool[event->uuid] = event;
      }
    }

    uuids::uuid new_catalogue(const std::string& name)
    {
      std::unique_ptr<Catalogue_t> catalogue = std::make_unique<Catalogue_t>();
      catalogue->name                        = name;
      auto uuid                              = catalogue->uuid;
      catalogues[uuid]                       = std::move(catalogue);
      return uuid;
    }

    std::shared_ptr<Event_t> event(uuids::uuid event_id)
    {
      auto event_it = event_pool.find(event_id);
      if(event_it != std::end(event_pool)) { return event_it->second; }
      return nullptr;
    }

    Catalogue_t* catalogue(uuids::uuid catalogue_id)
    {
      auto catalogue_it = catalogues.find(catalogue_id);
      if(catalogue_it != std::end(catalogues))
      { return catalogue_it->second.get(); }
      return nullptr;
    }

  private:
    std::map<uuids::uuid, std::shared_ptr<Event_t>> event_pool;
    std::map<uuids::uuid, std::unique_ptr<Catalogue_t>> catalogues;
  };
} // namespace CatalogiCpp

#endif
