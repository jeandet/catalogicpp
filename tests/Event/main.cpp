#include "gtest/gtest.h"
#include <Catalogue.h>
#include <Event.h>

namespace
{
  class AnEvent : public ::testing::Test
  {
    using Event_t = CatalogiCpp::Event<double>;

  protected:
    AnEvent() {}

    virtual ~AnEvent() {}

    Event_t e = {"Event1",
                 {},
                 {Event_t::Product_t{"Product1", 10., 11.},
                  Event_t::Product_t{"Product2", 11., 12.},
                  Event_t::Product_t{"Product3", 10.2, 11.}},
                 CatalogiCpp::make_uuid()};
  };

  TEST_F(AnEvent, CanBeDefaultConstructed) { EXPECT_FALSE(e.uuid.is_nil()); }

  TEST_F(AnEvent, CanBeConstructed)
  {
    EXPECT_FALSE(e.uuid.is_nil());

    EXPECT_EQ(e.name, "Event1");
  }

  TEST_F(AnEvent, IsUnique)
  {
    auto e1 = CatalogiCpp::Event<double>();
    auto e2 = CatalogiCpp::Event<double>();
    EXPECT_NE(e1.uuid, e2.uuid);
  }

  TEST_F(AnEvent, CanFindStartTime) { EXPECT_EQ(10., e.startTime()); }

  TEST_F(AnEvent, CanFindStopTime) { EXPECT_EQ(12., e.stopTime()); }
} // namespace

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
