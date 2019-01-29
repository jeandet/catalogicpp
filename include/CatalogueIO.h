#ifndef JSONIO_H
#define JSONIO_H
#include <Catalogue.h>
#include <Event.h>
#include <Repository.h>
#include <fstream>
#include <json.hpp>
#include <uuid.h>

using nlohmann::json;

namespace nlohmann
{
  template<> struct adl_serializer<uuids::uuid>
  {
    static void to_json(json& j, const uuids::uuid& id)
    {
      j = uuids::to_string(id);
    }

    static void from_json(const json& j, uuids::uuid& id)
    {
      id = uuids::uuid::from_string(j.get<std::string>()).value();
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
    j["name"] = r.name;
    for(auto& event : r.events())
    {
      j["events"].push_back(*event.second);
    }
    for(auto& catalogue : r.catalogues())
    {
      j["catalogues"].push_back(*catalogue.second);
    }
  }

  template<typename time_t = double>
  void from_json(const json& j, CatalogiCpp::Repository<time_t>& r)
  {
    j.at("name").get_to(r.name);
    if(j.find("events") != j.end())
    {
      for(auto& event_js : j["events"])
      {
        r.add_event(std::make_shared<CatalogiCpp::Event<time_t>>(
            event_js.get<CatalogiCpp::Event<time_t>>()));
      }
    }
    if(j.find("catalogues") != j.end())
    {
      for(auto& catalogue_js : j["catalogues"])
      {
        auto catalogue  = std::make_unique<Catalogue<time_t>>();
        catalogue->name = catalogue_js["name"];
        catalogue->uuid = catalogue_js["uuid"];
        for(auto event_js : catalogue_js["events"])
        {
          uuids::uuid uuid        = event_js["uuid"];
          auto event              = r.event(uuid);
          catalogue->events[uuid] = event;
        }
        r.add_catalogue(std::move(catalogue));
      }
    }
  }

  template<typename time_t = double>
  bool save_repository(const CatalogiCpp::Repository<time_t>& r,
                       const std::string& fname)
  {
    json js = r;
    std::ofstream js_file;
    js_file.open(fname);
    js_file << js;
    js_file.close();
  }

  template<typename time_t = double>
  CatalogiCpp::Repository<time_t> load_repository(const std::string& fname)
  {
    std::ifstream js_file(fname);
    json js;
    js_file >> js;
    return js.get<CatalogiCpp::Repository<time_t>>();
    js_file.close();
  }
} // namespace CatalogiCpp
#endif
