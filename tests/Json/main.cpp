#include "gtest/gtest.h"
#include <Catalogue.hpp>
#include <CatalogueIO.hpp>
#include <Event.hpp>
#include <Repository.hpp>
#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#else
#include <json.hpp>
#endif

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
      Event_t e = {"Event1",
                   {},
                   {Event_t::Product_t{"Product1", 10., 11.},
                    Event_t::Product_t{"Product2", 11., 12.},
                    Event_t::Product_t{"Product3", 10.2, 11.}},
                   CatalogiCpp::make_uuid()};
      c.add(Catalogue_t::make_event_ptr(e));
    }

    virtual ~ACatalogue() {}
    Catalogue_t c = {"Catastrophic", CatalogiCpp::make_uuid(), {}};
  };

  TEST_F(ACatalogue, CanBeSavedToJson)
  {
    // using json = nlohmann::json;
    // json js    = c;
    // std::stringstream ss;
    // ss << js;
    // json js2;
    // js2 << ss;
    // auto c2 = js2.get<CatalogiCpp::Catalogue<double>>();
    // EXPECT_EQ(c, c2);
    // TODO Should think about this later but saving a catalogue alone doesn't
    // make any sense now
  }

} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
