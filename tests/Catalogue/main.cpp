#include "gtest/gtest.h"
#include <Catalogue.hpp>
#include <Event.hpp>

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

  TEST_F(ACatalogue, CanBeDefaultConstructed) { EXPECT_FALSE(c.uuid.is_nil()); }

  TEST_F(ACatalogue, CanBeConstructed)
  {
    EXPECT_FALSE(c.uuid.is_nil());
    EXPECT_EQ("Catastrophic", c.name);
  }

} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
