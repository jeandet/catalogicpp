#include "gtest/gtest.h"
#include <Catalogue.h>
#include <CatalogueIO.h>
#include <Event.h>
#include <Repository.h>
#include <json.hpp>

namespace
{
  class ACatalogue : public ::testing::Test
  {
    using Catalogue_t = CatalogiCpp::Catalogue<double>;
    using Event_t     = CatalogiCpp::Event<double>;

  protected:
    ACatalogue() {}
    void SetUp()
    {
      Event_t e        = {"Event1",
                   {},
                   {Event_t::Product_t{"Product1", 10., 11.},
                    Event_t::Product_t{"Product2", 11., 12.},
                    Event_t::Product_t{"Product3", 10.2, 11.}},
                   CatalogiCpp::make_uuid()};
      c.events[e.uuid] = std::make_shared<Event_t>(e);
    }

    virtual ~ACatalogue() {}
    Catalogue_t c = {"Catastrophic", {}, CatalogiCpp::make_uuid()};
  };

  TEST_F(ACatalogue, CanBeSavedToJson)
  {
    using json = nlohmann::json;
    json js    = c;
    std::stringstream ss;
    ss << js;
    json js2;
    js2 << ss;
    auto c2 = js2.get<CatalogiCpp::Catalogue<double>>();
    EXPECT_EQ(c, c2);
  }

} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
