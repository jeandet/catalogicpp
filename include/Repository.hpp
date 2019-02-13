#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <Catalogue.hpp>
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
  template<typename time_t = double> struct Repository
  {
    using Product_t   = Product<time_t>;
    using Event_t     = Event<time_t>;
    using Catalogue_t = Catalogue<time_t>;
    using uuid_t      = uuids::uuid;

    std::string name;

    Repository() = default;
    Repository(const Repository& other)
    {
      for(const auto& [uuid, event] : other._event_pool)
      {
        _add_to_pool(std::make_shared<Event_t>(*event));
      }

      for(const auto& [uuid, catalog] : other._catalogues)
      {
        auto catalogue_copy  = std::make_shared<Catalogue_t>();
        catalogue_copy->uuid = catalog->uuid;
        catalogue_copy->name = catalog->name;
        for(const auto& [uuid, event] : other._event_pool)
        {
          catalogue_copy->add(_event_pool[uuid]);
        }
        add(catalog);
      }
    }

    ~Repository()
    {
      for(auto& [_, catalogue] : _catalogues)
      {
        _unregister_catalogue_callback(catalogue);
      }
    }

    void add(std::shared_ptr<Event_t> event,
             std::optional<uuids::uuid> catalogue_id = std::nullopt)
    {
      if(event)
      {
        if(catalogue_id)
        {
          auto catalogue_it = _catalogues.find(*catalogue_id);
          if(catalogue_it != std::end(_catalogues))
          { catalogue_it->second->add(event); }
        }
        else
        {
          _add_to_pool(event);
        }
      }
    }

    void add(std::shared_ptr<Catalogue_t> catalogue)
    {
      if(catalogue)
      {
        _catalogues[catalogue->uuid] = catalogue;
        _register_catalogue_callback(catalogue);
      }
    }

    void remove(std::shared_ptr<Event_t> event,
                std::optional<uuids::uuid> catalogue_id = std::nullopt)
    {
      if(event)
      {
        auto event_id = event->uuid;
        if(catalogue_id)
        {
          auto catalogue_it = _catalogues.find(*catalogue_id);
          if(catalogue_it != std::end(_catalogues))
          { catalogue_it->second->remove(event_id); }
        }
        else
        {
          for(auto& [_, catalogue] : _catalogues)
          {
            catalogue->remove(event_id);
          }
          _event_pool.erase(event_id);
        }
      }
    }

    void remove(std::shared_ptr<Catalogue_t> catalogue)
    {
      if(catalogue) { _catalogues.erase(catalogue->uuid); }
    }

    uuids::uuid new_catalogue(const std::string& name)
    {
      std::shared_ptr<Catalogue_t> catalogue = std::make_shared<Catalogue_t>();
      catalogue->name                        = name;
      auto uuid                              = catalogue->uuid;
      add(catalogue);
      return uuid;
    }

    std::shared_ptr<Event_t> event(uuid_t event_id)
    {
      auto event_it = _event_pool.find(event_id);
      if(event_it != std::end(_event_pool)) { return event_it->second; }
      return nullptr;
    }

    std::shared_ptr<Catalogue_t> catalogue(uuid_t catalogue_id)
    {
      auto catalogue_it = _catalogues.find(catalogue_id);
      if(catalogue_it != std::end(_catalogues)) { return catalogue_it->second; }
      return nullptr;
    }

    const std::map<uuid_t, std::shared_ptr<Event_t>>& events() const
    {
      return _event_pool;
    }

    const std::map<uuid_t, std::shared_ptr<Catalogue_t>>& catalogues() const
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

    friend bool operator!=(const Repository<time_t>& lhs,
                           const Repository<time_t>& rhs)
    {
      return !(lhs == rhs);
    }

  private:
    void _add_to_pool(std::shared_ptr<Event_t> event)
    {
      _event_pool[event->uuid] = event;
    }

    void _register_catalogue_callback(std::shared_ptr<Catalogue_t> catalogue)
    {
      if(catalogue)
      {
        catalogue->event_added_callbacks[this] =
            [this](std::shared_ptr<Event_t> event) {
              this->_add_to_pool(event);
            };
      }
    }

    void _unregister_catalogue_callback(std::shared_ptr<Catalogue_t> catalogue)
    {
      if(catalogue) { catalogue->event_added_callbacks.erase(this); }
    }

    std::map<uuid_t, std::shared_ptr<Event_t>> _event_pool;
    std::map<uuid_t, std::shared_ptr<Catalogue_t>> _catalogues;
  };
} // namespace CatalogiCpp

#endif
