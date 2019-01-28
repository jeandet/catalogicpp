#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include <optional>
#include <string>
#include <uuid.h>
#include <vector>

namespace CatalogiCpp
{
  uuids::uuid make_uuid()
  {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    return uuids::uuid_random_generator{generator}();
  }

  template<typename time_t = double> struct Product
  {
    std::string name;
    time_t startTime;
    time_t stopTime;
  };

  template<typename time_t>
  bool operator==(const Product<time_t>& lhs, const Product<time_t>& rhs)
  {
    return (lhs.name == rhs.name) && (lhs.startTime == rhs.startTime) &&
           (lhs.stopTime == rhs.stopTime);
  }

  template<typename time_t = double> struct Event
  {
    using Product_t = Product<time_t>;
    std::string name;
    std::vector<std::string> tags;
    std::vector<Product_t> products;
    uuids::uuid uuid = make_uuid();

    std::optional<time_t> startTime()
    {
      if(products.size())
      {
        return std::min_element(std::begin(products), std::end(products),
                                [](const Product_t& x, const Product_t& y) {
                                  return x.startTime < y.startTime;
                                })
            ->startTime;
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<time_t> stopTime()
    {
      if(products.size())
      {
        return std::max_element(std::begin(products), std::end(products),
                                [](const Product_t& x, const Product_t& y) {
                                  return x.stopTime < y.stopTime;
                                })
            ->stopTime;
      }
      else
      {
        return std::nullopt;
      }
    }
  };

  template<typename time_t>
  bool operator==(const Event<time_t>& lhs, const Event<time_t>& rhs)
  {
    return (lhs.name == rhs.name) && (lhs.uuid == rhs.uuid) &&
           (std::equal(std::begin(lhs.tags), std::end(lhs.tags),
                       std::begin(rhs.tags))) &&
           (std::equal(std::begin(lhs.products), std::end(lhs.products),
                       std::begin(rhs.products)));
  }

} // namespace CatalogiCpp

#endif // EVENT_H
