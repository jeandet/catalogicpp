#ifndef JSONIO_H
#define JSONIO_H
#include <Catalogue.h>
#include <Event.h>
#include <Repository.h>
#include <json.hpp>
#include <uuid.h>

using nlohmann::json;

namespace nlohmann
{
  template<> struct adl_serializer<uuids::uuid>
  {
    static void to_json(json& j, const uuids::uuid& id)
    {
      j = json{uuids::to_string(id)};
    }

    static void from_json(const json& j, uuids::uuid& id)
    {
      id.from_string(std::string{j});
    }
  };
} // namespace nlohmann

namespace CatalogiCpp
{
  template<typename time_t = double>
  void to_json(json& j, const CatalogiCpp::Product<time_t>& p)
  {
    j = json{
        {"name", p.name}, {"startTime", p.startTime}, {"stopTime", p.stopTime}};
  }

  template<typename time_t = double>
  void from_json(const json& j, CatalogiCpp::Product<time_t>& p)
  {
    j.at("name").get_to(p.name);
    j.at("startTime").get_to(p.startTime);
    j.at("stopTime").get_to(p.stopTime);
  }

  template<typename time_t = double>
  void to_json(json& j, const CatalogiCpp::Event<time_t>& e)
  {
    j = json{{"name", e.name},
             {"tags", e.tags},
             {"uuid", e.uuid},
             {"products", e.products}};
  }

  template<typename time_t = double>
  void from_json(const json& j, CatalogiCpp::Event<time_t>& e)
  {
    j.at("name").get_to(e.name);
    j.at("tags").get_to(e.tags);
    j.at("uuid").get_to(e.uuid);
    j.at("products").get_to(e.products);
  }

  template<typename time_t = double>
  void to_json(json& j, const std::shared_ptr<CatalogiCpp::Event<time_t>>& e)
  {
    j = json{{"uuid", e->uuid}};
  }

  template<typename time_t = double>
  void from_json(const json& j, std::shared_ptr<CatalogiCpp::Event<time_t>>& e)
  {
    e = nullptr;
  }

  template<typename time_t = double>
  void to_json(json& j, const CatalogiCpp::Catalogue<time_t>& c)
  {
    j = json{{"name", c.name}, {"uuid", c.uuid}, {"events", c.events}};
  }

  template<typename time_t = double>
  void from_json(const json& j, CatalogiCpp::Catalogue<time_t>& c)
  {
    j.at("name").get_to(c.name);
    j.at("uuid").get_to(c.uuid);
    j.at("events").get_to(c.events);
  }

  template<typename time_t = double>
  void to_json(json& j, const CatalogiCpp::Repository<time_t>& r)
  {
    j = json{{"name", r.name}};
    for(auto& event : r.event_pool)
    {
      j["event"][event->uuid] = *event;
    }
  }

  template<typename time_t = double>
  void from_json(const json& j, CatalogiCpp::Repository<time_t>& r)
  {
    j.at("name").get_to(r.name);
  }
} // namespace CatalogiCpp
#endif
