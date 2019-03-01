#include "gtest/gtest.h"
#include <Catalogue.hpp>
#include <CatalogueIO.hpp>
#include <Event.hpp>
#include <Repository.hpp>

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
    Repository_t repo;
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
    auto id = repo.new_catalogue("Catalogue1");
    EXPECT_NE(nullptr, repo.catalogue(id));
    EXPECT_EQ("Catalogue1", repo.catalogue(id)->name);
  }

  TEST_F(ARepository, CanAddAnEvent)
  {
    auto event = Catalogue_t::make_event_ptr();
    repo.add(event);
    auto e = repo.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(e.get(), event.get());
  }

  TEST_F(ARepository, CanAddAnEventFromCatalogue)
  {
    auto event     = Catalogue_t::make_event_ptr();
    auto catalogue = repo.catalogue(repo.new_catalogue("test"));
    // Adding and removing an event to a catalogue should add the event to the
    // repository pool
    catalogue->add(event);
    catalogue->remove(event);
    auto e = repo.event(event->uuid);
    EXPECT_TRUE(e);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(e.get(), event.get());
  }

  TEST_F(ARepository, CopyIsDifferent)
  {
    auto event = Catalogue_t::make_event_ptr();
    repo.add(event);
    auto repo2 = repo;
    auto e     = repo2.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(repo, repo2);
    EXPECT_NE(e.get(), event.get());
  }

  TEST_F(ARepository, CanRemoveAnEvent)
  {
    auto event = Catalogue_t::make_event_ptr();
    repo.add(event);
    auto e = repo.event(event->uuid);
    EXPECT_EQ(*e, *event);
    EXPECT_EQ(e.get(), event.get());
    repo.remove(event);
    e = repo.event(event->uuid);
    EXPECT_FALSE(e);
  }

  TEST_F(ARepository, CanBeSavedToJson)
  {
    auto event = Catalogue_t::make_event_ptr();
    repo.add(event);
    auto e     = repo.event(event->uuid);
    using json = nlohmann::json;
    json js    = repo;
    std::stringstream ss;
    ss << js;
    std::cout << js << std::endl;
    json js2;
    js2 << ss;
    auto repo2 = js2.get<CatalogiCpp::Repository<double>>();
    EXPECT_EQ(repo, repo2);
  }
} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
