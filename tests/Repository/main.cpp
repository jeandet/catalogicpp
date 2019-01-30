#include "gtest/gtest.h"
#include <Catalogue.h>
#include <CatalogueIO.h>
#include <Event.h>
#include <Repository.h>
#include <json.hpp>

namespace
{
  using Repository_t = CatalogiCpp::Repository<double>;
  using Catalogue_t  = Repository_t::Catalogue_t;
  using Event_t      = Repository_t::Event_t;

  class ARepository : public ::testing::Test
  {
  protected:
    ARepository() {}
    void SetUp() {}

    virtual ~ARepository() {}
    Repository_t r;
    Event_t e1 = {"Event1",
                  {},
                  {Event_t::Product_t{"Product1", 10., 11.},
                   Event_t::Product_t{"Product2", 11., 12.},
                   Event_t::Product_t{"Product3", 10.2, 11.}},
                  CatalogiCpp::make_uuid()};

    Event_t e2 = {"Event2",
                  {},
                  {Event_t::Product_t{"Product1", 10., 11.},
                   Event_t::Product_t{"Product2", 11., 12.},
                   Event_t::Product_t{"Product3", 10.2, 11.}},
                  CatalogiCpp::make_uuid()};
  };

  TEST_F(ARepository, CanCreateACatalogue)
  {
    auto id = r.new_catalogue("Catalogue1");
    EXPECT_NE(nullptr, r.catalogue(id));
    EXPECT_EQ("Catalogue1", r.catalogue(id)->name);
  }

  TEST_F(ARepository, CanAddAnEvent)
  {
    std::shared_ptr<Event_t> event = std::make_shared<Event_t>();
    r.add_event(event);
    auto e = r.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(e.get(), event.get());
  }

  TEST_F(ARepository, CopyIsDifferent)
  {
    std::shared_ptr<Event_t> event = std::make_shared<Event_t>();
    r.add_event(event);
    auto r2 = r;
    auto e  = r2.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(r, r2);
    EXPECT_NE(e.get(), event.get());
  }

  TEST_F(ARepository, CanRemoveAnEvent)
  {
    std::shared_ptr<Event_t> event = std::make_shared<Event_t>();
    r.add_event(event);
    auto e = r.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(e.get(), event.get());
    r.remove_event(event);
    e = r.event(event->uuid);
    EXPECT_FALSE(e);
  }

  TEST_F(ARepository, CanBeSavedToJson)
  {
    std::shared_ptr<Event_t> event = std::make_shared<Event_t>();
    r.add_event(event);
    auto e     = r.event(event->uuid);
    using json = nlohmann::json;
    json js    = r;
    std::stringstream ss;
    ss << js;
    std::cout << js << std::endl;
    json js2;
    js2 << ss;
    auto r2 = js2.get<CatalogiCpp::Repository<double>>();
    EXPECT_EQ(r, r2);
  }
} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
